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
 @file AKEnemyShot.cpp
 @brief 敵弾クラス定義
 
 敵の発射する弾のクラスを定義する。
 */

#include "AKEnemyShot.h"

using cocos2d::Vec2;
using cocos2d::Node;

// 敵弾の種類
enum AKEnemyShotType {
    kAKEnemyShotTypeNormal = 0,     ///< 標準弾
    kAKEnemyShotTypeChangeSpeed,    ///< 速度変更弾
    kAKEnemyShotTypeDefCount        ///< 敵弾の種類の数
};

/// 敵弾画像定義
struct AKEnemyShotImageDef {
    int fileNo;                 ///< ファイル名の番号
    int animationFrame;         ///< アニメーションフレーム数
    float animationInterval;    ///< アニメーション更新間隔
};

/// 画像名のフォーマット
static const char *kAKImageNameFormat = "EnemyShot_%02d";
/// 反射弾の威力
static const int kAKReflectionPower = 5;
/// 画像の種類の数
static const int kAKEnemyShotImageDefCount = 1;

/// 敵弾画像の定義
static const struct AKEnemyShotImageDef kAKEnemyShotImageDef[kAKEnemyShotImageDefCount] = {
    {1, 1, 0.0f}   // 標準弾
};

/// 敵弾の定義
const struct AKEnemyShot::AKEnemyShotDef AKEnemyShot::kAKEnemyShotDef[AKEnemyShot::kAKEnemyShotTypeDefCount] = {
    {&AKEnemyShot::actionNone, 1, 6, 6, 20},            // 標準弾
    {&AKEnemyShot::actionChangeSpeed, 1, 6, 6, 20}      // 速度変更弾
};

/*!
 @brief かすりポイント取得

 かすりポイントを取得する。
 @return かすりポイント
 */
float AKEnemyShot::getGrazePoint()
{
    return m_grazePoint;
}

/*!
 @brief かすりポイント設定
 
 かすりポイントを設定する。
 @param grazePoint かすりポイント
 */
void AKEnemyShot::setGrazePoint(float grazePoint)
{
    m_grazePoint = grazePoint;
}

/*!
 @brief 通常弾生成
 
 通常弾を生成する。
 @param position 生成位置
 @param angle 進行方向
 @param speed スピード
 @param parent 配置する親ノード
 */
void AKEnemyShot::createNormalShot(const Vec2 &position,
                                   float angle,
                                   float speed,
                                   Node *parent)
{
    // 種別に通常弾を指定して生成を行う
    createEnemyShot(kAKEnemyShotTypeNormal,
                    position,
                    angle,
                    speed,
                    parent);
}

/*!
 @brief スクロール影響弾生成
 
 スクリールスピードの影響を受ける弾を生成する。
 @param position 生成位置
 @param angle 進行方向
 @param speed スピード
 @param parent 配置する親ノード
 */
void AKEnemyShot::createScrollShot(const Vec2 &position,
                                   float angle,
                                   float speed,
                                   Node *parent)
{
    // 種別に通常弾を指定して生成を行う
    createEnemyShot(kAKEnemyShotTypeNormal,
                    position,
                    angle,
                    speed,
                    parent);
    
    // スクロールスピードの影響を設定する
    m_scrollSpeed = 1.0f;
}

/*!
 @brief 速度変更弾生成

 途中で速度を変更する弾を生成する。
 @param position 生成位置
 @param angle 進行方向
 @param speed スピード
 @param changeInterval 変更までの間隔
 @param changeAngle 変更後の角度
 @param changeSpeed 変更後のスピード
 @param parent 配置する親ノード
 */
void AKEnemyShot::createChangeSpeedShot(const Vec2 &position,
                                        float angle,
                                        float speed,
                                        int changeInterval,
                                        float changeAngle,
                                        float changeSpeed,
                                        Node *parent)
{
    // 種別に速度変更弾を指定して生成を行う
    createEnemyShot(kAKEnemyShotTypeChangeSpeed,
                    position,
                    angle,
                    speed,
                    parent);
    
    // 速度変更までの間隔を設定する
    m_changeInterval = changeInterval;
    
    // 変更後のスピードを設定する
    m_changeSpeedX = cosf(changeAngle) * changeSpeed;
    m_changeSpeedY = sinf(changeAngle) * changeSpeed;
}

/*!
 @brief 反射弾生成
 
 反射弾を生成する。
 元になった弾と速度を反対にし、残りのパラメータは同じものを生成する。
 @param base 反射する弾
 @param parent 配置する親ノード
 */
void AKEnemyShot::createReflectShot(AKEnemyShot *base, Node *parent)
{
    AKLog(kAKLogEnemyShot_1, "反射弾生成");
    
    // 位置を設定する
    m_position = *base->getPosition();
    
    // スピード反転させて設定する
    m_speedX = -base->getSpeedX();
    m_speedY = -base->getSpeedY();
    
    // 配置フラグを立てる
    m_isStaged = true;
    
    // 動作フレーム数をクリアする
    m_frame = 0;
    
    // 状態をクリアする
    m_state = 0;
    
    // 動作処理をなしにする
    m_action = &AKEnemyShot::actionNone;
    
    // 画像名を作成する
    setImageName(*base->getImageName());
    
    // アニメーションフレームの個数を設定する
    m_animationPattern = base->getAnimationPattern();
    
    // アニメーションフレーム間隔を設定する
    m_animationInterval = base->getAnimationInterval();
    
    // 当たり判定のサイズを設定する
    m_size = *base->getSize();
    
    // ヒットポイントを設定する
    m_hitPoint = 1;
    
    // 攻撃力は反射弾の場合は補正をかける
    m_power = kAKReflectionPower;
    
    // 障害物衝突時は消滅する
    m_blockHitAction = kAKBlockHitDisappear;
    
    // レイヤーに配置する
    parent->addChild(getImage());
}

/*!
 @brief キャラクター固有の動作
 
 生成時に指定されたセレクタを呼び出す。
 @param dt フレーム更新間隔
 @param data ゲームデータ
 */
void AKEnemyShot::action(AKPlayDataInterface *data)
{
    // 動作開始からのフレーム数をカウントする
    m_frame++;

    // 敵弾種別ごとの処理を実行
    (this->*m_action)(data);
}

/*!
 @brief 敵弾生成
 
 敵の弾を生成する。
 @para, type 種別
 @param position 生成位置
 @param angle 進行方向
 @param speed スピード
 @param parent 配置する親ノード
 */
void AKEnemyShot::createEnemyShot(int type,
                                  const Vec2 &position,
                                  float angle,
                                  float speed,
                                  Node *parent)
{
    // パラメータの内容をメンバに設定する
    m_position = position;
    
    // スピードをxとyに分割して設定する
    m_speedX = cos(angle) * speed;
    m_speedY = sin(angle) * speed;
    
    // 配置フラグを立てる
    m_isStaged = true;

    // 動作フレーム数をクリアする
    m_frame = 0;
    
    // 状態をクリアする
    m_state = 0;
    
    // 動作処理を設定する
    m_action = kAKEnemyShotDef[type].action;
    
    // 画像定義を取得する
    const struct AKEnemyShotImageDef *imageDef = &kAKEnemyShotImageDef[kAKEnemyShotDef[type].image - 1];
    
    // 画像名を作成する
    char imageName[16] = "";
    snprintf(imageName, sizeof(imageName), kAKImageNameFormat, imageDef->fileNo);
    setImageName(imageName);
    
    // アニメーションフレームの個数を設定する
    m_animationPattern = imageDef->animationFrame;
    
    // アニメーションフレーム間隔を設定する
    m_animationInterval = imageDef->animationInterval;
    
    // 当たり判定のサイズを設定する
    m_size.width = kAKEnemyShotDef[type].hitWidth;
    m_size.height = kAKEnemyShotDef[type].hitHeight;
    
    // ヒットポイントを設定する
    m_hitPoint = 1;
    
    // かすりポイントを設定する
    m_grazePoint = kAKEnemyShotDef[type].grazePoint;
    
    // 障害物衝突時は消滅する
    m_blockHitAction = kAKBlockHitDisappear;
    
    // スクロールをなしにする
    m_scrollSpeed = 0.0f;

    // レイヤーに配置する
    parent->addChild(getImage());
}

/*!
 @brief 動作処理なし
 
 スピード一定のまま進めるため、無処理。
 @param data ゲームデータ
 */
void AKEnemyShot::actionNone(AKPlayDataInterface *data)
{
    
}

/*!
 @brief 速度変更
 
 途中で速度を変更する。
 @param data ゲームデータ
 */
void AKEnemyShot::actionChangeSpeed(AKPlayDataInterface *data)
{
    // 速度変更までの間隔が経過している場合は速度を変更する
    if (m_frame >= m_changeInterval) {
        
        m_speedX = m_changeSpeedX;
        m_speedY = m_changeSpeedY;
    }
}
