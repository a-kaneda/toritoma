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
 @file SettingFileIO.cpp
 @brief 設定データ管理
 
 設定データをファイル入出力するクラスを定義する。
 */

#include "SettingFileIO.h"
#include "cocos2d.h"
#include "ID.h"

using cocos2d::UserDefault;

/// UserDefaultのハイスコアのキー
const char *SettingFileIO::UDKeyHighScore = "HighScore";

// スレッドセーフにするため、グローバル変数初期化処理でインスタンスの生成を行う
static SettingFileIO &_instance = SettingFileIO::GetInstance();

/*!
 @brief インスタンス取得
 
 シングルトンインスタンスを取得する。
 @return インスタンス
 */
SettingFileIO& SettingFileIO::GetInstance()
{
    // シングルトンインスタンス
    static SettingFileIO instance;
    
    return instance;
}

/*!
 @brief ファイルを読み込む
 
 ファイルを読み込んでメンバ変数に取り込む。
 */
void SettingFileIO::ReadFile()
{
    // UserDefaultインスタンスを取得する
    UserDefault *ud = UserDefault::getInstance();

    // ハイスコアを読み込む
    _highScore = ud->getIntegerForKey(UDKeyHighScore, 0);
}

/*!
 @brief ハイスコアをファイルに書き込む
 
 ハイスコアをファイルに書き込む。
 @param hiScore ハイスコア
 */
void SettingFileIO::WriteHighScore(int highScore)
{
    // UserDefaultインスタンスを取得する
    UserDefault *ud = UserDefault::getInstance();
    
    // ハイスコアを書き込む
    ud->setIntegerForKey(UDKeyHighScore, highScore);

    // メンバ変数の内容を変更する
    _highScore = highScore;
}

/*!
 @brief ハイスコアを取得する

 ハイスコアを取得する。
 @return ハイスコア
 */
int SettingFileIO::GetHighScore()
{
    return _highScore;
}

/*!
 @brief コンストラクタ
 
 メンバ変数を初期化する。
 */
SettingFileIO::SettingFileIO()
{
    // メンバ変数を初期化する
    _highScore = 0;
}