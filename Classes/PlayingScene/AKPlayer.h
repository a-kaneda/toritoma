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
 @file AKPlayer.h
 @brief 自機クラス定義
 
 自機を管理するクラスを定義する。
 */

#ifndef AKPLAYER_H
#define AKPLAYER_H

#include "AKCharacter.h"
#include "AKOption.h"

/*!
 @brief 自機クラス

 自機を管理する。
 */
class AKPlayer : public AKCharacter {
private:
    /// 無敵状態かどうか
    bool m_isInvincible;
    /// 無敵状態の残りフレーム数
    int m_invincivleFrame;
    /// 弾発射までの残りフレーム数
    int m_shootFrame;
    /// チキンゲージ
    int m_chickenGauge;
    /// オプション
    AKOption *m_option;

private:
    // デフォルトコンストラクタは使用禁止にする
    AKPlayer();

public:
    // 親ノードを指定したコンストラクタ
    AKPlayer(cocos2d::Node *parent, cocos2d::Node *optionParent);
    // デストラクタ
    ~AKPlayer();
    // 復活
    void rebirth(int stageNo);
    // 初期化
    void reset();
    // かすり判定
    void graze(std::vector<AKEnemyShot*> &characters);
    // 移動座標設定
    void setPosition(const cocos2d::Vec2 &position, bool hold, AKPlayDataInterface *data);
    // オプション数更新
    void updateOptionCount();
    // シールド有無設定
    void setShield(bool shield);
    // オプション取得
    AKOption* getOption();
    // 無敵状態かどうか取得
    bool isInvincible();
    // チキンゲージ取得
    int getChickenGauge();
    // チキンゲージ設定
    void setChickenGauge(int chickenGauge);
    // チキンゲージ%単位での習得
    int getChickenGaugePercent();

protected:
    // キャラクター固有の動作
    virtual void action(AKPlayDataInterface *data);
    // 破壊処理
    virtual void destroy(AKPlayDataInterface *data);
};

#endif
