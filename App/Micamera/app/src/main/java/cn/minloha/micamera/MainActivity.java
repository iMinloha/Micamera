package cn.minloha.micamera;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.app.DownloadManager;
import android.net.Uri;
import android.net.http.SslError;
import android.os.StrictMode;
import android.util.Log;
import android.view.KeyEvent;
import android.webkit.*;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;

import static android.os.Environment.DIRECTORY_DCIM;
import static android.os.Environment.DIRECTORY_DOWNLOADS;

public class MainActivity extends AppCompatActivity {

    @SuppressLint("SetJavaScriptEnabled")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);

        WebView browser = (WebView) findViewById(R.id.Toweb);
        browser.getSettings().setDomStorageEnabled(true);
        browser.getSettings().setDatabaseEnabled(true);
        browser.getSettings().setJavaScriptEnabled(true);
        browser.getSettings().setSupportZoom(true);
        browser.getSettings().setBuiltInZoomControls(true);
        String path = "http://10.0.4.1/";
        try {
            URL url = new URL(path);
            HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();
            httpURLConnection.setConnectTimeout(3000);
            httpURLConnection.connect();
            int code = httpURLConnection.getResponseCode();
            if (code == 200)  browser.loadUrl(path);
            else{
                browser.loadUrl("file:///android_asset/html/non.html");
                Log.d("MicameraInfo","Http");
            }
        } catch (IOException e) {
            browser.loadUrl("file:///android_asset/html/502.html");
            Log.d("MicameraInfo","HttpError");
        }

        browser.setWebViewClient(new WebViewClient() {
            @Override
            public void onReceivedHttpError(WebView view, WebResourceRequest request, WebResourceResponse errorResponse) {
                super.onReceivedHttpError(view, request, errorResponse);
                int statusCode = errorResponse.getStatusCode();
                Log.d("MINFO","Code is:" + statusCode);
            }

            @SuppressLint("WebViewClientOnReceivedSslError")
            @Override
            public void onReceivedSslError(WebView view, SslErrorHandler handler, SslError error) {
                if (handler != null) {
                    handler.proceed();
                }
            }
        });

        browser.setDownloadListener((url, userAgent, contentDisposition, mimetype, contentLength) -> {
            // 指定下载地址
            String Filetype = url.substring(url.length()-3);
            String filename = String.valueOf(System.currentTimeMillis()) + url.substring(url.length()-4);
            DownloadManager.Request request = new DownloadManager.Request(Uri.parse(url));
            request.setAllowedOverMetered(true);
            request.setAllowedOverRoaming(true);
            String info = "";
            if(Filetype.equals("pdf")) {
                filename = "产品说明书.pdf";
                info = "在文件管理器中的文档中查看\"产品说明书.pdf\"";
                request.setDestinationInExternalPublicDir(DIRECTORY_DOWNLOADS, filename);
            }else {
                request.setDestinationInExternalPublicDir(DIRECTORY_DCIM, filename);
                info = "在相册中的DCIM下查看";
            }
            final DownloadManager downloadManager = (DownloadManager) getSystemService(DOWNLOAD_SERVICE);
            long downloadId = downloadManager.enqueue(request);
            new AlertDialog.Builder(this).setTitle("下载成功!").setMessage(info).setPositiveButton("我知道了",null).show();
        });

    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        WebView browser=(WebView)findViewById(R.id.Toweb);
        if ((keyCode == KeyEvent.KEYCODE_BACK) && browser.canGoBack()) {
            browser.goBack();
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }

}