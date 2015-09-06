/*
 * Copyright (c) 2013 Akihiro Kaneda.
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
 @file AKCommon.h
 @brief 共通関数、共通定数定義
 
 アプリケーション全体で共通に使用する関数、定数の定義を行う。
 */

#ifndef AKCOMMON_H
#define AKCOMMON_H

//#include <Foundation/Foundation.h>
//#include <GameKit/GameKit.h>
#include "cocos2d.h"

/// 色番号
enum kAKColorNumber {
    kAKColorLight = 0,      ///< 明るい色
    kAKColorLittleLight,    ///< やや明るい色
    kAKColorLittleDark,     ///< やや暗い色
    kAKColorDark,           ///< 暗い色
    kAKColorCount           ///< 色種類数
};

// 色
extern const cocos2d::Color4B kAKColor[kAKColorCount];

#ifdef DEBUG

/// デバッグフラグ
extern unsigned long debug_flg;

// ライブラリ用ログ区分
extern bool kAKLogAppBankNetworkBanner_0;
extern bool kAKLogAppBankNetworkBanner_1;
extern bool kAKLogFont_0;
extern bool kAKLogFont_1;
extern bool kAKLogInterface_0;
extern bool kAKLogInterface_1;
extern bool kAKLogLabel_0;
extern bool kAKLogLabel_1;
extern bool kAKLogMenuItem_0;
extern bool kAKLogMenuItem_1;
extern bool kAKLogNavigationController_0;
extern bool kAKLogNavigationController_1;
extern bool kAKLogScreenSize_0;
extern bool kAKLogScreenSize_1;
extern bool kAKLogTwitterHelper_0;
extern bool kAKLogTwitterHelper_1;

/*!
 @brief デバッグフラグ取得

 デバッグ用のフラグを取得する。ログの出力条件に使用する。
 @return フラグ値
 */
#define AKGetDebugFlg() (debug_flg)

/*!
 @brief デバッグフラグ設定
 
 デバッグ用のフラグを設定する。ログの出力条件に使用する。
 @param flg フラグ値
 */
#define AKSetDebugFlg(flg) (debug_flg = (flg))

/*!
 @brief デバッグログ
 
 デバッグログ。出力条件の指定が可能。ログの先頭にメソッド名と行数を付加する。
 @param cond 出力条件
 @param fmt 出力フォーマット
 */
#define AKLog(cond, fmt, ...) if (cond) cocos2d::log("%s(%d) " fmt, __FILE__, __LINE__, ## __VA_ARGS__)

/*!
 @brief アサート
 
 デバッグログを出力し、アサートを出力する。
 @param cond 出力条件
 @param fmt 出力フォーマット
 */
#define AKAssert(cond, fmt, ...) if (!(cond)) {cocos2d::log("%s(%d) " fmt, __FILE__, __LINE__, ## __VA_ARGS__); assert(0);}

#else

/*!
 @brief デバッグフラグ取得
 
 デバッグ用のフラグを取得する。ログの出力条件に使用する。
 @return フラグ値
 */
#define AKGetDebugFlg()
/*!
 @brief デバッグフラグ設定
 
 デバッグ用のフラグを設定する。ログの出力条件に使用する。
 @param flg フラグ値
 */
#define AKSetDebugFlg(flg)

/*!
 @brief デバッグログ
 
 デバッグログ。出力条件の指定が可能。ログの先頭にメソッド名と行数を付加する。
 @param cond 出力条件
 @param fmt 出力フォーマット
 */
#define AKLog(cond, fmt, ...)

/*!
 @brief アサート
 
 デバッグログを出力し、アサートを出力する。
 @param cond 出力条件
 @param fmt 出力フォーマット
 */
#define AKAssert(cond, fmt, ...)

#endif

// 日本語フォント
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define JAPANESE_FONT "fonts/NotoSansCJKjp-Regular.ttf"
#else
#define JAPANESE_FONT "NotoSansCJKjp-Regular.ttf"
#endif

// コントロール用フォント
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define CONTROL_FONT "fonts/misaki_gothic.ttf"
#else
#define CONTROL_FONT "misaki_gothic.ttf"
#endif

// 範囲チェック
float AKRangeCheckLF(float val, float min, float max);
float AKRangeCheckF(float val, float min, float max);

// 矩形内判定
bool AKIsInside(const cocos2d::Vec2 &point, const cocos2d::Rect &rect);

// float同値判定
bool AKIsEqualFloat(float a, float b);

// 中心座標とサイズから矩形を作成する
cocos2d::Rect AKMakeRectFromCenter(const cocos2d::Vec2 &center, int size);

// 単一色レイヤーを作成する
cocos2d::LayerColor *AKCreateColorLayer(int colorNo, const cocos2d::Rect &rect);

// 背景色レイヤーを作成する
cocos2d::LayerColor *AKCreateBackColorLayer(void);

// 英語の序数詞を作成する
std::string MakeOrdinal(int number);

#endif
