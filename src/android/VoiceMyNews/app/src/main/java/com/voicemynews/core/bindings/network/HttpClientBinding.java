package com.voicemynews.core.bindings.network;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Provides a HttpClient implementation which can be consumed by native part of the code.
 */
public class HttpClientBinding {
    /**
     * Provides a inner data structure which carries information about a get response.
     */
    private class HttpClientBindingGetResponse {
        private int statusCode;
        private Map<String, String> headers;
        private InputStream stream;

        HttpClientBindingGetResponse(int statusCode, Map<String, String> headers,
                 InputStream stream) {
            this.statusCode = statusCode;
            this.headers = headers;
            this.stream = stream;
        }
    }

    private final static int MaxNumberOfHttpThreads = 20;

    /**
     * Provides the pool of threads which can be safely used for http requests.
     */
    private final static ExecutorService HttpClientBindingPool = Executors.newFixedThreadPool(MaxNumberOfHttpThreads);

    /**
     * Provides the logic for wiring the client into the native part of the code.
     * @param httpClient The http client instance which can be used by native part to make http requests.
     */
    public static native void initializeNative(HttpClientBinding httpClient);

    /**
     * Provides the http get operation. It uses the url and the headers in order to execute an HTTP
     * get request. When the request is complete it invokes the callback.
     *
     * @param url the url we want to fetch.
     * @param headers the headers we want to pass to request.
     * @param callback the callback invoked once the request is complete.
     */
    public void get(final String url, final Map<String, String> headers, final HttpClientBindingGetAction callback) {
        HttpClientBindingPool.submit(new Runnable() {
            @Override
            public void run() {
                try {
                    URL httpUrl = new URL(url);
                    HttpURLConnection conn = (HttpURLConnection) httpUrl.openConnection();
                    conn.setReadTimeout(10000);
                    conn.setConnectTimeout(15000);
                    conn.setRequestMethod("GET");
                    conn.setDoInput(true);

                    conn.connect();
                    Map<String, String> headers = new HashMap<>();
                    for(String headerName : conn.getHeaderFields().keySet()) {
                        headers.put(headerName, conn.getHeaderFields().get(headerName).get(0));
                    }

                    callback.onComplete(new HttpClientBindingGetResponse(conn.getResponseCode(), headers, conn.getInputStream()));
                } catch(IOException ioex) {
                    // TODO [rcosnita] handle io exception.
                    System.out.println(ioex);
                }
            }
        });
    }

    /**
     * Provides the functionality of parsing a request response data object. Once it completes, it
     * invokes the callback with the resulting string.
     *
     * @param responseData the response data object we want to parse.
     * @param callback the callback which must be invoked.
     */
    public void parseResponseWithStringContent(final Object responseData,
                                       final HttpClientBindingParseStringContentAction callback) {
        HttpClientBindingPool.submit(new Runnable() {
            @Override
            public void run() {
                HttpClientBindingGetResponse response = (HttpClientBindingGetResponse)responseData;

                try {
                    Reader reader = null;
                    reader = new InputStreamReader(response.stream, "UTF-8");
                    char[] buffer = new char[Integer.parseInt(response.headers.get("Content-Length"))];
                    reader.read(buffer);
                    callback.onParsed(new String(buffer));
                } catch(Exception ex) {
                    // TODO [rcosnita] handle exception.
                    System.out.println(ex);
                } finally {
                    try {
                        response.stream.close();
                    } catch(Exception ex) {
                        // Nothing to do at this point. Just let it pass.
                    }
                }
            }
        });
    }

    /**
     * Provides a factory method which builds a new headers map instance.
     * @return
     */
    public Map<String, String> getNewHeadersMap() {
        return new HashMap<>();
    }
}
