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
 @file AKNWayAngle.m
 @brief n-way弾角度計算クラス定義
 
 n-way弾角度の計算を行うクラスを定義する。
 */

#include "AKNWayAngle.h"

using cocos2d::Vector2;

/*!
 @brief 2点間指定によるn-way角度計算
 
 2点の座標からn-way弾の各弾の角度を計算する。
 @param src 始点
 @param dest 終点
 @param count 弾数
 @param interval 弾の間の角度
 */
AKNWayAngle::AKNWayAngle(const Vector2 &src, const Vector2 &dest, int count, float interval)
{
    // 中心の弾の角度を計算する
    float centerAngle = AKAngle::calcDestAngle(src, dest);

    // 各弾の角度を計算する
    calcAngles(centerAngle, count, interval);
}

/*!
 @brief 中心角度指定によるn-way角度計算
 
 中心の弾の角度からn-way弾の各弾の角度を計算する。
 @param center 中心角度
 @param count 弾数
 @param interval 弾の間の角度
 @return 初期化したインスタンス
 */
AKNWayAngle::AKNWayAngle(float center, int count, float interval)
{
    // 各弾の角度を計算する
    calcAngles(center, count, interval);
}

/*!
 @brief 弾の角度取得
 
 弾の角度を取得する。
 @return 弾の角度
 */
const std::vector<float>* AKNWayAngle::getAngles()
{
    return &m_angles;
}

/*!
 @brief 1個目の弾の角度
 
 1個目の弾の角度を取得する。
 @return 1個目の弾の角度
 */
float AKNWayAngle::getTopAngle()
{
    return m_angles.at(0);
}

/*!
 @brief 各弾の角度計算
 
 各弾の角度を計算し、メンバ変数に格納する。
 @param center 中心角度
 @param count 弾数
 @param interval 弾の間の角度
 */
void AKNWayAngle::calcAngles(float center, int count, float interval)
{
    // 最小値の角度を計算する
    float minAngle = center - (interval * (count - 1)) / 2.0f;
    
    // 各弾の発射角度を計算する
    for (int i = 0; i < count; i++) {
        
        // 弾の角度を計算する
        float angle = minAngle + i * interval;
        
        // 配列に追加する
        m_angles.push_back(angle);
    }
}

