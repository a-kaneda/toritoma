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

import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;

import org.cocos2dx.lib.Cocos2dxActivity;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

public class AppActivity extends Cocos2dxActivity {

    /** Twitterのパッケージ名 */
    final static String TWITTER_PACKAGE_NAME = "com.twitter.android";

    /** static関数からメンバ関数呼び出すためのthisインスタンス */
    private static AppActivity me = null;

    /**
     * JNIからメンバ変数を呼び出せるようにstaticメンバにthisインスタンスを設定する。
     * @param savedInstanceState 状態変数
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        me = this;
    }

    /**
     * Twitter投稿用のインテントを作成し、アクティビティを呼び出す。
     * @param message ツイート内容
     * @param url ツイートURL
     * @param imagePath 画像パス
     */
    public static void postTweet(String message, String url, String imagePath) {

        // Twitter投稿用インテントに設定するURLを作成する。
        // TwitterのURLのパラメータにメッセージとURLを設定する。
        // ハッシュタグの#の文字はそのままでは処理できないため、URLエンコードの%23に置換する。
        String twitterUrl = String.format("http://twitter.com/share?text=%s&url=%s", message, url).replaceAll("#", "%23");

        // インテントを作成する。
        Intent intent = new Intent(Intent.ACTION_SEND, Uri.parse(twitterUrl));

        // インテントにメッセージを追加する
        intent.putExtra(Intent.EXTRA_TEXT, message);

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

                // パッケージタイプをTwitterにする
                intent.setPackage(TWITTER_PACKAGE_NAME);

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

            // ブラウザ表示用のインテントを作成する
            Intent intentForBrowser = new Intent(Intent.ACTION_VIEW, Uri.parse(twitterUrl));

            // アクティビティを呼び出す。
            me.startActivity(intentForBrowser);
        }
    }

    /**
     * パスからファイルの拡張子を取得する。
     * @param imagePath 画像ファイルのパス
     * @return 画像ファイルの拡張子
     */
    static private String getExtension(Uri imagePath) {

        // URIからパス文字列を取得する
        String pathString = imagePath.getPath();

        // 一番後ろの"."以降の文字列を切り出して返す
        return pathString.substring(pathString.lastIndexOf(".") + 1);
    }

    /**
     * パスからファイル名を取得する。
     * @param imagePath 画像ファイルのパス
     * @return 画像ファイルのファイル名
     */
    static private String getFilename(Uri imagePath) {

        // URLの最後のセグメントを返す
        return imagePath.getLastPathSegment();
    }

    /**
     * ファイルを外部ストレージへ保存する。
     * @param imageUri 元画像のURI
     * @return 外部ストレージのURI
     */
    static private Uri saveImageToExternalDirectory(Uri imageUri) {

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

        // コピー先ファイルからURIを作成して返す
        return Uri.fromFile(dst);
    }

    /**
     * ファイルをコピーする。
     * @param src コピー元ファイル
     * @param dst コピー先ファイル
     * @throws Exception ファイルアクセス異常
     */
    static public void copyFile(File src, File dst) throws Exception {

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
    }
}
