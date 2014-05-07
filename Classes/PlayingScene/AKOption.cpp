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
 @file AKOption.cpp
 @brief オプションクラス
 
 オプションを管理するクラスを定義する。
 */

#include "AKOption.h"
#include "AKEnemyShot.h"

using cocos2d::Node;
using cocos2d::Vector2;

/// オプションの画像ファイル名
static const char *kAKOptionImageFile = "Option_%02d";
/// シールドなし時のアニメーションフレーム数
static const int kAKOptionAnimationCountOfShieldOff = 2;
/// シールドあり時のアニメーションフレーム数
static const int kAKOptionAnimationCountOfShieldOn = 1;
/// 弾発射の間隔
static const int kAKOptionShotInterval = 12;
/// オプション間の距離
static const int kAKOptionSpace = 20;
/// オプションの当たり判定
static const int kAKOptionSize = 32;

/*!
 @brief オブジェクト生成処理
 
 オブジェクトの生成を行う。
 指定されたオプションの個数分を再帰的に生成する。
 @param count オプションの個数
 @param parent 画像を配置する親ノード
 @return 生成したオブジェクト。失敗時はnilを返す。
 */
AKOption::AKOption(int count, Node *parent)
{
    // アニメーションフレームの個数を設定する
    m_animationPattern = kAKOptionAnimationCountOfShieldOff;
    
    // 画像名を設定する
    char imageName[16] = "";
    snprintf(imageName, sizeof(imageName), kAKOptionImageFile, 1);
    setImageName(imageName);
    
    // 弾発射までの残りフレーム数を設定する
    m_shootFrame = kAKOptionShotInterval;

    // ヒットポイントは1としておく
    m_hitPoint = 1;
    
    // オプションの当たり判定を設定する
    // これはシールド時のみ使用する
    m_size.width = kAKOptionSize;
    m_size.height = kAKOptionSize;
    
    // 初期状態はシールドなしとする
    m_shield = false;

    // 初期状態では画面には配置しない
    m_isStaged = false;
    getImage()->setVisible(false);
    
    // 画像を親ノードに配置する
    parent->addChild(getImage());
    
    // オプション個数が指定されている場合は次のオプションを生成する
    if (count > 0) {
        m_next = new AKOption(count - 1, parent);
    }
    else {
        m_next = NULL;
    }
}

/*!
 @brief デストラクタ
 
 次のオプションを解放する。
 */
AKOption::~AKOption()
{
    delete m_next;
}

/*!
 @brief シールド有無設定
 
 シールド有無を設定する。
 画像の切り替えも行う。
 次のオプションに対しても同様の設定を行う。
 @param shield シールド有無
 */
void AKOption::setShiled(bool shield)
{
    // メンバに設定する
    m_shield = shield;
        
    // シールド有無によって画像を切り替える
    if (m_shield) {
        
        AKLog(kAKLogOption_1, "シールドあり");
        
        // 画像名を設定する
        char imageName[16] = "";
        snprintf(imageName, sizeof(imageName), kAKOptionImageFile, 2);
        setImageName(imageName);
        
        // アニメーションフレームの個数を設定する
        m_animationPattern = kAKOptionAnimationCountOfShieldOn;
    }
    else {

        AKLog(kAKLogOption_1, "シールドなし");

        // 画像名を設定する
        char imageName[16] = "";
        snprintf(imageName, sizeof(imageName), kAKOptionImageFile, 1);
        setImageName(imageName);
        
        // アニメーションフレームの個数を設定する
        m_animationPattern = kAKOptionAnimationCountOfShieldOff;
    }
    
    // 次のオプションがある場合は次のオプションにも設定する
    if (m_next != NULL) {
        m_next->setShiled(m_shield);
    }
}

/*!
 @brief オプション数設定
 
 オプションの個数を設定する。
 1以上が設定された場合は自分を配置状態に設定する。
 0以下が設定された場合は自分を配置状態から外す。
 次のオプションが存在する場合は個数を一つ減らして再帰的に呼び出す。
 @param count オプション個数
 @param position 初期配置位置
 */
void AKOption::setOptionCount(int count, const Vector2 &position)
{
    AKLog(kAKLogOption_2, "count=%d", count);
    
    // オプション個数が設定された場合はオプションを有効とする
    if (count > 0) {
        
        // 配置されていない場合は配置状態にして、初期配置位置に移動する
        if (!m_isStaged) {
            AKLog(kAKLogOption_1, "オプション配置");

            m_isStaged = true;
            getImage()->setVisible(true);
            m_position = position;

            // 初期表示時に前回の位置に表示されることを防ぐため、画像表示位置の更新も行う
            updateImagePosition();
        }
    }
    // オプション個数が0以下の場合はオプションを無効とする
    else {
        
        // 配置されている場合は配置状態を解除して、移動座標をすべてクリアする
        if (m_isStaged) {
            AKLog(kAKLogOption_1, "オプション削除");

            m_isStaged = false;
            getImage()->setVisible(false);
            m_movePositions.clear();
        }
    }
    
    // 自分の座標を初期座標として次のオプションを設定する
    if (m_next != NULL) {
        m_next->setOptionCount(count - 1, m_position);
    }
}

/*!
 @brief 移動座標設定
 
 移動座標を設定する。オプションが付属している場合はオプションの移動も行う。
 @param position 移動先座標
 */
void AKOption::setPosition(const Vector2 &position)
{
    // オプションに自分の移動前の座標を通知する
    if (m_next != NULL && m_next->isStaged()) {
        
        m_next->setPosition(m_position);
    }
    
    // 移動先座標が間隔分溜まっている場合は先頭の座標に移動する
    if (m_movePositions.size() >= kAKOptionSpace) {
        
        // 先頭の要素を取得し、座標を移動する
        std::vector<Vector2>::iterator start = m_movePositions.begin();
        m_position = *start;
        
        // 取得した要素を配列から取り除く
        m_movePositions.erase(start);
    }
    
    // 移動先座標の配列の末尾に追加する
    m_movePositions.push_back(position);
}

/*!
 @brief オプション移動
 
 オプションの移動を行う。自機クラスから呼び出される。
 @param data ゲームデータ
 */
void AKOption::optionMove(AKPlayDataInterface *data)
{
    move(data);
}

/*!
 @brief 次のオプション取得
 
 次のオプションを取得する。
 最後のオプションの場合はNULLを返す。
 @return 次のオプション
 */
AKOption* AKOption::getNext()
{
    return m_next;
}

/*!
 @brief キャラクター固有の動作
 
 速度によって位置を移動する。オプションの表示位置は固定とする。
 次のオプションの移動を行う。
 @param data ゲームデータ
 */
void AKOption::action(AKPlayDataInterface *data)
{
    // 弾発射までのフレーム数をカウントする
    m_shootFrame--;
    
    // 弾発射までの残りフレーム数が0になっている場合は弾を発射する
    if (m_shootFrame <= 0) {
        
        // オプション弾を生成する
        data->createOptionShot(m_position);
        
        // 弾発射までの残り時間をリセットする
        m_shootFrame = kAKOptionShotInterval;
    }
    
    // 次のオプションの移動を行う
    if (m_next != NULL) {
        m_next->move(data);
    }
}

/*!
 @brief 衝突処理
 
 衝突した時の処理。
 反射弾を作成後、相手のHPを0にする。
 @param character 衝突した相手
 @param data ゲームデータ
 */
void AKOption::hit(AKCharacter *character, AKPlayDataInterface *data)
{
    AKLog(kAKLogOption_1, "反射処理開始");
    
    // 反射弾を作成する
    data->createReflectShot(static_cast<AKEnemyShot*>(character));
    
    // 相手のHPを0にする
    character->setHitPoint(0);
}

