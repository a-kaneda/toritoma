/****************************************************************************
Copyright (c) 2015 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import android.app.Dialog;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.content.IntentSender.SendIntentException;
import android.content.DialogInterface;
import android.support.v4.app.DialogFragment;
import android.util.Log;
import android.view.WindowManager;
import android.widget.LinearLayout;
import android.view.Gravity;
import android.view.View;

import org.cocos2dx.lib.GameControllerActivity;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

import com.google.android.gms.ads.AdSize;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.GoogleApiAvailability;
import com.google.android.gms.games.Games;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;
import com.google.android.gms.ads.AdListener;

import com.monochromesoft.toritoma2.R;

public class AppActivity extends GameControllerActivity implements
        GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener {

    /** Twitterのパッケージ名 */
    private final static String TWITTER_PACKAGE_NAME = "com.twitter.android";

    /** Unique tag for the error dialog fragment */
    private static final String DIALOG_ERROR = "dialog_error";

    /** Request code to use when launching the resolution activity */
    private static final int REQUEST_RESOLVE_ERROR = 1001;

    /** Leaderboard表示のリクエストコード */
    private static final int REQUEST_LEADERBOARD = 1002;

    /** static関数からメンバ関数呼び出すためのthisインスタンス */
    private static AppActivity me = null;

    /** Google APIにアクセスするためのクライアント */
    private GoogleApiClient mGoogleApiClient;

    /** Google API接続エラーを解決中かどうか */
    private boolean mResolvingError = false;

    /** バナー広告 */
    private AdView mAdView;

    /** インタースティシャル広告 */
    private InterstitialAd mInterstitialAd;

    /**
     * Activityの生成処理。
     * JNIからメンバ変数を呼び出せるようにstaticメンバにthisインスタンスを設定する。
     * Google APIクライアントを作成する。
     * @param savedInstanceState 状態変数
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        //Log.d("toritoma", "onCreate: begin.");

        // 基底クラスの処理を呼び出す
        super.onCreate(savedInstanceState);

        // スリープを無効化する
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        // staticメンバにthisインスタンスを設定する
        me = this;

        // GoogleAPIクライアントを作成する
        mGoogleApiClient = new GoogleApiClient.Builder(this)
                .addConnectionCallbacks(this)
                .addOnConnectionFailedListener(this)
                .addApi(Games.API).addScope(Games.SCOPE_GAMES)
                .build();

        // 広告バナーを作成する
        createAdBanner();

        // アプリ課金後は表示されないようにするため、デフォルトは非表示としておく。
        hideAdBanner();

        // インタースティシャル広告を作成する
        mInterstitialAd = new InterstitialAd(this);
        mInterstitialAd.setAdUnitId(getString(R.string.banner_ad_unit_id));
        requestNewInterstitial();

        // インタースティシャル広告を閉じた時に次の広告を読み込むためにリスナーを登録する
        mInterstitialAd.setAdListener(new AdListener() {

            @Override
            public void onAdClosed() {
                requestNewInterstitial();
            }
        });

        //The standard controller,without doing anything special. e.g: Amazon Fire TV
        // Manually specify an adapter.
        this.connectController(DRIVERTYPE_STANDARD);

        //Log.d("toritoma", "onCreate: end.");
    }

    /**
     * Activity開始時の処理。
     * Google APIへ接続する。
     */
    @Override
    protected void onStart() {

        //Log.d("toritoma", "onStart: begin.");

        // 基底クラスの処理を呼び出す
        super.onStart();

        // エラー解決中でない場合はGoogle APIへの接続を行う。
        // Googleへのサインインを行って戻る場合などもonStartが走るが、
        // この時にも接続を行おうとすると二重に接続することになるので、
        // これを防止するため。
        if (!mResolvingError) {
            mGoogleApiClient.connect();
        }

        //Log.d("toritoma", "onStart: end.");
    }

    /**
     * Activity停止時の処理。
     * Google APIを切断する。
     */
    @Override
    protected void onStop() {

        //Log.d("toritoma", "onStop: begin.");

        // Google APIを切断する
        mGoogleApiClient.disconnect();

        // 基底クラスの処理を呼び出す
        super.onStop();

        //Log.d("toritoma", "onStop: end.");
    }

    /**
     * アクティビティの結果取得。
     * エラー解決アクティビティから正常終了を受け取った場合はGoogle APIへ接続する。
     * @param requestCode
     * @param resultCode
     * @param data
     */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {

        //Log.d("toritoma", "onActivityResult: begin.");

        // エラー解決アクティビティからの呼び出しの場合
        if (requestCode == REQUEST_RESOLVE_ERROR) {

            // エラー解決中のフラグを落とす
            mResolvingError = false;

            // 結果が正常終了の場合
            if (resultCode == RESULT_OK) {

                // Make sure the app is not already connected or attempting to connect
                if (!mGoogleApiClient.isConnecting() &&
                        !mGoogleApiClient.isConnected()) {

                    // Google APIへ接続する
                    mGoogleApiClient.connect();
                }
            }
        }

        //Log.d("toritoma", "onActivityResult: end.");
    }

    /**
     * Google APIに接続した時の処理。無処理。
     * @param connectionHint
     */
    @Override
    public void onConnected(Bundle connectionHint) {

        //Log.d("toritoma", "onConnected: begin.");

        // 無処理

        //Log.d("toritoma", "onConnected: end.");
    }

    /**
     * Google APIへの接続が中断された時の処理。
     * 再度、Google APIへの接続を試みる。
     * @param cause
     */
    @Override
    public void onConnectionSuspended(int cause) {

        //Log.d("toritoma", "onConnectionSuspended: begin.");

        // Google APIへ接続する
        mGoogleApiClient.connect();

        //Log.d("toritoma", "onConnectionSuspended: end.");
    }

    /**
     * Google APIへの接続に失敗した時の処理。
     * 解決方法がある場合は解決を試みる。
     * @param result
     */
    @Override
    public void onConnectionFailed(ConnectionResult result) {

        //Log.d("toritoma", "onConnectionFailed: begin.");

        if (mResolvingError) {

            // すでにエラー解決中の場合は何もしない
        }
        else if (result.hasResolution()) {

            // 解決方法がある場合は解決を試みる
            try {

                // エラー解決中のフラグを立てる
                mResolvingError = true;

                // 解決を試みる
                result.startResolutionForResult(this, REQUEST_RESOLVE_ERROR);

            } catch (SendIntentException e) {

                // エラー解決中に例外が発生した場合は、再度接続を試みる
                mGoogleApiClient.connect();
            }
        }
        else {

            // 解決方法がない場合はエラーダイアログを表示する
            showErrorDialog(result.getErrorCode());
            mResolvingError = true;
        }

        //Log.d("toritoma", "onConnectionFailed: end.");
    }

    /**
     * Twitter投稿用のインテントを作成し、アクティビティを呼び出す。
     * @param message ツイート内容
     * @param imagePath 画像パス
     */
    public static void postTweet(String message, String imagePath) {

        //Log.d("toritoma", "postTweet: begin.");

        // インテントを作成する。
        Intent intent = new Intent(Intent.ACTION_SEND);

        // インテントにメッセージを追加する
        intent.putExtra(Intent.EXTRA_TEXT, message);

        // パッケージタイプをTwitterにする
        intent.setPackage(TWITTER_PACKAGE_NAME);

        // 画像パスが指定されている場合は画像の処理を行う
        if (!imagePath.equals("")) {

            try {

                // 画像パスからURIを作成する
                Uri originalImageUri = Uri.parse(imagePath);

                // 画像の拡張子を取得する
                String ext = getExtension(originalImageUri);

                // 他のアプリから読み込めるように外部ストレージへと画像をコピーする
                Uri readableFileUri = saveImageToExternalDirectory(originalImageUri);

                // 画像のコピーに成功した場合はインテントに格納する
                if (readableFileUri != null) {
                    intent.setType("image/" + ext);
                    intent.putExtra(Intent.EXTRA_STREAM, readableFileUri);
                }

            } catch (Exception e) {

                // 画像のコピーに失敗した場合は画像なしでツイートを行う
                // ここでは何もしない
            }
        }

        try {

            // アクティビティを呼び出す。
            me.startActivity(intent);

        } catch (ActivityNotFoundException e) {

            // Twitterクライアントがない場合は代わりにブラウザでTwitterのサイトを開くようにする。
            // 画像を付けることはできないため、テキストメッセージだけを付ける。

            // Twitter投稿用インテントに設定するURLを作成する。
            // TwitterのURLのパラメータにメッセージを設定する。
            // ハッシュタグの#の文字はそのままでは処理できないため、URLエンコードの%23に置換する。
            String twitterUrl = String.format("http://twitter.com/share?text=%s", message).replaceAll("#", "%23");

            // ブラウザ表示用のインテントを作成する
            Intent intentForBrowser = new Intent(Intent.ACTION_VIEW, Uri.parse(twitterUrl));

            // アクティビティを呼び出す。
            me.startActivity(intentForBrowser);
        }

        //Log.d("toritoma", "postTweet: end.");
    }

    /**
     * Leaderboardの画面を表示する。
     */
    public static void openRanking() {

        //Log.d("toritoma", "openRanking: begin. mGoogleApiClient.isConnected=" + (me.mGoogleApiClient == null ? "(null)" : me.mGoogleApiClient.isConnected()));

        if (me.mGoogleApiClient != null && me.mGoogleApiClient.isConnected()) {

            // Leaderboardのアクティビティを開始する
            me.startActivityForResult(
                    Games.Leaderboards.getLeaderboardIntent(
                            me.mGoogleApiClient,
                            me.getString(R.string.leaderboard_id)),
                    REQUEST_LEADERBOARD);
        }

        //Log.d("toritoma", "openRanking: end.");
    }

    /**
     * Leaderboardにハイスコアを送信する。
     * @param score スコア
     */
    public static void postHighScore(int score) {

        //Log.d("toritoma", "postHighScore: begin.");

        if (me.mGoogleApiClient != null && me.mGoogleApiClient.isConnected()) {

            // Leaderboardにハイスコアを送信する
            Games.Leaderboards.submitScore(
                    me.mGoogleApiClient,
                    me.getString(R.string.leaderboard_id),
                    score);
        }

        //Log.d("toritoma", "postHighScore: end.");
    }

    /**
     * バナー広告を表示する。
     */
    public static void viewAdBanner() {

        //Log.d("toritoma", "viewAdBanner: begin.");

        // 広告表示切替のスレッドを開始する
        me.runOnUiThread(new Runnable() {

            @Override
            public void run() {

                // 広告が無効な場合は有効にする
                if (!me.mAdView.isEnabled()) {
                    me.mAdView.setEnabled(true);
                }

                // 広告が表示でない場合は表示にする
                if (me.mAdView.getVisibility() != View.VISIBLE) {
                    me.mAdView.setVisibility(View.VISIBLE);
                }
            }
        });

        //Log.d("toritoma", "viewAdBanner: end.");
    }

    /**
     * バナー広告を非表示にする。
     */
    public static void hideAdBanner() {

        //Log.d("toritoma", "hideAdBanner: begin.");

        // 広告表示切替のスレッドを開始する
        me.runOnUiThread(new Runnable() {

            @Override
            public void run() {

                // 広告が有効な場合は無効にする
                if (me.mAdView.isEnabled()) {
                    me.mAdView.setEnabled(false);
                }

                // 広告が非表示でない場合は非表示にする
                if (me.mAdView.getVisibility() != View.INVISIBLE) {
                    me.mAdView.setVisibility(View.INVISIBLE);
                }
            }
        });

        //Log.d("toritoma", "hideAdBanner: end.");
    }

    /**
     * インタースティシャル広告を表示する。
     */
    public static void viewAdInterstitial() {

        //Log.d("toritoma", "viewAdInterstitial: begin.");

        // 広告表示のスレッドを開始する
        me.runOnUiThread(new Runnable() {

            @Override
            public void run() {

                // 広告が読み込まれている場合は広告を表示する
                if (me.mInterstitialAd.isLoaded()) {
                    me.mInterstitialAd.show();
                }
            }
        });

        //Log.d("toritoma", "viewAdInterstitial: end.");
    }

    /**
     * パスからファイルの拡張子を取得する。
     * @param imagePath 画像ファイルのパス
     * @return 画像ファイルの拡張子
     */
    private static String getExtension(Uri imagePath) {

        //Log.d("toritoma", "getExtension: begin.");

        // URIからパス文字列を取得する
        String pathString = imagePath.getPath();

        //Log.d("toritoma", "getExtension: end.");

        // 一番後ろの"."以降の文字列を切り出して返す
        return pathString.substring(pathString.lastIndexOf(".") + 1);
    }

    /**
     * パスからファイル名を取得する。
     * @param imagePath 画像ファイルのパス
     * @return 画像ファイルのファイル名
     */
    private static String getFilename(Uri imagePath) {

        //Log.d("toritoma", "getFilename: begin.");

        //Log.d("toritoma", "getFilename: end.");

        // URLの最後のセグメントを返す
        return imagePath.getLastPathSegment();
    }

    /**
     * ファイルを外部ストレージへ保存する。
     * @param imageUri 元画像のURI
     * @return 外部ストレージのURI
     */
    private static Uri saveImageToExternalDirectory(Uri imageUri) {

        //Log.d("toritoma", "saveImageToExternalDirectory: begin.");

        // コピー先ファイルのFileインスタンスを作成する
        File dst = new File(Environment.getExternalStoragePublicDirectory(
                Environment.DIRECTORY_PICTURES), getFilename(imageUri));

        // コピー元ファイルのFileインスタンスを作成する
        File src = new File(imageUri.getPath());

        try {

            // ファイルをコピーする
            copyFile(src, dst);

        } catch (Exception e) {

            return null;
        }

        //Log.d("toritoma", "saveImageToExternalDirectory: end.");

        // コピー先ファイルからURIを作成して返す
        return Uri.fromFile(dst);
    }

    /**
     * ファイルをコピーする。
     * @param src コピー元ファイル
     * @param dst コピー先ファイル
     * @throws Exception ファイルアクセス異常
     */
    private static void copyFile(File src, File dst) throws Exception {

        //Log.d("toritoma", "copyFile: begin.");

        // コピー元ファイルの入力ストリームを作成する
        InputStream in = new FileInputStream(src);

        // コピー先ファイルの出力ストリームを作成数r
        OutputStream out = new FileOutputStream(dst);

        // コピー元ファイルの内容を読み込み、コピー先ファイルへ書き込んでいく
        byte[] buf = new byte[1024];
        int len;
        while ((len = in.read(buf)) > 0) {
            out.write(buf, 0, len);
        }

        // 入出力ストリームを閉じる
        in.close();
        out.close();

        //Log.d("toritoma", "copyFile: end.");
    }

    /**
     * Creates a dialog for an error message
     * @param errorCode エラーコード
     */
    private void showErrorDialog(int errorCode) {

        //Log.d("toritoma", "showErrorDialog: begin.");

        // Create a fragment for the error dialog
        ErrorDialogFragment dialogFragment = new ErrorDialogFragment();

        // Pass the error that should be displayed
        Bundle args = new Bundle();
        args.putInt(DIALOG_ERROR, errorCode);
        dialogFragment.setArguments(args);
        dialogFragment.show(getSupportFragmentManager(), "errordialog");

        //Log.d("toritoma", "showErrorDialog: end.");
    }

    /**
     *  Called from ErrorDialogFragment when the dialog is dismissed.
     */
    public void onDialogDismissed() {

        //Log.d("toritoma", "onDialogDismissed: begin.");

        mResolvingError = false;

        //Log.d("toritoma", "onDialogDismissed: end.");
    }

    /**
     * 広告バナーを作成する。
     */
    private void createAdBanner() {

        // 広告ビューのレイアウトパラメータを作成する
        LinearLayout.LayoutParams adParams = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT);

        // 表示位置を左上に設定する
        adParams.gravity = (Gravity.TOP | Gravity.LEFT);

        // 広告ビューを作成する
        mAdView = new AdView(this);
        mAdView.setAdSize(AdSize.BANNER);
        mAdView.setAdUnitId(getString(R.string.banner_ad_unit_id));

        // テストデバイスのIDを指定してAdRequestを作成する
        AdRequest adRequest = new AdRequest.Builder()
                .addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
                .addTestDevice(getString(R.string.test_device_id))
                .build();

        // 広告を読み込む
        mAdView.loadAd(adRequest);

        // 広告ビューをアクティビティに追加する
        addContentView(mAdView, adParams);
    }

    /**
     * インタースティシャル広告を要求する。
     */
    private void requestNewInterstitial() {

        // テストデバイスのIDを指定してAdRequestを作成する
        AdRequest adRequest = new AdRequest.Builder()
                .addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
                .addTestDevice(me.getString(R.string.test_device_id))
                .build();

        // 広告を読み込む
        mInterstitialAd.loadAd(adRequest);
    }

    /**
     *  A fragment to display an error dialog
     */
    public static class ErrorDialogFragment extends DialogFragment {

        /**
         * コンストラクタ。無処理。
         */
        public ErrorDialogFragment() {

            //Log.d("toritoma", "ErrorDialogFragment: begin.");

            //Log.d("toritoma", "ErrorDialogFragment: end.");
        }

        /**
         * ダイアログ作成処理。
         * Google APIのエラーダイアログを作成する。
         * @param savedInstanceState 状態変数
         * @return エラーダイアログ
         */
        @Override
        public Dialog onCreateDialog(Bundle savedInstanceState) {

            //Log.d("toritoma", "ErrorDialogFragment.onCreateDialog: begin.");

            // Get the error code and retrieve the appropriate dialog
            int errorCode = this.getArguments().getInt(DIALOG_ERROR);

            //Log.d("toritoma", "ErrorDialogFragment.onCreateDialog: end.");

            // エラーダイアログを作成する
            return GoogleApiAvailability.getInstance().getErrorDialog(
                    this.getActivity(), errorCode, REQUEST_RESOLVE_ERROR);
        }

        /**
         * 終了時処理。
         * アクティビティのダイアログ終了処理を呼び出す。
         * @param dialog ダイアログ
         */
        @Override
        public void onDismiss(DialogInterface dialog) {

            //Log.d("toritoma", "ErrorDialogFragment.onDismiss: begin.");

            // アクティビティのダイアログ終了処理を呼び出す
            ((AppActivity)getActivity()).onDialogDismissed();

            //Log.d("toritoma", "ErrorDialogFragment.onDismiss: end.");
        }
    }
}
