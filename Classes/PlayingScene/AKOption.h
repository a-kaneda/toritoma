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
 @file AKOption.h
 @brief オプションクラス
 
 オプションを管理するクラスを定義する。
 */

#ifndef AKOPTION_H
#define AKOPTION_H

#include "AKCharacter.h"

/*!
 @brief オプションクラス
 
 オプションを管理するクラス。
 */
class AKOption : public AKCharacter {
private:
    /// 移動座標
    std::vector<cocos2d::Vec2> m_movePositions;
    /// 弾発射までの残りフレーム数
    int m_shootFrame;
    /// 次のオプション
    AKOption *m_next;
    /// シールド有無
    bool m_shield;
    
private:
    // デフォルトコンストラクタは使用禁止にする
    AKOption();
    
public:
    // オプション個数と親ノードを指定したコンストラクタ
    AKOption(int count, cocos2d::Node *parent);
    // デストラクタ
    ~AKOption();
    // シールド有無設定
    void setShiled(bool shield);
    // オプション数設定
    void setOptionCount(int count, const cocos2d::Vec2 &position);
    // 移動処理
    void movePosition(const cocos2d::Vec2 &position, const cocos2d::Vec2 &distance);
    // 移動座標設定
    void setPosition(const cocos2d::Vec2 &position);
    // オプション移動
    void optionMove(AKPlayDataInterface *data);
    // 次のオプション取得
    AKOption* getNext();

protected:
    // キャラクター固有の動作
    virtual void action(AKPlayDataInterface *data);
    // 衝突処理
    virtual void hit(AKCharacter *character, AKPlayDataInterface *data);
};

#endif
