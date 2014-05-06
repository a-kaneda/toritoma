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
 @file AKEffect.h
 @brief 画面効果クラス定義
 
 爆発等の画面効果を生成するクラスを定義する。
 */

#ifndef AKEFFECT_H
#define AKEFFECT_H

#include "AKCharacter.h"

/// 画面効果定義
struct AKEffectDef {
    int fileNo;             ///< ファイル名の番号
    int width;              ///< 幅
    int height;             ///< 高さ
    int speedX;             ///< x方向の速度
    int speedY;             ///< y方向の速度
    int lifeFrame;          ///< 生存フレーム数
    int animationFrame;     ///< アニメーションフレーム数
    int animationInterval;  ///< アニメーション更新間隔
    int animationRepeat;    ///< アニメーション繰り返し回数
};

/*!
 @brief 画面効果クラス
 
 爆発等の画面効果を生成する。
 */
class AKEffect : public AKCharacter {
private:
    /// 生存フレーム数
    int m_lifeFrame;
protected:
    // キャラクター固有の動作
    virtual void action(AKPlayDataInterface *data);
public:
    // 画面効果開始
    void createEffect(int type, const cocos2d::Vector2 &position, cocos2d::Node *parent);
};

#endif
