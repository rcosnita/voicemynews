package com.voicemynews.core.bindings.analytics;

import android.app.Activity;
import android.content.Context;
import android.webkit.ValueCallback;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import com.voicemynews.core.bindings.config.Config;
import com.voicemynews.voicemynews.utils.Tasks;

import java.util.LinkedList;
import java.util.Queue;

/**
 * Provides a thin wrapper over android web browser and exposes an api compatible with analytics layer.
 */
public class WebBrowserWrapper {
    private class NoOpValueCallback implements ValueCallback<String> {
        @Override
        public void onReceiveValue(String value) {
        }
    }

    /**
     * In case events are sent to the web browser before analytics layer started,
     * we enqueue all actions.
     */
    private interface DelayedSendEvent {
        void send();
    }

    private WebView webBrowser;
    private WebSettings webSettings;
    private boolean browserReady = false;
    private Queue<DelayedSendEvent> pendingSendEvents = new LinkedList<>();

    public WebBrowserWrapper(Context ctx) {
        WebView.setWebContentsDebuggingEnabled(true);

        this.webBrowser = new WebView(ctx);
        this.webSettings = this.webBrowser.getSettings();

        this.webSettings.setJavaScriptEnabled(true);

        this.webBrowser.setWebViewClient(new WebViewClient() {
            @Override
            public void onPageFinished(WebView view, String url) {
                browserReady = true;
                flushPendingEvents();
            }
        });

        this.initNativeBrowser();
    }

    public void loadContent(String content, String contentType) {
        String baseUrl = Config.getWebBaseUrl();
        this.webBrowser.loadDataWithBaseURL(baseUrl, content, contentType, "UTF-8", "");
    }

    /**
     * Build a log message and sends it to the underlying browser as post message.
     *
     * @param evtName the event name we want to dispatch to the browser.
     * @param evtData the json event data represented as string.
     */
    public void sendEvent(String evtName, String evtData) {
        final String postMessageJS = String.format("window.postMessage(`%s`, '*')", evtData);

        if (browserReady) {
            Tasks.runOnUiThread(new Runnable() {
                  @Override
                  public void run() {
                      webBrowser.evaluateJavascript(postMessageJS, new NoOpValueCallback());
                  }
            });
            return;
        }

        pendingSendEvents.add(new DelayedSendEvent() {
            @Override
            public void send() {
                Tasks.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        webBrowser.evaluateJavascript(postMessageJS, new NoOpValueCallback());
                    }
                });
            }
        });
    }

    /**
     * Execute all pending events queued before page loading was complete.
     */
    private void flushPendingEvents() {
        for (DelayedSendEvent evt : pendingSendEvents) {
            evt.send();
        }
    }

    /**
     * Allows the wiring of a native web browser contract implementation. This is the glue layer for
     * exposing analytics layer in js / java / c++ code.
     */
    private native void initNativeBrowser();
}
