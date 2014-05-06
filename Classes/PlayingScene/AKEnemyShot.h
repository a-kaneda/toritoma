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
 @file AKEnemyShot.h
 @brief 敵弾クラス定義
 
 敵の発射する弾のクラスを定義する。
 */

#ifndef AKENEMYSHOT_H
#define AKENEMYSHOT_H

#include "AKCharacter.h"

// 敵の発射する弾のクラス
class AKEnemyShot : public AKCharacter {
private:
    /// 動作処理関数
    typedef void (AKEnemyShot::*ACTION_FUNC)(AKPlayDataInterface *data);
        
    /// 敵弾種別定義
    struct AKEnemyShotDef {
        AKEnemyShot::ACTION_FUNC action; ///< 動作処理
        int image;          ///< 画像ID
        int hitWidth;       ///< 当たり判定の幅
        int hitHeight;      ///< 当たり判定の高さ
        int grazePoint;     ///< かすりポイント
    };
    
    // 敵弾の種類
    enum AKEnemyShotType {
        kAKEnemyShotTypeNormal = 0,     ///< 標準弾
        kAKEnemyShotTypeChangeSpeed,    ///< 速度変更弾
        kAKEnemyShotTypeDefCount        ///< 敵弾の種類の数
    };
    
    // 敵弾の定義
    static const struct AKEnemyShotDef kAKEnemyShotDef[kAKEnemyShotTypeDefCount];
    
private:
    /// 動作開始からの経過フレーム数(各敵種別で使用)
    int m_frame;
    /// 動作状態(各敵種別で使用)
    int m_state;
    /// 動作処理
    ACTION_FUNC m_action;
    /// かすりポイント
    float m_grazePoint;
    /// 速度変更までの間隔
    int m_changeInterval;
    /// 変更後のx軸方向の速度
    float m_changeSpeedX;
    /// 変更後のy軸方向の速度
    float m_changeSpeedY;

public:
    // かすりポイント取得
    float getGrazePoint();
    // かすりポイント設定
    void setGrazePoint(float grazePoint);
    // 通常弾生成
    void createNormalShot(const cocos2d::Vector2 &position,
                          float angle,
                          float speed,
                          cocos2d::Node *parent);
    // スクロール影響弾生成
    void createScrollShot(const cocos2d::Vector2 &position,
                          float angle,
                          float speed,
                          cocos2d::Node *parent);
    // 速度変更弾生成
    void createChangeSpeedShot(const cocos2d::Vector2 &position,
                               float angle,
                               float speed,
                               int changeInterval,
                               float changeAngle,
                               float changeSpeed,
                               cocos2d::Node *parent);
    // 反射弾生成
    void createReflectShot(AKEnemyShot *base, cocos2d::Node *parent);

protected:
    // キャラクター固有の動作
    virtual void action(AKPlayDataInterface *data);
    
private:
    // 敵弾生成
    void createEnemyShot(int type,
                         const cocos2d::Vector2 &position,
                         float angle,
                         float speed,
                         cocos2d::Node *parent);
    // 種別から動作処理を判定
    ACTION_FUNC getActionOfType(int type);
    // 動作処理なし
    void actionNone(AKPlayDataInterface *data);
    // 速度変更
    void actionChangeSpeed(AKPlayDataInterface *data);
};

#endif
