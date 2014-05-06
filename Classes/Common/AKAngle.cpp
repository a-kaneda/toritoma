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
 @file AKAngle.cpp
 @brief 角度計算クラス定義
 
 角度の計算を行うクラスを定義する。
 */

#include "AKAngle.h"

using cocos2d::Vector2;

/*!
 @brief radianからdegreeへの角度変換
 
 radianからdegreeへ変換する。
 @param radAngle rad角度
 @return deg角度
 */
float AKAngle::convertAngleRad2Deg(float radAngle)
{
    // radianからdegreeへ変換する
    return radAngle / (2 * M_PI) * 360;
}

/*!
 @brief degreeからradianへの角度変換
 
 degreeからradianへ変換する。
 @param degAngle deg角度
 @return rad角度
 */
float AKAngle::convertAngleDeg2Rad(float degAngle)
{
    // degreeからradianへ変換する
    return degAngle / 360 * (2 * M_PI);
}

/*!
 @brief radianからスクリーン角度への変換
 
 radianからdegreeへ変換し、上向きを0°とする。時計回りを正とする。
 @param radAngle rad角度
 @return スクリーン角度
 */
float AKAngle::convertAngleRad2Scr(float radAngle)
{
    // radianからdegreeへ変換する
    float screenAngle = AKAngle::convertAngleRad2Deg(radAngle);
    
    // 上向きを0°とするため、90°ずらす。
    screenAngle -= 90;
    
    // 時計回りを正とするため符号を反転する。
    screenAngle *= -1;
    
    return screenAngle;
}

/*!
 @brief スクリーン角度からradianへの変換
 
 スクリーンの角度からradianへ変換する。
 上向き0°、時計回りが正、degreeとなっているものを左向き0°、反時計回りが正、radianへと変換する。
 @param screenAngle スクリーン角度
 @return rad角度
 */
float AKAngle::convertAngleScr2Rad(float screenAngle)
{
    // 時計回りを正となっているものを逆向きにするため、符号を反転する
    screenAngle *= -1;
    
    // 上向きを0°となっているため、90°ずらす
    screenAngle += 90;
    
    // degreeからradianへ変換する
    float radAngle = AKAngle::convertAngleDeg2Rad(screenAngle);
    
    return radAngle;
}

/*!
 @brief 2点間の角度計算
 
 2点間を線で結んだときの角度を計算する。
 @param srcx 始点
 @param dest 終点
 @return 2点間の角度
 */
float AKAngle::calcDestAngle(const Vector2 &src, const Vector2 &dest)
{
    // x方向のベクトルの大きさを計算する
    float vx = dest.x - src.x;
    
    // y方向のベクトルの大きさを計算する
    float vy = dest.y - src.y;
    
    // 角度を計算する
    float angle = atan2f(vy, vx);
    
    return angle;
}

/*!
 @brief 回転方向の計算
 
 現在の角度から見て、対象の角度が時計回りの側にあるか反時計回りの側にあるかを計算する。
 @param srcAngle 現在の角度
 @param destAngle 計算対象の角度
 @return 1:反時計回り、-1:時計回り、0:直進
 */
int AKAngle::calcRotationDirection(float srcAngle, float destAngle)
{
    // 現在の角度から見て入力角度が時計回りの側か反時計回りの側か調べる
    // sin(目的角度 - 現在の角度) > 0の場合は反時計回り
    // sin(目的角度 - 現在の角度) < 0の場合は時計回り
    float destsin = sin(destAngle - srcAngle);
    
    // 回転方向を設定する
    int rotdirect = 0;
    if (destsin > 0.0f) {
        rotdirect = 1;
    }
    else if (destsin < 0.0f) {
        rotdirect = -1;
    }
    else {
        // 上記判定でこのelseに入るのは入力角度が同じ向きか反対向きのときだけ
        // 同じ向きか反対向きか調べる
        // cos(入力角度 - 現在角度) < 0の場合は反対向き
        // 反対向きの場合は反時計回りとする
        float destcos = cos(destAngle - srcAngle);
        if (destcos < 0.0f) {
            rotdirect = 1;
        }
        else {
            rotdirect = 0;
        }
    }
    
    return rotdirect;
}

/*!
 @brief +πから-πの間に正規化する
 
 角度を+πから-πの間に正規化する。
 @return 正規化した角度
 */
float AKAngle::normalize(float angle)
{
    // -πより小さい間はプラス方向に回転させる
    while (angle < -M_PI) {
        angle += 2 * M_PI;
    }
    
    // +πより大きい間はマイナス方向に回転させる
    while (angle > M_PI) {
        angle -= 2 * M_PI;
    }
    
    return angle;
}
