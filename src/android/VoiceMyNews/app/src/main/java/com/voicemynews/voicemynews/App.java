package com.voicemynews.voicemynews;

import android.app.Application;
import com.voicemynews.core.bindings.events.EventLoopBindingNative;
import com.voicemynews.core.bindings.network.HttpClientBinding;
import com.voicemynews.voicemynews.models.SideMenuState;

/**
 * Provides an app implementation which allows us to initialize the native platform and bootstrap
 * v8 engine only once.
 */
public class App extends Application {
    private static App currAppInstance = null;
    private EventLoopBindingNative eventLoop = null;
    private Thread v8Thread = null;
    private final SideMenuState sideMenuState = new SideMenuState();

    static {
        System.loadLibrary("voicemynews-core");
        System.loadLibrary("voicemynews-android");
    }

    /**
     * Returns the current event loop which allows communication between js engine and java app.
     * @return
     */
    public EventLoopBindingNative getEventLoop() {
        return eventLoop;
    }

    /**
     * Returns the current application instance. This might be necessary in order to be able to
     * navigate without an activity context.
     * @return
     */
    public static App getCurrent() {
        return currAppInstance;
    }

    /**
     * Returns the current state of the side menu. It is used as persistent mechanism of a side menu
     * between various activities of the application.
     * @return
     */
    public SideMenuState getSideMenuState() {
        return sideMenuState;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        currAppInstance = this;
        initAppJsLogic();
    }

    /**
     * Provides the logic for bootstraping v8 engine and voicemynews business logic.
     */
    private void initAppJsLogic()
    {
        if (eventLoop != null) {
            return;
        }

        eventLoop = (EventLoopBindingNative)EventLoopBindingNative.getInstance();

        v8Thread = new Thread(new Runnable() {
            @Override
            public void run() {
                HttpClientBinding.initializeNative(new HttpClientBinding());
                JsApp.startPlatform(eventLoop.getNativeEmitterPtr(), getAssets());
            }
        });
        v8Thread.start();
    }
}
