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

using cocos2d::UserDefault;

/// UserDefaultの課金済みかどうかのキー
const char *SettingFileIO::UDKeyIsPurchased = "IsPurchased";
/// UserDefaultのハイスコアのキー
const char *SettingFileIO::UDKeyHighScore = "HighScore";

/*!
 @brief 課金済みかどうかをファイルに書き込む
 
 課金済みかどうかをファイルに書き込む。
 @param isPurhcased 課金済みかどうか
 */
void SettingFileIO::WriteIsPurchased(bool isPurchased)
{
    // UserDefaultインスタンスを取得する
    UserDefault *ud = UserDefault::getInstance();
    
    // 課金済みかどうかを書き込む
    ud->setBoolForKey(UDKeyIsPurchased, isPurchased);
}

/*!
 @brief 課金済みかどうかをファイルから読み込む
 
 課金済みかどうかをファイルから読み込む。
 @return 課金済みかどうか
 */
bool SettingFileIO::ReadIsPurchased()
{
    // UserDefaultインスタンスを取得する
    UserDefault *ud = UserDefault::getInstance();
    
    // 課金済みかどうかを取得する
    bool isPurchased = ud->getBoolForKey(UDKeyIsPurchased, false);
    
    return isPurchased;
}

/*!
 @brief ハイスコアをファイルに書き込む
 
 ハイスコアをファイルに書き込む。
 @param hiScore ハイスコア
 */
void SettingFileIO::WriteHighScore(int hiScore)
{
    // UserDefaultインスタンスを取得する
    UserDefault *ud = UserDefault::getInstance();
    
    // ハイスコアを書き込む
    ud->setIntegerForKey(UDKeyHighScore, hiScore);
}

/*!
 @brief ハイスコアをファイルから読み込む

 ハイスコアをファイルから読み込む。
 @return ハイスコア
 */
int SettingFileIO::ReadHighScore()
{
    // UserDefaultインスタンスを取得する
    UserDefault *ud = UserDefault::getInstance();
    
    // ハイスコアを取得する
    int hiScore = ud->getIntegerForKey(UDKeyHighScore, 0);
    
    return hiScore;
}