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
 @file AKScreenSize.cpp
 @brief 画面サイズ管理クラス
 
 画面サイズ管理クラスを定義する。
 */

#include "AKScreenSize.h"
#include "AKLogNoDef.h"

using cocos2d::CCSize;
using cocos2d::CCDirector;
using cocos2d::CCPoint;
using cocos2d::CCRect;
using cocos2d::CCDirector;
using cocos2d::CCEGLView;

/// 解像度調整のベースサイズ
const CCSize AKScreenSize::kAKBaseSize(568, 320);
/// ゲーム画面のステージサイズ
const CCSize AKScreenSize::kAKStageSize(384, 288);
/// スクリーンサイズ
CCSize AKScreenSize::m_screenSize;
/// 画面に表示されている一番左下の座標
CCPoint AKScreenSize::m_screenOffset;

/*!
 @brief 解像度初期化処理

 解像度の調整とデバイスの解像度取得を行う。
 */
void AKScreenSize::init()
{
    // 解像度を調整する
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(kAKBaseSize.width,
                                                           kAKBaseSize.height,
                                                           kResolutionNoBorder); 

    // デバイスの解像度を取得する
    m_screenSize = CCDirector::sharedDirector()->getVisibleSize();

    // 左下の座標を計算する
    m_screenOffset.x = (kAKBaseSize.width - m_screenSize.width) / 2;
    m_screenOffset.y = (kAKBaseSize.height - m_screenSize.height) / 2;
}

/*!
 @brief 画面サイズ取得
 
 デバイスの画面サイズを取得する。
 @return 画面サイズ
 */
CCSize AKScreenSize::screenSize()
{
    return m_screenSize;
}

/*!
 @brief ステージサイズ取得
 
 ゲームステージのサイズを取得する。
 @return ステージサイズ
 */
CCSize AKScreenSize::stageSize()
{
    return kAKStageSize;
}

/*!
 @brief 中央座標取得
 
 画面の中央の座標を取得する。
 @return 中央座標
 */
CCPoint AKScreenSize::center()
{
    CCPoint center(kAKBaseSize.width / 2,
                   kAKBaseSize.height / 2);

    return center;
}

/*!
 @brief 左からの比率で座標取得
 
 左からの画面サイズの比率で距離を指定したときのデバイススクリーン座標を返す。
 @return デバイススクリーン座標
 */
int AKScreenSize::positionFromLeftRatio(float ratio)
{
    return AKScreenSize::screenSize().width * ratio + m_screenOffset.x;
}

/*!
 @brief 右からの比率で座標取得
 
 右からの画面サイズの比率で距離を指定したときのデバイススクリーン座標を返す。
 @return デバイススクリーン座標
 */
int AKScreenSize::positionFromRightRatio(float ratio)
{
    return AKScreenSize::screenSize().width * (1 - ratio) + m_screenOffset.x;
}

/*!
 @brief 上からの比率で座標取得
 
 上からの画面サイズの比率で距離を指定したときのデバイススクリーン座標を返す。
 @return デバイススクリーン座標
 */
int AKScreenSize::positionFromTopRatio(float ratio)
{
    return AKScreenSize::screenSize().height * (1 - ratio) + m_screenOffset.y;
}

/*!
 @brief 下からの比率で座標取得
 
 下からの画面サイズの比率で距離を指定したときのデバイススクリーン座標を返す。
 @return デバイススクリーン座標
 */
int AKScreenSize::positionFromBottomRatio(float ratio)
{
    return AKScreenSize::screenSize().height * ratio + m_screenOffset.y;
}

/*!
 @brief 左からの位置で座標取得
 
 左からの座標で距離を指定したときのデバイススクリーン座標を返す。
 iPadの場合は座標を倍にして処理する。
 @return デバイススクリーン座標
 */
int AKScreenSize::positionFromLeftPoint(float point)
{
    return point + m_screenOffset.x;
}

/*!
 @brief 右からの位置で座標取得
 
 右からの座標で距離を指定したときのデバイススクリーン座標を返す。
 iPadの場合は座標を倍にして処理する。
 @return デバイススクリーン座標
 */
int AKScreenSize::positionFromRightPoint(float point)
{
    return AKScreenSize::screenSize().width - point + m_screenOffset.x;
}

/*!
 @brief 上からの位置で座標取得
 
 上からの座標で距離を指定したときのデバイススクリーン座標を返す。
 iPadの場合は座標を倍にして処理する。
 @return デバイススクリーン座標
 */
int AKScreenSize::positionFromTopPoint(float point)
{
    return AKScreenSize::screenSize().height - point + m_screenOffset.y;
}

/*!
 @brief 下からの位置で座標取得
 
 下からの座標で距離を指定したときのデバイススクリーン座標を返す。
 iPadの場合は座標を倍にして処理する。
 @return デバイススクリーン座標
 */
int AKScreenSize::positionFromBottomPoint(float point)
{
    return point + m_screenOffset.y;
}

/*!
 @brief 中心からの横方向の位置で座標取得

 中心からの横方向の座標で距離を指定したときのデバイススクリーン座標を返す。
 iPadの場合は座標を倍にして処理する。
 @return デバイススクリーン座標
 */
int AKScreenSize::positionFromHorizontalCenterPoint(float point)
{
    return AKScreenSize::center().x + point;
}

/*!
 @brief 中心からの縦方向の位置で座標取得
 
 中心からの縦方向の座標で距離を指定したときのデバイススクリーン座標を返す。
 iPadの場合は座標を倍にして処理する。
 @return デバイススクリーン座標
 */
int AKScreenSize::positionFromVerticalCenterPoint(float point)
{
    return AKScreenSize::center().y + point;
}

/*!
 @brief ステージ座標x座標からデバイススクリーン座標の取得
 
 ステージ座標からデバイススクリーン座標へと変換する。x座標を取得する。
 @param stageX ステージ座標x座標
 @return デバイススクリーン座標
 */
int AKScreenSize::xOfStage(float stageX)
{
    return stageX +
        (AKScreenSize::screenSize().width - AKScreenSize::stageSize().width) / 2 +
        m_screenOffset.x;
}

/*!
 @brief ステージ座標y座標からデバイススクリーン座標の取得
 
 ステージ座標からデバイススクリーン座標へと変換する。y座標を取得する。
 @param stageY ステージ座標y座標
 @return デバイススクリーン座標
 */
int AKScreenSize::yOfStage(float stageY)
{
    return stageY +
        (AKScreenSize::screenSize().height - AKScreenSize::stageSize().height) +
        m_screenOffset.y;
}

/*!
 @brief デバイススクリーン座標x座標からステージ座標の取得
 
 デバイススクリーン座標からステージ座標へ変換する。x座標を取得する
 @param deviceX デバイススクリーン座標x座標
 @return ステージ座標
 */
float AKScreenSize::xOfDevice(float deviceX)
{
    return deviceX -
        (AKScreenSize::screenSize().width - AKScreenSize::stageSize().width) / 2 -
        m_screenOffset.x;
}

/*!
 @brief デバイススクリーン座標y座標からステージ座標の取得
 
 デバイススクリーン座標からステージ座標へ変換する。y座標を取得する
 @param deviceY デバイススクリーン座標x座標
 @return ステージ座標
 */
float AKScreenSize::yOfDevice(float deviceY)
{
    return deviceY -
        (AKScreenSize::screenSize().height - AKScreenSize::stageSize().height) - 
        m_screenOffset.y;
}

/*! 
 @brief 矩形のデバイス補正、x座標、y座標、幅、高さ指定
 
 矩形の座標とサイズをデバイスに合わせて補正する。
 x座標、y座標、幅、高さから矩形を作成する。
 @param x x座標
 @param y y座標
 @param w 幅
 @param h 高さ
 @return 補正した矩形
 */
CCRect AKScreenSize::deviceRect(float x, float y, float w, float h)
{
    // 矩形を作成する
    CCRect rect(x, y, w, h);
    
    // 作成した矩形を返す
    return rect;
}

/*!
 @brief 矩形のデバイス補正、座標、サイズ指定
 
 矩形の座標とサイズをデバイスに合わせて補正する。
 座標、サイズから矩形を作成する。
 @param point 座標
 @param size サイズ
 @return 補正した矩形
 */
CCRect AKScreenSize::deviceRect(const CCPoint &point, const CCSize &size)
{
    return AKScreenSize::deviceRect(point.x, point.y, size.width, size.height);
}

/*!
 @brief 矩形のデバイス補正、矩形指定
 
 矩形の座標とサイズをデバイスに合わせて補正する。
 矩形から矩形を作成する。
 @param rect 矩形
 @return 補正した矩形
 */
CCRect AKScreenSize::deviceRect(const CCRect &rect)
{
    return AKScreenSize::deviceRect(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
}

/*!
 @brief 中心座標とサイズから矩形を作成する
 
 中心座標とサイズから矩形を作成する。
 @param center 中心座標
 @param size サイズ
 @return 矩形
 */
CCRect AKScreenSize::makeRectFromCenter(const CCPoint &center, int size)
{
    CCRect rect(center.x - size / 2,
                center.y - size / 2,
                size,
                size);

    return rect;
}

/*!
 @brief 長さのデバイス補正
 
 長さをデバイスに応じて補正する。
 @param len プログラム上の長さ
 @return デバイスごとに対応した長さ
 */
float AKScreenSize::deviceLength(float len)
{
    return len;
}