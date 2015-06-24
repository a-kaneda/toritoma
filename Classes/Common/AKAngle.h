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
 @file AKAngle.h
 @brief 角度計算クラス定義
 
 角度の計算を行うクラスを定義する。
 */

#ifndef AKANGLE_H
#define AKANGLE_H

#include "AKCommon.h"

/*!
 @brief 角度計算クラス
 
 角度の計算するクラス。
 */
class AKAngle {
public:
    // radianからdegreeへの角度変換
    static float convertAngleRad2Deg(float radAngle);
    // degreeからradianへの角度変換
    static float convertAngleDeg2Rad(float degAngle);
    // radianからスクリーン角度への変換
    static float convertAngleRad2Scr(float radAngle);
    // スクリーン角度からradianへの変換
    static float convertAngleScr2Rad(float screenAngle);
    // 2点間の角度計算
    static float calcDestAngle(const cocos2d::Vec2 &src, const cocos2d::Vec2 &dest);
    // 回転方向の計算
    static int calcRotationDirection(float srcAngle, float destAngle);
    // +πから-πの間に正規化する
    static float normalize(float angle);
};

#endif
