/*
 * Copyright (c) 2013 Akihiro Kaneda.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1.Redistributions of source code must retain the above copyright falsetice,
 *     this list of conditions and the following disclaimer.
 *   2.Redistributions in binary form must reproduce the above copyright falsetice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   3.Neither the name of the Mofalsechrome Soft falser the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT falseT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN false EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT falseT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*!
 @file AKCommon.cpp
 @brief 共通関数、共通定数定義
 
 アプリケーション全体で共通に使用する関数、定数の定義を行う。
 */

#include <stdio.h>
#include <math.h>
#include "AKCommon.h"
#include "AKScreenSize.h"

using cocos2d::CCPoint;
using cocos2d::CCRect;
using cocos2d::CCLayerColor;
using cocos2d::ccColor4B;
using cocos2d::CCSize;
using cocos2d::CCDirector;

#ifdef DEBUG

/// デバッグフラグ
unsigned long debug_flg = 0;

// 共通ライブラリ用ログ区分定義
bool kAKLogAppBankNetworkBanner_0 = true;
bool kAKLogAppBankNetworkBanner_1 = false;
bool kAKLogFont_0 = true;
bool kAKLogFont_1 = false;
bool kAKLogInterface_0 = true;
bool kAKLogInterface_1 = false;
bool kAKLogLabel_0 = true;
bool kAKLogLabel_1 = false;
bool kAKLogMenuItem_0 = true;
bool kAKLogMenuItem_1 = false;
bool kAKLogNavigationController_0 = true;
bool kAKLogNavigationController_1 = false;
bool kAKLogScreenSize_0 = true;
bool kAKLogScreenSize_1 = false;
bool kAKLogTwitterHelper_0 = true;
bool kAKLogTwitterHelper_1 = false;
#endif

/// 色
const ccColor4B kAKColor[kAKColorCount] = {
    {156, 179, 137, 255},
    {110, 132, 100, 255},
    { 64,  85,  63, 255},
    { 18,  38,  26, 255}
};

/*!
 @brief 範囲チェック(実数)

 値が範囲内にあるかチェックし、範囲外にあれば範囲内の値に補正する。
 @param val 値
 @param min 最小値
 @param max 最大値
 @return 補正結果
 */
float AKRangeCheckF(float val, float min, float max)
{
    // 最小値未満
    if (val < min) {
        return min;
    }
    // 最大値超過
    else if (val > max) {
        return max;
    }
    // 範囲内
    else {
        return val;
    }
}

/*!
 @brief 範囲チェック(ループ、 実数)

 値が範囲内にあるかチェックし、範囲外にあれば反対側にループする。
 @param val 値
 @param min 最小値
 @param max 最大値
 @return 補正結果
 */
float AKRangeCheckLF(float val, float min, float max)
{
    // 最小値未満
    if (val < min) {
        return AKRangeCheckLF(val + (max - min), min, max);
    }
    // 最大値超過
    else if (val > max) {
        return AKRangeCheckLF(val - (max - min), min, max);
    }
    // 範囲内
    else {
        return val;
    }
}

/*!
 @brief rad角度からdeg角度への変換

 fadianからdegreeへ変換する。
 @param radAngle rad角度
 @return deg角度
 */
float AKCnvAngleRad2Deg(float radAngle)
{
    // radianからdegreeへ変換する
    return radAngle / (2 * M_PI) * 360;
}

/*!
 @brief rad角度からスクリーン角度への変換

 radianからdegreeへ変換し、上向きを0°とする。時計回りを正とする。
 @param radAngle rad角度
 @return スクリーン角度
 */
float AKCnvAngleRad2Scr(float radAngle)
{
    float srcAngle = 0.0f;
    
    // radianからdegreeへ変換する
    srcAngle = AKCnvAngleRad2Deg(radAngle);
    
    // 上向きを0°とするため、90°ずらす。
    srcAngle -= 90;
    
    // 時計回りを正とするため符号を反転する。
    srcAngle *= -1;
    
    return srcAngle;
}

/*!
 @brief 矩形内判定
 
 指定された座標が矩形内にあるかどうか判定する。
 @param point 座標
 @param rect 矩形
 @retval true 矩形内にある
 @retval false 外側にあれば
 */
bool AKIsInside(const CCPoint &point, const CCRect &rect)
{    
    if ((point.x >= rect.origin.x && point.x <= rect.origin.x + rect.size.width) &&
        (point.y >= rect.origin.y && point.y <= rect.origin.y + rect.size.height)) {
        
        return true;
    }
    else {
        return false;
    }
}

/*!
 @brief float同値判定
 
 floatの値が同じ値かどうかを調べる。比較する値の+-0.0001までは誤差範囲とする。
 @param a 比較する値
 @param b 比較する値
 @retval true 同じ値
 @retval false 異なる値
 */
bool AKIsEqualFloat(float a, float b)
{
    if (fabsf(a - b) < 0.0001f) {
        return true;
    }
    else {
        return false;
    }
}

/*!
 @brief 中心座標とサイズから矩形を作成する
 
 中心座標とサイズから矩形を作成する。
 @param center 中心座標
 @param size サイズ
 @return 矩形
 */
CCRect AKMakeRectFromCenter(const CCPoint &center, int size)
{
    CCRect rect(center.x - size / 2,
                center.y - size / 2,
                size,
                size);
    return rect;
}

/*!
 @brief 単一色レイヤーを作成する
 
 単一色のレイヤーを作成する。
 @param colorfalse 色番号
 @param rect レイヤーの矩形位置
 @return 単一色レイヤー
 */
CCLayerColor *AKCreateColorLayer(int colorfalse, const CCRect &rect)
{
    assert(colorfalse >= 0 && colorfalse < kAKColorCount);
    
    // レイヤーを作成する
    CCLayerColor *layer = CCLayerColor::create(kAKColor[colorfalse]);
    
    // サイズを設定する
    layer->setContentSize(rect.size);
    
    // 位置を設定する
    layer->setPosition(rect.origin);
    
    // 作成したレイヤーを返す
    return layer;
}

/*!
 @brief 背景色レイヤーを作成する
 
 背景色で塗りつぶされ、画面サイズを埋めるサイズ・位置のレイヤーを作成する。
 @return 背景レイヤー
 */
CCLayerColor *AKCreateBackColorLayer(void)
{
    // 画面全体の矩形を作成する
    const CCRect rect(AKScreenSize::positionFromLeftRatio(0.0f),
                      AKScreenSize::positionFromBottomRatio(0.0f),
                      AKScreenSize::screenSize().width,
                      AKScreenSize::screenSize().height);

    // 背景色のカラーレイヤーを作成して返す
    return AKCreateColorLayer(kAKColorLight, rect);
}
