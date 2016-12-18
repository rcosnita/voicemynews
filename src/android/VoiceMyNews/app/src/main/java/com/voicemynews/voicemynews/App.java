package com.voicemynews.voicemynews;

import android.app.Application;

import com.voicemynews.core.bindings.events.EventDataBindingNative;
import com.voicemynews.core.bindings.events.EventHandler;
import com.voicemynews.core.bindings.events.EventLoopBindingNative;
import com.voicemynews.core.bindings.network.HttpClientBinding;
import com.voicemynews.core.bindings.news.VoiceSupport;
import com.voicemynews.core.bindings.news.VoiceSupportActions;
import com.voicemynews.core.bindings.news.VoiceSupportAndroid;
import com.voicemynews.voicemynews.models.SideMenuState;

/**
 * Provides an app implementation which allows us to initialize the native platform and bootstrap
 * v8 engine only once.
 */
public class App extends Application {
    private String appStartKey;
    private static App currAppInstance = null;
    private EventLoopBindingNative eventLoop = null;
    private VoiceSupport voiceSupport = null;
    private Thread v8Thread = null;
    private final SideMenuState sideMenuState = new SideMenuState();

    static {
        System.loadLibrary("voicemynews-core");
        System.loadLibrary("voicemynews-android");
    }

    /**
     * Provides a mechanism for allowing external activities to listen to on app started. At this point,
     * we know for sure that there is only one listener interested into this event.
     */
    public interface OnAppStarted {
        void doAction(EventDataBindingNative evtData);
    }

    private EventDataBindingNative appStartedEvtData = null;
    private OnAppStarted appStartedListener = null;
    public OnAppStarted getAppStartedListener() {
        return appStartedListener;
    }

    public void setAppStartedListener(OnAppStarted appStartedListener) {
        this.appStartedListener = appStartedListener;

        if (appStartedEvtData != null) {
            appStartedListener.doAction(appStartedEvtData);
        }
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

        appStartKey = eventLoop.on("app:js:start", new EventHandler() {
            @Override
            public void handleEvent(EventDataBindingNative evtData) {
                appStartedEvtData = evtData;

                if (appStartedListener != null) {
                    appStartedListener.doAction(evtData);
                    return;
                }
            }
        });
    }

    /**
     * Provides the logic for unregistering all listeners from app start event.
     */
    public void clearAppStartedListeners() {
        eventLoop.off(appStartKey);
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
        voiceSupport = VoiceSupportAndroid.getInstance();

        v8Thread = new Thread(new Runnable() {
            @Override
            public void run() {
                HttpClientBinding.initializeNative(new HttpClientBinding());
                JsApp.startPlatform(eventLoop.getNativeEmitterPtr(), getAssets(), voiceSupport);
            }
        });
        v8Thread.setName("V8 JS engine");
        v8Thread.start();
    }
}
