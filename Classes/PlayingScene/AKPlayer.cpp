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
 @file AKPlayer.m
 @brief 自機クラス定義
 
 自機を管理するクラスを定義する。
 */

#include "AKPlayer.h"
#include "AKEnemyShot.h"
#include "AKBlock.h"

using cocos2d::CCPoint;
using cocos2d::CCNode;
using cocos2d::CCBlink;

/// 自機のサイズ
static const int kAKPlayerSize = 8;
/// 自機のかすり判定サイズ
static const int kAKPlayerGrazeSize = 32;
/// 復活後の無敵状態のフレーム数
static const int kAKInvincibleTime = 120;
/// 自機の画像ファイル名
static const char *kAKPlayerImageFile = "Player_%02d";
/// 画像サイズ
static const float kAKPlayerImageSize = 32;
/// アニメーションフレーム数
static const int kAKPlayerAnimationCount = 2;
/// 弾発射の間隔
static const float kAKPlayerShotInterval = 12;
/// 最大のオプション数
static const int kAKMaxOptionCount = 3;

/*!
 @brief 親ノードを指定したコンストラクタ

 画像を読み込み、親ノードに画像を配置する。
 @param parent 画像を配置するノード
 @param optionParent オプションの画像を配置するノード
 @return 生成したオブジェクト。失敗時はnilを返す。
 */
AKPlayer::AKPlayer(CCNode *parent, CCNode *optionParent)
{
    AKLog(kAKLogPlayer_1, "AKPlayer() start:parent=%p, optionParent=%p", parent, optionParent);
    
    // サイズを設定する
    m_size.width = kAKPlayerSize;
    m_size.height = kAKPlayerSize;
    
    // アニメーションフレームの個数を設定する
    m_animationPattern = kAKPlayerAnimationCount;
    
    // 画像名を設定する
    char imageName[16] = "";
    snprintf(imageName, sizeof(imageName), kAKPlayerImageFile, 1);
    setImageName(imageName);
    
    // 状態を初期化する
    reset();
    
    // 弾発射までの残り時間を設定する
    m_shootFrame = kAKPlayerShotInterval;
    
    // チキンゲージをリセットする
    m_chickenGauge = 0;
    
    // 障害物と衝突した時の処理に自機を設定する。
    // 自機の場合は移動時は無処理(画面入力時にチェックするため)。
    // 障害物の衝突判定時は移動を行う。
    m_blockHitAction = kAKBlockHitPlayer;
    
    // 画像を親ノードに配置する
    parent->addChild(getImage());
    
    // オプションを作成する
    m_option = new AKOption(kAKMaxOptionCount, optionParent);

    AKLog(kAKLogPlayer_1, "AKPlayer() end");
}

/*!
 @brief デストラクタ

 オプションを解放する。
 */
AKPlayer::~AKPlayer()
{
    delete m_option;
}

/*!
 @brief 復活
 
 破壊された自機を復活させる。
 */
void AKPlayer::rebirth()
{    
    // HPの設定
    m_hitPoint = 1;
    
    // ステージ配置フラグを立てる
    m_isStaged = true;
    
    // 表示させる
    getImage()->setVisible(true);
    
    // 無敵状態にする
    m_isInvincible = true;
    m_invincivleFrame = kAKInvincibleTime;
    
    // チキンゲージを初期化する
    m_chickenGauge = 0;
    
    // 無敵中はブリンクする
    CCBlink *blink = CCBlink::create(kAKInvincibleTime / 60, 
                                     (kAKInvincibleTime / 60) * 8);
    getImage()->runAction(blink);
}

/*!
 @brief 初期化
 
 状態を初期化する。
 */
void AKPlayer::reset()
{
    AKLog(kAKLogPlayer_1, "reset() start");
    
    // 初期位置は原点とする
    CCPoint initPosition(0.0f, 0.0f);
    getImage()->setPosition(initPosition);
    
    // HPを設定する
    m_hitPoint = 1;
    
    // ステージ配置フラグを立てる
    m_isStaged = true;
    
    // 表示させる
    getImage()->setVisible(true);
    
    // 無敵状態はOFFにする
    m_isInvincible = false;
    m_invincivleFrame = 0;
    
    // アクションはすべて停止する
    getImage()->stopAllActions();
    
    AKLog(kAKLogPlayer_1, "reset() end");
}

/*!
 @brief かすり判定
 
 自機が敵弾にかすっているか判定し、かすっている場合は弾のかすりポイントを自機の方へ移す。
 @param characters 判定対象のキャラクター群
 */
void AKPlayer::graze(std::vector<AKEnemyShot*> &characters)
{
    // 画面に配置されていない場合は処理しない
    if (!m_isStaged) {
        return;
    }
    
    // 自キャラのかすり判定の上下左右の端を計算する
    float myleft = m_position.x - kAKPlayerGrazeSize / 2.0f;
    float myright = m_position.x + kAKPlayerGrazeSize / 2.0f;
    float mytop = m_position.y + kAKPlayerGrazeSize / 2.0f;
    float mybottom = m_position.y - kAKPlayerGrazeSize / 2.0f;
    
    AKLog(kAKLogPlayer_2, "player=(%f, %f, %f, %f)", myleft, myright, mytop, mybottom);
    
    // 判定対象のキャラクターごとに判定を行う
    for (AKEnemyShot *enemyShot : characters) {
        
        // 相手が画面に配置されていない場合は処理しない
        if (!enemyShot->isStaged()) {
            continue;
        }
        
        // 相手の上下左右の端を計算する
        float targetleft = enemyShot->getPosition()->x - enemyShot->getSize()->width / 2.0f;
        float targetright = enemyShot->getPosition()->x + enemyShot->getSize()->width / 2.0f;
        float targettop = enemyShot->getPosition()->y + enemyShot->getSize()->height / 2.0f;
        float targetbottom = enemyShot->getPosition()->y - enemyShot->getSize()->height / 2.0f;
        
        AKLog(kAKLogPlayer_3, "target=(%f, %f, %f, %f)", targetleft, targetright, targettop, targetbottom);
        
        // 以下のすべての条件を満たしている時、衝突していると判断する。
        //   ・相手の右端が自キャラの左端よりも右側にある
        //   ・相手の左端が自キャラの右端よりも左側にある
        //   ・相手の上端が自キャラの下端よりも上側にある
        //   ・相手の下端が自キャラの上端よりも下側にある
        if ((targetright > myleft) &&
            (targetleft < myright) &&
            (targettop > mybottom) &&
            (targetbottom < mytop)) {
            
            // 相手のかすりポイントを取得する
            if (enemyShot->getGrazePoint() > 0.0f) {
                
                AKLog(kAKLogPlayer_2, "かすりポイント:%f", enemyShot->getGrazePoint());
                
                m_chickenGauge += enemyShot->getGrazePoint();
                
                // 最大で100%とする
                if (m_chickenGauge > 100) {
                    m_chickenGauge = 100;
                }
            }
            
            // 相手のかすりポイントをリセットする
            enemyShot->setGrazePoint(0.0f);
        }
    }
}

/*!
 @brief 移動座標設定
 
 移動座標を設定する。オプションが付属している場合はオプションの移動も行う。
 @param position 移動先座標
 @param data ゲームデータ
 */
void AKPlayer::setPosition(const CCPoint &position, AKPlayDataInterface *data)
{
    // オプションに自分の移動前の座標を通知する
    if (m_option != NULL && m_option->isStaged()) {
        m_option->setPosition(m_position);
    }
    
    // 移動前の座標を記憶する
    savePosition();
    
    // 移動先の座標を設定する
    m_position = position;
    
    // 障害物との衝突判定を行う
    checkHit(*data->getBlocks(), data, &AKPlayer::moveOfBlockHit);
}

/*!
 @brief オプション個数更新
 
 チキンゲージに応じてオプション個数を更新する。
 */
void AKPlayer::updateOptionCount()
{
    // チキンゲージからオプション個数を計算する
    int count = m_chickenGauge / (100 / (kAKMaxOptionCount + 1));
    
    // 最大個数で制限をかける
    if (count > kAKMaxOptionCount) {
        count = kAKMaxOptionCount;
    }
    
    // 自分の座標を初期座標として次のオプションを設定する
    if (m_option != NULL) {
        m_option->setOptionCount(count, m_position);
    }
}

/*!
 @brief シールド有無設定
 
 オプションのシールド有無を設定する。
 @param shield シールド有無
 */
void AKPlayer::setShield(bool shield)
{
    // オプションがある場合はオプションのシールド有無を設定する
    if (m_option != NULL) {
        m_option->setShiled(shield);
    }
}

/*!
 @brief オプション取得
 
 オプションを取得する。
 @return オプション
 */
AKOption* AKPlayer::getOption()
{
    return m_option;
}

/*!
 @brief 無敵状態かどうか取得
 
 無敵状態かどうかを取得する。
 @return 無敵状態かどうか
 */
bool AKPlayer::isInvincible()
{
    return m_isInvincible;
}

/*!
 @brief チキンゲージ取得
 
 チキンゲージの溜まっている値を取得する。
 @return チキンゲージ
 */
int AKPlayer::getChickenGauge()
{
    return m_chickenGauge;
}

/*!
 @brief チキンゲージ設定
 
 チキンゲージの溜まっている値を設定する。
 @param chickenGauge チキンゲージ
 */
void AKPlayer::setChickenGauge(int chickenGauge)
{
    m_chickenGauge = chickenGauge;
}

/*!
 @brief キャラクター固有の動作

 速度によって位置を移動する。自機の表示位置は固定とする。
 オプションの移動も行う。
 @param data ゲームデータ
 */
void AKPlayer::action(AKPlayDataInterface *data)
{
    // 無敵状態の時は無敵フレーム数をカウントする
    if (m_isInvincible) {
        m_invincivleFrame--;
        
        // 無敵フレーム数が切れている場合は通常状態に戻す
        if (m_invincivleFrame <= 0) {
            AKLog(kAKLogPlayer_1, "無敵時間終了");
            m_isInvincible = false;
        }
    }
    
    // 弾発射までのフレーム数をカウントする
    m_shootFrame--;
    
    // 弾発射までの残りフレーム数が0になっている場合は弾を発射する
    if (m_shootFrame <= 0) {
        
        AKLog(kAKLogPlayer_1, "弾発射");
        
        // 自機弾を生成する
        data->createPlayerShot(m_position);
        
        // 弾発射までの残りフレーム数をリセットする
        m_shootFrame = kAKPlayerShotInterval;
    }
    
    // オプションの移動を行う
    if (m_option) {
        m_option->optionMove(data);
    }
}

/*!
 @brief 破壊処理
 
 HPが0になったときに爆発エフェクトを生成する。
 @param ゲームデータ
 */
void AKPlayer::destroy(AKPlayDataInterface *data)
{
    AKLog(kAKLogPlayer_1, "destroy() start");

    // TODO:破壊時の効果音を鳴らす

    // 画面効果を生成する
    data->createEffect(2, m_position);
    
    // 配置フラグを落とす
    m_isStaged = false;
    
    // 非表示とする
    getImage()->setVisible(false);
    
    // 自機破壊時の処理を行う
    data->miss();
    
    AKLog(kAKLogPlayer_1, "destroy() end");
}
