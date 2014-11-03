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
 @file AKPlayerShot.m
 @brief 自機弾クラス定義
 
 自機の発射する弾のクラスを定義する。
 */

#include "AKPlayerShot.h"

using cocos2d::Vector2;
using cocos2d::Node;

/// 自機弾のスピード
static const float kAKPlayerShotSpeed = 5.0f;
/// 自機弾の画像名
static const char *kAKPlayerShotImage = "PlayerShot_01";
/// 自機弾の幅
static const int kAKPlayerShotWidth = 6;
/// 自機弾の高さ
static const int kAKPlayerShotHeight = 6;
/// 自機弾の画面外判定しきい値
static const int kAKPlayerShotOutThreshold = 3;
/// 自機弾の攻撃力
static const int kAKPlayerShotPower = 4;
/// オプション弾の攻撃力
static const int kAKOptionShotPower = 1;

/*!
 @brief 自機弾生成
 
 自機の弾を生成する。
 @param position 生成位置
 @param parent 配置する親ノード
 */
void AKPlayerShot::createPlayerShot(const Vector2 &position, float angle, Node *parent)
{
    // 攻撃力を設定する
    m_power = kAKPlayerShotPower;

    // スピードをxとyに分割して設定する
    m_speedX = cos(angle) * kAKPlayerShotSpeed;
    m_speedY = sin(angle) * kAKPlayerShotSpeed;
    
    // その他のパラメータを設定する
    setCommonParam(position, parent);
}

/*!
 @brief 自機弾(上向き)生成
 
 上向きに移動する自機の弾を生成する。
 @param position 生成位置
 @param parent 配置する親ノード
 */
void AKPlayerShot::createPlayerShotUpward(const Vector2 &position, Node *parent)
{
    // 攻撃力を設定する
    m_power = kAKPlayerShotPower;

    // スピードを設定する。右上方向へまっすぐに進む。
    m_speedX = kAKPlayerShotSpeed;
    m_speedY = -kAKPlayerShotSpeed / 2.0f;
    
    // その他のパラメータを設定する
    setCommonParam(position, parent);
}

/*!
 @brief 自機弾(下向き)生成
 
 下向きに移動する自機の弾を生成する。
 @param position 生成位置
 @param parent 配置する親ノード
 */
void AKPlayerShot::createPlayerShotDownward(const Vector2 &position, Node *parent)
{
    // 攻撃力を設定する
    m_power = kAKPlayerShotPower;

    // スピードを設定する。右下方向へまっすぐに進む。
    m_speedX = kAKPlayerShotSpeed;
    m_speedY = kAKPlayerShotSpeed / 2.0f;
    
    // その他のパラメータを設定する
    setCommonParam(position, parent);
}

/*!
 @brief オプション弾生成
 
 オプションの弾を生成する。
 @param position 生成位置
 @param parent 配置する親ノード
 */
void AKPlayerShot::createOptionShot(const Vector2 &position, Node *parent)
{
    // 攻撃力を設定する
    m_power = kAKOptionShotPower;

    // スピードを設定する。右方向へまっすぐに進む。
    m_speedX = kAKPlayerShotSpeed;
    m_speedY = 0.0f;
    
    // その他のパラメータを設定する
    setCommonParam(position, parent);
}

/*!
 @brief オプション弾(上向き)生成
 
 上向きに移動するオプションの弾を生成する。
 @param position 生成位置
 @param parent 配置する親ノード
 */
void AKPlayerShot::createOptionShotUpward(const Vector2 &position, Node *parent)
{
    // 攻撃力を設定する
    m_power = kAKOptionShotPower;

    // スピードを設定する。右上方向へまっすぐに進む。
    m_speedX = kAKPlayerShotSpeed;
    m_speedY = -kAKPlayerShotSpeed / 2.0f;
    
    // その他のパラメータを設定する
    setCommonParam(position, parent);
}

/*!
 @brief オプション弾(下向き)生成
 
 下向きに移動するオプションの弾を生成する。
 @param position 生成位置
 @param parent 配置する親ノード
 */
void AKPlayerShot::createOptionShotDownward(const Vector2 &position, Node *parent)
{
    // 攻撃力を設定する
    m_power = kAKOptionShotPower;

    // スピードを設定する。右下方向へまっすぐに進む。
    m_speedX = kAKPlayerShotSpeed;
    m_speedY = kAKPlayerShotSpeed / 2.0f;
    
    // その他のパラメータを設定する
    setCommonParam(position, parent);
}

/*!
 @brief 共通項目設定
 
 自機弾、オプション弾共通項目を設定する。
 @param position 生成位置
 @param parent 配置する親ノード
 */
void AKPlayerShot::setCommonParam(const Vector2 &position, Node *parent)
{
    // パラメータの内容をメンバに設定する
    m_position = position;
    
    // 配置フラグを立てる
    m_isStaged = true;
    
    // 画像名を設定する
    setImageName(kAKPlayerShotImage);
    
    // アニメーションフレームの個数を設定する
    m_animationPattern = 1;
    
    // アニメーションフレーム間隔を設定する
    m_animationInterval = 0.0f;
    
    // 当たり判定のサイズを設定する
    m_size.width = kAKPlayerShotWidth;
    m_size.height = kAKPlayerShotHeight;
    
    // ヒットポイントを設定する
    m_hitPoint = 1;
    
    // 障害物衝突時は消滅する
    m_blockHitAction = kAKBlockHitDisappear;

    // 敵が表示される前に倒してしまわないように
    // 自機弾の画面外判定しきい値は小さめにする
    m_outThreshold = kAKPlayerShotOutThreshold;

    // レイヤーに配置する
    parent->addChild(getImage());
}
