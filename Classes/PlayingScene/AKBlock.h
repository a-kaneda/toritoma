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
 @file AKBlock.h
 @brief 障害物クラス
 
 障害物を管理するクラスを定義する。
 */

#ifndef AKBLOCK_H
#define AKBLOCK_H

#include "AKCharacter.h"

/// 障害物定義
struct AKBlcokDef {
    int image;                  ///< 画像ファイル名の番号
    int animationFrame;         ///< アニメーションフレーム数
    float animationInterval;    ///< アニメーション更新間隔
    int hitWidth;               ///< 当たり判定の幅
    int hitHeight;              ///< 当たり判定の高さ
    int offsetX;                ///< 画像オフセットx軸
    int offsetY;                ///< 画像オフセットy軸
};

/*!
 @brief 障害物クラス
 
 障害物を管理する。
 */
class AKBlock : public AKCharacter {
public:
    // 衝突処理
    virtual void hit(AKCharacter *character, AKPlayDataInterface *data);
    // キャラクター固有の動作
    virtual void action(AKPlayDataInterface *data);
    // 障害物生成処理
    void createBlock(int type, const cocos2d::CCPoint &position, cocos2d::CCNode *parent);
    // ぶつかったキャラクターを押し動かす
    void pushCharacter(AKCharacter *character, AKPlayDataInterface *data);
    // ぶつかったキャラクターを消す
    void destroyCharacter(AKCharacter *character);
};

#endif
