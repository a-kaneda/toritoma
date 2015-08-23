/*
 * Copyright (c) 2015 Akihiro Kaneda.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1.Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   2.Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   3.Neither the name of the Monochrome Soft nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*!
 @file SettingFileIO.h
 @brief 設定データファイル入出力
 
 設定データをファイル入出力するクラスを定義する。
 */

#ifndef __toritoma__SettingFileIO__
#define __toritoma__SettingFileIO__

/*!
 @brief 設定データファイル入出力
 
 設定データをファイル入出力する。
 */
class SettingFileIO {
private:
    // UserDefaultの課金済みかどうかのキー
    static const char *UDKeyIsPurchased;
    // UserDefaultのハイスコアのキー
    static const char *UDKeyHighScore;
    // 課金済みかどうか
    bool _isPurchased;
    // ハイスコア
    int _highScore;

public:
    // インスタンス取得
    static SettingFileIO& GetInstance();
    // ファイルを読み込む
    void ReadFile();
    // 課金済みかどうかをファイルに書き込む
    void WriteIsPurchased(bool isPurchased);
    // 課金済みかどうかを取得する
    bool IsPurchased();
    // ハイスコアをファイルに書き込む
    void WriteHighScore(int hiScore);
    // ハイスコアを取得する
    int GetHighScore();
    
private:
    // コンストラクタ
    SettingFileIO();
};

#endif /* defined(__toritoma__SettingFileIO__) */
