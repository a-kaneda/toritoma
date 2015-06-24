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
 @file AKNWayAngle.h
 @brief n-way弾角度計算クラス定義
 
 n-way弾角度の計算を行うクラスを定義する。
 */

#ifndef AKNWAYANGLE_H
#define AKNWAYANGLE_H

#include "AKToritoma.h"

/*!
 @brief n-way弾角度計算クラス
 
 n-way弾を作成する際の角度を計算するクラス。
 */
class AKNWayAngle {
private:
    /// 弾の角度
    std::vector<float> m_angles;
private:
    // デフォルトコンストラクタは使用禁止にする
    AKNWayAngle();
public:
    // 2点間指定によるn-way角度計算
    AKNWayAngle(const cocos2d::Vec2 &src, const cocos2d::Vec2 &dest, int count, float interval);
    // 中心角度指定によるn-way角度計算
    AKNWayAngle(float center, int count, float interval);
    // 弾の角度取得
    const std::vector<float>* getAngles();
    // 1個目の弾の角度取得
    float getTopAngle();
private:
    // 各弾の角度計算
    void calcAngles(float center, int count, float interval);
};

#endif