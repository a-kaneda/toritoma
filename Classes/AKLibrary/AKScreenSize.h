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
 @file AKScreenSize.h
 @brief 画面サイズ管理クラス
 
 画面サイズ管理クラスを定義する。
 */

#ifndef AKSCREENSIZE_H
#define AKSCREENSIZE_H

#include "AKCommon.h"

/*!
 @brief 画面サイズ管理クラス
 
 画面サイズを管理する。
 */
class AKScreenSize {
private:
    // 解像度調整のベースサイズ
    static const cocos2d::Size kAKBaseSize;
    // ゲーム画面のステージサイズ
    static const cocos2d::Size kAKStageSize;
    // スクリーンサイズ
    static cocos2d::Size m_screenSize;
    // 画面に表示されている一番左下の座標
    static cocos2d::Vector2 m_screenOffset;
public:
    // 解像度初期化処理
    static void init(cocos2d::GLView *view);
    // 画面サイズ取得
    static cocos2d::Size screenSize();
    // ステージサイズ取得
    static cocos2d::Size stageSize();
    // 中央座標取得
    static cocos2d::Vector2 center();
    // 左からの比率で座標取得
    static int positionFromLeftRatio(float ratio);
    // 右からの比率で座標取得
    static int positionFromRightRatio(float ratio);
    // 上からの比率で座標取得
    static int positionFromTopRatio(float ratio);
    // 下からの比率で座標取得
    static int positionFromBottomRatio(float ratio);
    // 左からの位置で座標取得
    static int positionFromLeftPoint(float point);
    // 右からの位置で座標取得
    static int positionFromRightPoint(float point);
    // 上からの位置で座標取得
    static int positionFromTopPoint(float point);
    // 下からの位置で座標取得
    static int positionFromBottomPoint(float point);
    // 中心からの横方向の位置で座標取得
    static int positionFromHorizontalCenterPoint(float point);
    // 中心からの縦方向の位置で座標取得
    static int positionFromVerticalCenterPoint(float point);
    // ステージ座標x座標からデバイススクリーン座標の取得
    static int xOfStage(float stageX);
    // ステージ座標y座標からデバイススクリーン座標の取得
    static int yOfStage(float stageY);
    // デバイススクリーン座標x座標からステージ座標の取得
    static float xOfDevice(float deviceX);
    // デバイススクリーン座標y座標からステージ座標の取得
    static float yOfDevice(float deviceY);
    // 矩形のデバイス補正、x座標、y座標、幅、高さ指定
    static cocos2d::Rect deviceRect(float x, float y, float w, float h);
    // 句形のデバイス補正、座標、サイズ指定
    static cocos2d::Rect deviceRect(const cocos2d::Vector2 &point, const cocos2d::Size &size);
    // 句形のデバイス補正、矩形指定
    static cocos2d::Rect deviceRect(const cocos2d::Rect &rect);
    // 中心座標とサイズから矩形を作成する
    static cocos2d::Rect makeRectFromCenter(const cocos2d::Vector2 &center, int size);
    // 長さのデバイス補正
    static float deviceLength(float len);
};

#endif
