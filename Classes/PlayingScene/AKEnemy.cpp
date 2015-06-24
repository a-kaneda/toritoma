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
 @file AKEnemy.cpp
 @brief 敵クラス定義
 
 敵キャラクターのクラスの定義をする。
 */

#include "AKEnemy.h"
#include "AKEnemyShot.h"
#include "AKBlock.h"

using cocos2d::Vec2;
using cocos2d::Size;
using cocos2d::Node;
using CocosDenshion::SimpleAudioEngine;

/// 敵種別定義
struct AKEnemyDef {
    AKEnemy::AKActionFunc action;   ///< 動作処理
    AKEnemy::AKActionFunc destroy;  ///< 破壊処理の種別
    int image;                      ///< 画像ID
    int animationFrame;             ///< アニメーションフレーム数
    int animationInterval;          ///< アニメーション更新間隔
    int hitWidth;                   ///< 当たり判定の幅
    int hitHeight;                  ///< 当たり判定の高さ
    int offsetX;                    ///< 当たり判定オフセットx軸
    int offsetY;                    ///< 当たり判定オフセットy軸
    int historyCount;               ///< 移動履歴を保存する個数
    int hitPoint;                   ///< ヒットポイント
    int defence;                    ///< 防御力
    int score;                      ///< スコア
    int boss;                       ///< ボスフラグ(0:雑魚、1:ボス)
};

/// 画像名のフォーマット
static const char *kAKImageNameFormat = "Enemy_%02d";
/// 敵撃破時のチキンゲージ増加量
static const int kAKIncrementChickenGauge = 5;

/// 敵の種類
enum AKEnemyType {
    kAKEnemyDragonfly = 1,              ///< トンボ
    kAKEnemyAnt = 2,                    ///< アリ
    kAKEnemyButterfly = 3,              ///< チョウ
    kAKEnemyLadybug = 4,                ///< テントウムシ
    kAKEnemyBagworm = 11,               ///< ミノムシ
    kAKEnemyCicada= 12,                 ///< セミ
    kAKEnemyGrasshopper = 13,           ///< バッタ
    kAKEnemyHornet = 14,                ///< ハチ
    kAKEnemyCockroach = 21,             ///< ゴキブリ
    kAKEnemySnail = 22,                 ///< カタツムリ
    kAKEnemyStagBeetle = 23,            ///< クワガタ
    kAKEnemyRhinocerosBeetle = 31,      ///< カブトムシ
    kAKEnemyMantis = 32,                ///< カマキリ
    kAKEnemyHoneycomb = 33,             ///< ハチの巣
    kAKEnemySpider = 34,                ///< クモ
    kAKEnemyCentipedeHead = 35,         ///< ムカデ（頭）
    kAKEnemyCentipedeBody = 36,         ///< ムカデ（胴体）
    kAKEnemyCentipedeTail = 37,         ///< ムカデ（尾）
    kAKEnemyMaggot = 38,                ///< ウジ
    kAKEnemyFly = 39                    ///< ハエ
    };

/// 敵の定義
const struct AKEnemyDef AKEnemy::kAKEnemyDef[kAKEnemyDefCount] = {
    // 破壊,画像,フレーム数,フレーム間隔,幅,高さ,オフセットX,オフセットY,移動履歴,HP,防御力,スコア,ボスフラグ
    {&AKEnemy::actionOfDragonfly, &AKEnemy::destroyNormal, 1, 2, 30, 32, 32, 0, 0, 0, 3, 0, 100, 0},    // トンボ
    {&AKEnemy::actionOfAnt, &AKEnemy::destroyNormal, 2, 2, 30, 32, 16, 0, 0, 0, 7, 0, 200, 0},          // アリ
    {&AKEnemy::actionOfButterfly, &AKEnemy::destroyNormal, 3, 2, 30, 32, 32, 0, 0, 0, 10, 0, 200, 0},   // チョウ
    {&AKEnemy::actionOfLadybug, &AKEnemy::destroyNormal, 4, 2, 6, 32, 32, 0, 0, 0, 18, 0, 200, 0},      // テントウムシ
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備5
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備6
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備7
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備8
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備9
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備10
    {&AKEnemy::actionOfBagworm, &AKEnemy::destroyNormal, 11, 1, 0, 32, 32, 0, 0, 0, 30, 0, 300, 0},     // ミノムシ
    {&AKEnemy::actionOfCicada, &AKEnemy::destroyNormal, 12, 1, 0, 32, 32, 0, 0, 0, 20, 0, 200, 0},      // セミ
    {&AKEnemy::actionOfGrasshopper, &AKEnemy::destroyNormal, 13, 1, 0, 32, 32, 0, 0, 0, 9, 0, 200, 0},  // バッタ
    {&AKEnemy::actionOfHornet, &AKEnemy::destroyNormal, 14, 2, 6, 32, 32, 0, 0, 0, 12, 0, 200, 0},      // ハチ
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備15
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備16
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備17
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備18
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備19
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備20
    {&AKEnemy::actionOfCockroach, &AKEnemy::destroyNormal, 21, 2, 6, 32, 32, 0, 0, 0, 15, 0, 300, 0},   // ゴキブリ
    {&AKEnemy::actionOfSnail, &AKEnemy::destroyNormal, 22, 2, 30, 32, 32, 0, 0, 0, 13, 0, 200, 0},      // カタツムリ
    {&AKEnemy::actionOfStagBeetle, &AKEnemy::destroyNormal, 23, 2, 6, 32, 32, 0, 0, 0, 20, 0, 300, 0},  // クワガタ
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備24
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備25
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備26
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備27
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備28
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備29
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // 予備30
    {&AKEnemy::actionOfRhinocerosBeetle, &AKEnemy::destroyOfBoss, 31, 2, 3, 64, 40, 0, 0, 0, 1000, 0, 3000, 1},     // カブトムシ
    {&AKEnemy::actionOfMantis, &AKEnemy::destroyOfBoss, 32, 0, 0, 64, 64, 0, 0, 0, 1000, 3, 3000, 1},               // カマキリ
    {&AKEnemy::actionOfHoneycomb, &AKEnemy::destroyOfBoss, 33, 0, 0, 64, 64, 0, 0, 0, 1200, 3, 3000, 1},            // ハチの巣
    {&AKEnemy::actionOfSpider, &AKEnemy::destroyOfBoss, 34, 2, 12, 64, 64, 0, 0, 0, 1600, 0, 3000, 1},              // クモ
    {&AKEnemy::actionOfCentipedeHead, &AKEnemy::destroyOfBoss, 35, 0, 0, 32, 32, 0, 16, 11, 19, 99, 3000, 1},       // ムカデ（頭）
    {&AKEnemy::actionOfCentipedeBody, &AKEnemy::destroyNone, 36, 2, 12, 32, 16, 0, 0, 11, 1, 99, 0, 0},             // ムカデ（胴体）
    {&AKEnemy::actionOfCentipedeTail, &AKEnemy::destroyOfCentipede, 37, 2, 12, 32, 16, 0, -24, 0, 15, 0, 0, 0},     // ムカデ（尾）
    {&AKEnemy::actionOfMaggot, &AKEnemy::destroyOfMaggot, 38, 2, 30, 16, 16, 0, 0, 0, 150, 0, 500, 0},              // ウジ
    {&AKEnemy::actionOfFly, &AKEnemy::destroyOfBoss, 39, 2, 6, 32, 32, 0, 0, 0, 1500, 0, 8000, 1},                  // ハエ
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}    // 予備40
};

/*!
 @brief 自機を狙うn-way弾発射
 
 自機を狙うn-way弾の発射を行う。
 @param position 発射する位置
 @param count 発射方向の数
 @param interval 弾の間隔
 @param speed 弾の速度
 @param data ゲームデータ
 */
void AKEnemy::fireNWay(Vec2 position,
                       int count,
                       float interval,
                       float speed,
                       AKPlayDataInterface *data)
{
    // n-way弾の各弾の角度を計算する
    AKNWayAngle nWayAngle(position,
                          *data->getPlayerPosition(),
                          count,
                          interval);
    
    // 各弾を発射する
    for (float angle : *nWayAngle.getAngles()) {
        
        // 敵弾インスタンスを取得する
        AKEnemyShot *enemyShot = data->getEnemyShot();
        
        // 取得できない場合は処理しない
        if (enemyShot == NULL) {
            break;
        }
        
        // 通常弾を生成する
        enemyShot->createNormalShot(position,
                                    angle,
                                    speed,
                                    data->getEnemyShotParent());
    }
}

/*!
 @brief 角度指定によるn-way弾発射
 
 角度指定によるn-way弾の発射を行う。
 @param angle 発射する角度
 @param position 発射する位置
 @param count 発射方向の数
 @param interval 弾の間隔
 @param speed 弾の速度
 @param isScroll スクロールの影響を受けるか
 @param data ゲームデータ
 */
void AKEnemy::fireNWay(float angle,
                       Vec2 position,
                       int count,
                       float interval,
                       float speed,
                       bool isScroll,
                       AKPlayDataInterface *data)
{
    // n-way弾の各弾の角度を計算する
    AKNWayAngle nWayAngle(angle,
                          count,
                          interval);
    
    // 各弾を発射する
    for (float angle : *nWayAngle.getAngles()) {

        // 敵弾インスタンスを取得する
        AKEnemyShot *enemyShot = data->getEnemyShot();
        
        // 取得できない場合は処理しない
        if (enemyShot == NULL) {
            break;
        }
        
        // スクロールの影響を受けるかどうかで弾の種別を変える
        if (isScroll) {
            // スクロール影響弾を生成する
            enemyShot->createScrollShot(position,
                                        angle,
                                        speed,
                                        data->getEnemyShotParent());
        }
        else {
            // 通常弾を生成する
            enemyShot->createNormalShot(position,
                                        angle,
                                        speed,
                                        data->getEnemyShotParent());
        }
    }
}

/*!
 @brief 自機を狙うグループ弾発射
 
 自機を狙う一塊のグループ弾を発射する。
 中心点から自機の角度を計算し、すべての弾をその角度で発射する。
 @param position グループ弾の中心点の座標
 @param distance 中心点からの距離
 @param count 弾の数
 @param speed 弾の速度
 @param data ゲームデータ
 */
void AKEnemy::fireGroupShot(Vec2 position,
                            const float distance[][2],
                            int count,
                            float speed,
                            AKPlayDataInterface *data)
{
    // 弾の角度を計算する
    AKNWayAngle nWayAngle(position,
                          *data->getPlayerPosition(),
                          1,
                          0.0f);
    
    // 各弾の位置に通常弾を生成する
    for (int i = 0; i < count; i++) {

        // 敵弾インスタンスを取得する
        AKEnemyShot *enemyShot = data->getEnemyShot();
        
        // 取得できない場合は処理しない
        if (enemyShot == NULL) {
            break;
        }
        
        // 通常弾を生成する
        Vec2 shotPosition(position.x + distance[i][0],
                             position.y + distance[i][1]);
        enemyShot->createNormalShot(shotPosition,
                                    nWayAngle.getTopAngle(),
                                    speed,
                                    data->getEnemyShotParent());
    }
}

/*!
 @brief 破裂弾発射
 
 一定時間で破裂する弾を発射する。
 @param position 発射する位置
 @param count 破裂後の数
 @param interval 破裂後の弾の間隔
 @param speed 弾の速度
 @param burstInterval 破裂までの間隔
 @param burstSpeed 破裂後の速度
 @param data ゲームデータ
 */
void AKEnemy::fireBurstShot(Vec2 position,
                            int count,
                            float interval,
                            float speed,
                            float burstInterval,
                            float burstSpeed,
                            AKPlayDataInterface *data)
{
    // 中心点からの弾の距離
    const float kAKDistance = 4.0f;
    
    // 破裂弾弾全体の角度を計算する
    AKNWayAngle centerAngle(position,
                            *data->getPlayerPosition(),
                            1,
                            0.0f);
    
    // 個別の弾の角度を計算する
    AKNWayAngle burstAngle(M_PI, count, interval);
    
    // 各弾を発射する
    for (float angle : *burstAngle.getAngles()) {
        
        // 敵弾インスタンスを取得する
        AKEnemyShot *enemyShot = data->getEnemyShot();
        
        // 取得できない場合は処理しない
        if (enemyShot == NULL) {
            break;
        }
        
        // 破裂弾を生成する
        Vec2 shotPosition(position.x + cosf(angle) * kAKDistance,
                             position.y + sinf(angle) * kAKDistance);
        enemyShot->createChangeSpeedShot(shotPosition,
                                        centerAngle.getTopAngle(),
                                        speed,
                                        burstInterval,
                                        angle,
                                        burstSpeed,
                                        data->getEnemyShotParent());

    }
}

/*!
 @brief 障害物との衝突判定
 
 障害物との衝突判定を行う。
 移動先と移動元の段差が1/2ブロック以上ある場合は移動しない。
 左端の足元と右端の足元で障害物の高さが異なる場合は高い方（逆さまの場合は低い方）に合わせる。
 @param current 現在位置
 @param size キャラクターのサイズ
 @param isReverse 逆さまになっているかどうか
 @param data ゲームデータ
 @return 移動先の座標
 */
Vec2 AKEnemy::checkBlockPosition(const Vec2 &current,
                                    const Size &size,
                                    bool isReverse,
                                    AKPlayDataInterface *data)
{
    // 乗り越えることのできる高さ
    const int kAKHalfBlockSize = 17;
    
    // 頭の座標を計算する
    float top = 0.0f;
    if (!isReverse) {
        top = current.y + size.height / 2.0f;
    }
    else {
        top = current.y - size.height / 2.0f;
    }
        
    // 左端の座標を計算する
    float left = current.x - size.width / 2.0f;
    
    // 左側の足元の障害物を取得する
    AKCharacter *leftBlock = AKEnemy::getBlockAtFeet(left,
                                                     current.x,
                                                     top,
                                                     isReverse,
                                                     *data->getBlocks());
    
    // 右端の座標を計算する
    float right = current.x+ size.width / 2.0f;

    // 左側の足元の障害物を取得する
    AKCharacter *rightBlock = AKEnemy::getBlockAtFeet(right,
                                                      current.x,
                                                      top,
                                                      isReverse,
                                                      *data->getBlocks());
    
    // 足元に障害物がない場合は移動はしない
    if (leftBlock == NULL && rightBlock == NULL) {
        AKLog(kAKLogEnemy_2, "足元に障害物なし");
        return current;
    }
    
    // 高さを合わせる障害物と移動先のx座標を決定する
    AKCharacter *blockAtFeet = NULL;
    float newX = 0.0f;

    // 左側の障害物がない場合は自分の左端を右側の障害物の左端に合わせる
    if (leftBlock == NULL) {
        AKLog(kAKLogEnemy_2, "左側に障害物なし");
        newX = rightBlock->getPosition()->x - rightBlock->getSize()->width / 2.0f + size.width / 2.0f;
        blockAtFeet = rightBlock;
    }
    // 右側の障害物がない場合は自分の右端を左側の障害物の右端に合わせる
    else if (rightBlock == NULL) {
        AKLog(kAKLogEnemy_2, "右側に障害物なし");
        newX = leftBlock->getPosition()->x + leftBlock->getSize()->width / 2.0f - size.width / 2.0f;
        blockAtFeet = leftBlock;
    }
    // 左右の障害物の高さの差が1/2ブロック以上ある場合は進行方向と逆側の障害物に合わせる
    else if ((!isReverse && (fabsf((leftBlock->getPosition()->y + leftBlock->getSize()->height / 2.0f) - (rightBlock->getPosition()->y + rightBlock->getSize()->height / 2.0f)) > kAKHalfBlockSize)) ||
             (isReverse && (fabsf((leftBlock->getPosition()->y - leftBlock->getSize()->height / 2.0f) - (rightBlock->getPosition()->y - rightBlock->getSize()->height / 2.0f)) > kAKHalfBlockSize))) {
        
        AKLog(kAKLogEnemy_2, "左右の障害物の高さの差が1/2ブロック以上");
        AKLog(kAKLogEnemy_2, "rx=%f lx=%f cx=%f", rightBlock->getPosition()->x, leftBlock->getPosition()->x, current.x);
        
        // 近い方のブロックに移動する
        if  (rightBlock->getPosition()->x - current.x < current.x - leftBlock->getPosition()->x) {
            AKLog(kAKLogEnemy_2, "右側のブロックに合わせる");
            
            newX = rightBlock->getPosition()->x - rightBlock->getSize()->width / 2.0f + size.width / 2.0f;
            blockAtFeet = rightBlock;
        }
        else {
            AKLog(kAKLogEnemy_2, "左側のブロックに合わせる");
            
            newX = leftBlock->getPosition()->x + leftBlock->getSize()->width / 2.0f - size.width / 2.0f;
            blockAtFeet = leftBlock;
        }
    }
    // その他の場合は足元に近い方の高さに合わせる
    else {
        
        // 逆向きでない場合は上の方にあるものを採用する
        if (!isReverse) {
            if (leftBlock->getPosition()->y + leftBlock->getSize()->height / 2.0f > rightBlock->getPosition()->y + rightBlock->getSize()->height / 2.0f) {
                blockAtFeet = leftBlock;
            }
            else {
                blockAtFeet = rightBlock;
            }
        }
        // 逆向きの場合は下の方にあるものを採用する
        else {
            if (leftBlock->getPosition()->y - leftBlock->getSize()->height / 2.0f < rightBlock->getPosition()->y - rightBlock->getSize()->height / 2.0f) {
                blockAtFeet = leftBlock;
            }
            else {
                blockAtFeet = rightBlock;
            }
        }
        
        // x軸方向は移動しない
        newX = current.x;
    }
    
    // 移動先のy座標を決定する
    float newY = 0.0f;
    
    // 逆向きでない場合は自分の下端の位置を障害物の上端に合わせる
    if (!isReverse) {
        newY = blockAtFeet->getPosition()->y + blockAtFeet->getSize()->height / 2.0f + size.height / 2.0f;
    }
    // 逆向きの場合は自分の上端の位置を障害物の下端に合わせる
    else {
        newY = blockAtFeet->getPosition()->y - blockAtFeet->getSize()->height / 2.0f - size.height / 2.0f;
    }
    
    AKLog(kAKLogEnemy_3, "(%.0f, %.0f)->(%.0f, %.0f)", current.x, current.y, newX, newY);
    
    // 移動先の座標を返す
    return Vec2(newX, newY);
}

/*!
 @brief 足元の障害物を取得する
 
 足元の障害物を取得する。
 指定したx座標で一番上にある障害物を取得する。ただし、頭よりも上にある障害物は除外する。
 逆さまになっている場合は上下を逆にして検索を行う。
 @param x x座標
 @param center 中心の位置
 @param top 頭の位置
 @param isReverse 逆さまになっているかどうか
 @param blocks 障害物
 @return 足元の障害物
 */
AKCharacter* AKEnemy::getBlockAtFeet(float x,
                                     float center,
                                     float from,
                                     bool isReverse,
                                     const std::vector<AKBlock*> &blocks)
{    
    // 足元の障害物を探す
    AKCharacter *blockAtFeet = NULL;
    for (AKBlock *block : blocks) {
        
        // 配置されていない障害物は除外する
        if (!block->isStaged()) {
            continue;
        }
        
        // 障害物の幅の範囲内に指定座標が入っていない場合は除外する
        if (roundf(block->getPosition()->x - block->getSize()->width / 2) > roundf(x) ||
            roundf(block->getPosition()->x + block->getSize()->width / 2) < roundf(x)) {
            
            continue;
        }
        
        // 逆さまでない場合は上端より上にあるものは除外する
        if (!isReverse) {
            if (roundf(block->getPosition()->y - block->getSize()->height / 2) > roundf(from)) {
                continue;
            }
        }
        // 逆さまの場合は下端より下にあるものは除外する
        else {
            if (roundf(block->getPosition()->y + block->getSize()->height / 2) < roundf(from)) {
                continue;
            }
        }
        
        // 最初に見つかったブロックの場合は無条件に採用する
        if (blockAtFeet == NULL) {
            blockAtFeet = block;
            continue;
        }
        
        // 逆さまでない場合は一番上のものを採用する
        if (!isReverse) {
            if (block->getPosition()->y + block->getSize()->height / 2 >
                blockAtFeet->getPosition()->y + blockAtFeet->getSize()->height / 2) {
                
                blockAtFeet = block;
                continue;
            }
        }
        // 逆さまの場合は一番下のものを採用する
        else {
            if (block->getPosition()->y + block->getSize()->height / 2 <
                blockAtFeet->getPosition()->y + blockAtFeet->getSize()->height / 2) {
                
                blockAtFeet = block;
                continue;
            }
        }
        
        // 同じ高さの場合は近い方を採用する
        if (AKIsEqualFloat(block->getPosition()->y + block->getSize()->height / 2,
            blockAtFeet->getPosition()->y + blockAtFeet->getSize()->height / 2)) {
            
            if (fabs(center - block->getPosition()->x) < fabs(center - blockAtFeet->getPosition()->x)) {
                blockAtFeet = block;
                continue;
            }
        }
    }
    
    return blockAtFeet;
}


/*!
 @brief 生成処理

 敵キャラを生成する。
 @param type 敵キャラの種別
 @param position 生成位置
 @param progress 倒した時に進む進行度
 @param parent 敵キャラを配置する親ノード
 */
void AKEnemy::createEnemy(int type,
                          const Vec2 &position,
                          int progress,
                          Node *parent)
{
    AKLog(kAKLogEnemy_1, "start createEnemy():type=%d", type); 

    // パラメータの内容をメンバに設定する
    m_position = position;
    m_progress = progress;
    
    // 配置フラグを立てる
    m_isStaged = true;
    
    // 各メンバ変数を初期化する
    m_speedX = 0;
    m_speedY = 0;
    m_frame = 0;
    m_state = 0;
    m_scrollSpeed = 0;
    for (int i = 0; i < kAKEnemyWorkCount; i++) {
        m_work[i] = 0;
    }
    m_parentEnemy = NULL;
    m_childEnemy = NULL;
    m_blockHitAction = kAKBlockHitNone;
    while (!m_moveHistory.empty()) {
        m_moveHistory.pop();
    }
    m_isDead = false;
    m_destroyframe = 0;
    setAnimationInitPattern(1);
    
    AKAssert(type > 0 && type <= kAKEnemyDefCount, "敵の種類の値が範囲外:%d", type);
    
    // 動作処理を設定する
    m_action = kAKEnemyDef[type - 1].action;
    
    // 破壊処理を設定する
    m_destroy = kAKEnemyDef[type - 1].destroy;
        
    // 画像名を作成する
    char imageName[16] = "";
    snprintf(imageName, sizeof(imageName), kAKImageNameFormat, kAKEnemyDef[type - 1].image);
    setImageName(imageName);
                
    // アニメーションフレームの個数を設定する
    m_animationPattern = kAKEnemyDef[type - 1].animationFrame;
    
    // アニメーションフレーム間隔を設定する
    m_animationInterval = kAKEnemyDef[type - 1].animationInterval;
    
    // 当たり判定のサイズを設定する
    m_size.width = kAKEnemyDef[type - 1].hitWidth;
    m_size.height = kAKEnemyDef[type - 1].hitHeight;
    
    // 当たり判定のオフセットを設定する
    m_offset.x = kAKEnemyDef[type - 1].offsetX;
    m_offset.y = kAKEnemyDef[type - 1].offsetY;
    
    // 移動履歴を保存する個数を設定する
    m_historyCount = kAKEnemyDef[type - 1].historyCount;
    
    // ヒットポイントを設定する
    m_hitPoint = kAKEnemyDef[type - 1].hitPoint;
    
    // 防御力を設定する
    m_defence = kAKEnemyDef[type - 1].defence;
    
    // スコアを設定する
    m_score = kAKEnemyDef[type - 1].score;
    
    // ボスフラグを設定する
    if (kAKEnemyDef[type - 1].boss != 0) {
        m_boss = true;
    }
    else {
        m_boss = false;
    }
    
    // 画像の回転をリセットする
    getImage()->setRotation(0.0f);

    // 画面外判定のサイズを設定する
    m_outThreshold = m_size.width;
    
    // レイヤーに配置する
    parent->addChild(getImage());
}

/*!
 @brief 親キャラクター設定
 
 親キャラクターを設定する。
 @param parent 親キャラクター
 */
void AKEnemy::setParentEnemy(AKEnemy *parent)
{
    m_parentEnemy = parent;
}

/*!
 @brief 小キャラクター設定
 
 小キャラクターを設定する。
 @param child 小キャラクター
 */
void AKEnemy::setChildEnemy(AKEnemy *child)
{
    m_childEnemy = child;
}

/*!
 @brief 状態設定
 
 状態を設定する。
 @param state 状態
 */
void AKEnemy::setState(int state)
{
    m_state = state;
}

/*!
 @brief 移動履歴取得
 
 移動履歴を取得する。
 @return 移動履歴
 */
const std::queue<cocos2d::Vec2>* AKEnemy::getMoveHistory()
{
    return &m_moveHistory;
}

/*!
 @brief ボスキャラかどうかを取得する
 
 ボスキャラかどうかを取得する。
 @return ボスキャラかどうか
 */
bool AKEnemy::isBoss() const
{
    return m_boss;
}

/*!
 @brief キャラクター固有の動作

 生成時に指定されたセレクタを呼び出す。
 @param data ゲームデータ
 */
void AKEnemy::action(AKPlayDataInterface *data)
{
    // 動作開始からのフレーム数をカウントする
    m_frame++;
    
    // 移動履歴を保存する個数が設定されている場合は履歴を保存する
    if (m_historyCount > 0) {
        
        // 個数を超えている分を破棄する
        while (m_moveHistory.size() >= m_historyCount) {
            m_moveHistory.pop();
        }
        
        // 前回位置を保存する
        m_moveHistory.push(m_prevPosition);
    }
        
    // 敵種別ごとの処理を実行
    (this->*m_action)(data);
}

/*!
 @brief 破壊処理

 HPが0になったときに敵種別固有の破壊処理を呼び出す。
 @param data ゲームデータ
 */
void AKEnemy::destroy(AKPlayDataInterface *data)
{
    // 破壊開始1回目の場合
    if (m_destroyframe == 0) {
        
        // スコアを加算する
        data->addScore(m_score);
        
        // チキンゲージを増やす
        data->addChickenGauge(kAKIncrementChickenGauge);
    }
    
    // 破壊開始からの経過フレーム数をカウントする
    m_destroyframe++;
    
    // 死亡フラグを立てる
    m_isDead = true;
    
    // 敵種別ごとの処理を実行
    if (this->m_destroy != NULL) {
        (this->*m_destroy)(data);
    }
    
    // 死亡フラグが立っている場合は進行度を進め、スーパークラスの処理を行う
    // 死んだあと画面からすぐに消えない場合は敵種別ごとの処理でフラグを落とす
    if (m_isDead) {

        // 進行度を進める
        data->addProgress(m_progress);
        
        // スーパークラスの処理を行う（ステージから取り除かれる）
        AKCharacter::destroy(data);
    }
}

/*!
 @brief トンボの動作処理
 
 まっすぐ進む。一定間隔で左方向へ1-way弾発射。
 @param data ゲームデータ
 */
void AKEnemy::actionOfDragonfly(AKPlayDataInterface *data)
{
    // 弾のスピード
    const float kAKShotSpeed = 2.0f;
    // 移動スピード
    const float kAKMoveSpeed = -1.5f;
    // 弾発射間隔
    const int kAKShotInterval = 120;

    // 左へ直進する
    m_speedX = kAKMoveSpeed;
    m_speedY = 0.0f;
    
    // 一定時間経過しているときは左方向へ1-way弾を発射する
    if ((m_frame + 1) % kAKShotInterval == 0) {
        
        // 左へ弾を発射する
        AKEnemy::fireNWay(M_PI,
                          Vec2(m_position.x, m_position.y),
                          1,
                          0.0f,
                          kAKShotSpeed,
                          false,
                          data);
    }
}

/*!
 @brief アリの動作処理
 
 天井または地面に張り付いて歩く。
 
 初期状態:上下どちらに張り付くか判定する。距離の近い方に張り付く。
 天井に張り付く場合は画像を上下反転する。
 
 左移動:左方向への移動。一定時間経過後に弾発射に遷移する。
 
 弾発射:停止して弾の発射。自機に向かって1-wayを一定数発射する。
 一定時間経過後に右移動に遷移する。
 
 右移動:地面右方向への移動。一定時間経過後に弾発射に遷移する。
 
 @param data ゲームデータ
 */
void AKEnemy::actionOfAnt(AKPlayDataInterface *data)
{
    // 弾のスピード
    const float kAKShotSpeed = 1.5f;
    // 移動スピード
    const float kAKMoveSpeed = 1.0f;
    // 移動するフレーム数
    const int kAKMoveFrame = 120;
    // 弾発射間隔
    const int kAKShotInterval = 30;
    // 弾発射時間
    const int kAKShotFrame = 120;
    
    // 状態
    enum STATE {
        kAKStateInit = 0,           // 初期状態
        kAKStateLeftMove,           // 左移動
        kAKStateRightMove,          // 右移動
        kAKStateFire                // 弾発射
    };

    // 縦方向の速度は状態にかかわらず0とする
    m_speedY = 0.0f;
    
    // スクロールに合わせて移動する
    m_scrollSpeed = 1.0f;
    
    // 状態によって処理を分岐する
    switch (m_state) {
        case kAKStateInit:     // 初期状態
            
            // 逆さま判定を行う
            checkReverse(*data->getBlocks());
            
            // 左移動に遷移する
            m_state = kAKStateLeftMove;
            
            break;
            
        case kAKStateLeftMove:  // 左移動
            
            // スピードをマイナスにして、左右反転はなしにする
            m_speedX = -kAKMoveSpeed;
            getImage()->setFlippedX(false);
            
            // 移動フレーム数が経過したら弾発射に遷移する
            if ((m_frame - m_work[0] + 1) % kAKMoveFrame == 0) {

                m_state = kAKStateFire;
                
                // 作業領域に状態遷移した時のフレーム数を保存する
                m_work[0] = m_frame;
            }

            break;

        case kAKStateRightMove: // 右移動
            
            // スピードをプラスにして、左右反転はありにする
            m_speedX = kAKMoveSpeed;
            getImage()->setFlippedX(true);
            
            // 移動フレーム数が経過したら弾発射に遷移する
            if ((m_frame - m_work[0] + 1) % kAKMoveFrame == 0) {

                m_state = kAKStateFire;
                
                // 作業領域に状態遷移した時のフレーム数を保存する
                m_work[0] = m_frame;
            }
            
            break;
                        
        case kAKStateFire:  // 弾発射
            
            // 自分より右側に自機がいれば左右反転する
            if (m_position.x < data->getPlayerPosition()->x) {
                getImage()->setFlippedX(true);
            }
            else {
                getImage()->setFlippedX(false);
            }

            // 待機する
            m_speedX = 0.0f;
            
            // 弾発射時間が経過している場合は右移動へ遷移する
            if ((m_frame - m_work[0] + 1) % kAKShotFrame == 0) {
                
                m_state = kAKStateRightMove;
                
                // 作業領域に状態遷移した時のフレーム数を保存する
                m_work[0] = m_frame;
                
            }
            // 弾発射間隔が経過している場合は弾を発射する
            else if ((m_frame - m_work[0] + 1) % kAKShotInterval == 0) {
                
                // 自機へ向けて弾を発射する
                AKEnemy::fireNWay(m_position,
                                  1,
                                  0.0f,
                                  kAKShotSpeed,
                                  data);
            }
            
            break;
            
        default:
            AKAssert(false, "状態が異常:m_state=%d", m_state);
            break;
    }
        
    // 障害物との衝突判定を行う
    m_position = AKEnemy::checkBlockPosition(m_position,
                                             getImage()->getContentSize(),
                                             getImage()->isFlippedY(),
                                             data);
    
    // 画像表示位置の更新を行う
    updateImagePosition();
}


/*!
 @brief チョウの動作処理
 
 上下に斜めに移動しながら左へ進む。
 定周期で左方向へ3-way弾を発射する。
 @param data ゲームデータ
 */
void AKEnemy::actionOfButterfly(AKPlayDataInterface *data)
{
    // 状態
    enum STATE {
        kAKStateInit = 0,   // 初期状態
        kAKStateUpMove,     // 上移動
        kAKStateDownMove    // 下移動
    };

    // 上下方向転換の間隔
    const int kAKChangeDirectionInterval = 50;
    // 弾発射間隔
    const int kAKShotInterval = 60;
    // x方向の移動スピード
    const float kAKMoveSpeedX = -1.0f;
    // y方向の移動スピード
    const float kAKMoveSpeedY = 2.0f;
    // 弾のスピード
    const float kAKShotSpeed = 2.0f;
    
    // 初期状態の場合
    if (m_state == kAKStateInit) {

        // 左方向への速度を決める
        m_speedX = kAKMoveSpeedX;
        
        // 最初は上向きの移動とする
        m_state = kAKStateUpMove;
    }
    
    // 上下方向転換の間隔経過している場合は上下の移動方向の状態を逆にする
    if ((m_frame + 1) % kAKChangeDirectionInterval == 0) {
        
        if (m_state == kAKStateUpMove) {
            m_state = kAKStateDownMove;
        }
        else {
            m_state = kAKStateUpMove;
        }
    }
    
    // 状態に応じて上下の移動方向を決める
    if (m_state == kAKStateUpMove) {
        m_speedY = kAKMoveSpeedY;
    }
    else {
        m_speedY = -kAKMoveSpeedY;
    }
    
    // 弾発射間隔経過しているときは左方向へ3-way弾を発射する
    if ((m_frame + 1) % kAKShotInterval == 0) {
        
        // 左へ弾を発射する
        AKEnemy::fireNWay(M_PI,
                          m_position,
                          3,
                          M_PI / 8.0f,
                          kAKShotSpeed,
                          false,
                          data);
    }
}

/*!
 @brief テントウムシの動作処理
 
 まっすぐ進む。一定間隔で自機を狙う1-way弾発射。
 @param data ゲームデータ
 */
void AKEnemy::actionOfLadybug(AKPlayDataInterface *data)
{
    // 弾発射間隔
    const int kAKShotInterval = 60;
    // 移動スピード
    const float kAKMoveSpeed = -1.3f;
    // 弾のスピード
    const float kAKShotSpeed = 1.5f;

    // 左へ直進する
    m_speedX = kAKMoveSpeed;
    m_speedY = 0.0f;
    
    // 弾発射間隔経過しているときは自機を狙う1-way弾を発射する
    if ((m_frame + 1) % kAKShotInterval == 0) {
        
        // 自機へ向けて弾を発射する
        AKEnemy::fireNWay(m_position,
                          1,
                          0.0f,
                          kAKShotSpeed,
                          data);        
    }
}

/*!
 @brief ミノムシの動作処理
 
 スクロールスピードに合わせて移動する。一定時間で全方位に12-way弾を発射する。
 @param data ゲームデータ
 */
void AKEnemy::actionOfBagworm(AKPlayDataInterface *data)
{
    // 弾発射間隔
    const int kAKShotInterval = 60;
    // 弾のスピード
    const float kAKShotSpeed = 1.5f;

    // スクロールに合わせて移動する
    m_scrollSpeed = 1.0f;
    
    // 弾発射間隔経過しているときは全方位に弾を発射する
    if ((m_frame + 1) % kAKShotInterval == 0) {
        
        // 全方位に 12-way弾を発射する
        AKEnemy::fireNWay(M_PI,
                          m_position,
                          12,
                          M_PI / 6.0f,
                          kAKShotSpeed,
                          true,
                          data);
    }
}

/*!
 @brief セミの動作処理
 
 自機に向かって一定時間飛ぶ。その後待機して自機に向かって3-way弾を発射する。
 @param data ゲームデータ
 */
void AKEnemy::actionOfCicada(AKPlayDataInterface *data)
{
    // 移動スピード
    const float kAKSpeed = 2.0f;
    // 移動間隔
    const int kAKMoveInterval = 60;
    // 弾発射間隔
    const int kAKShotInterval = 20;
    // 待機間隔
    const int kAKWaitInterval = 70;
    // 弾のスピード
    const float kAKShotSpeed = 1.5f;

    // 状態
    enum STATE {
        kAKStateInit = 0,           // 初期状態
        kAKStateLeftMove,           // 左移動
        kAKStateFire                // 弾発射
    };

    // 状態によって処理を分岐する
    switch (m_state) {
        case kAKStateInit:     // 初期状態
        {
            // 自機との角度を求める
            float angle = AKAngle::calcDestAngle(m_position, *data->getPlayerPosition());
            
            // 縦横の速度を決定する
            m_speedX = kAKSpeed * cosf(angle);
            m_speedY = kAKSpeed * sinf(angle);
            
            // 移動中はアニメーションを設定する
            m_animationPattern = 2;
            m_animationInterval = 6;
            setAnimationInitPattern(11);
            
            // 左移動の状態へ遷移する
            m_state = kAKStateLeftMove;            
        }
            break;
            
        case kAKStateLeftMove:     // 左へ移動
            
            // 移動間隔経過したら次の状態へ進める
            if ((m_frame + 1) % kAKMoveInterval == 0) {
                
                // 弾発射の状態へ遷移する
                m_state= kAKStateFire;
                
                // 停止する（スクロールスピードに合わせる）
                m_speedX = -data->getScrollSpeedX();
                m_speedY = -data->getScrollSpeedY();
                
                // 待機中はアニメーションを無効化
                m_animationPattern = 1;
                m_animationInterval = 0.0f;
                setAnimationInitPattern(1);
                
                // 経過フレーム数を初期化する
                m_frame = 0;
            }
            break;
            
        case kAKStateFire:  // 弾発射
            
            // 弾発射間隔経過で自機に向かって3-way弾を発射する
            if ((m_frame + 1) % kAKShotInterval == 0) {
                
                AKEnemy::fireNWay(m_position,
                                  3,
                                  M_PI / 8.0f,
                                  kAKShotSpeed,
                                  data);                
            }
            
            // 待機間隔経過で初期状態に戻る
            if ((m_frame + 1) % kAKWaitInterval == 0) {
                
                // 初期状態へ遷移する
                m_state = kAKStateInit;
                
                // 経過フレーム数を初期化する
                m_frame = 0;
            }
            
            break;
            
        default:
            AKAssert(false, "状態が異常:m_state=%d", m_state);
            break;
    }
}

/*!
 @brief バッタの動作処理
 
 地面または天井を移動する。左方向へジャンプ、着地して弾発射を繰り返す。
 @param data ゲームデータ
 */
void AKEnemy::actionOfGrasshopper(AKPlayDataInterface *data)
{
    // 移動スピード
    const float kAKMoveSpeed = -1.0f;
    // ジャンプのスピード
    const float kAKJumpSpeed = 4.0f;
    // 重力加速度
    const float kAKGravitationAlacceleration = 0.15f;
    // 左移動間隔
    const int kAKLeftMoveInterval = 60;
    // 待機間隔
    const int kAKWaitInterval = 60;
    // 弾発射間隔
    const int kAKShotInterval = 30;
    // 弾のスピード
    const float kAKShotSpeed = 1.5f;
    
    // 状態
    enum STATE {
        kAKStateInit = 0,           // 初期状態
        kAKStateLeftMove,           // 左移動
        kAKStateWait                // 待機
    };

    // スクロールに合わせて移動する
    m_scrollSpeed = 1.0f;
    
    // 障害物との当たり判定を有効にする
    m_blockHitAction = kAKBlockHitMove;

    // 初期状態の時は逆さま判定を行う
    if (m_state == kAKStateInit) {
        checkReverse(*data->getBlocks());
    }
    
    // 左移動中の場合
    if (m_state == kAKStateLeftMove) {

        // 重力加速度をかけて減速する
        if (!getImage()->isFlippedY()) {
            m_speedY -= kAKGravitationAlacceleration;
        }
        else {
            m_speedY += kAKGravitationAlacceleration;
        }

        // 一定時間経過したら次の状態へ進める
        if (m_frame - m_work[0] > kAKLeftMoveInterval) {
            
            // 弾発射の状態へ遷移する
            m_state = kAKStateWait;
            
            // 停止する
            m_speedX = 0;
            
            // 作業領域に状態遷移した時のフレーム数を保存する
            m_work[0] = m_frame;
        }
    }
    
    // 落ちていく方向の障害物に接触している場合、着地したとしてスピードを0にする。
    if ((!getImage()->isFlippedY() && (m_blockHitSide & kAKHitSideBottom)) ||
        (getImage()->isFlippedY() && (m_blockHitSide & kAKHitSideTop))) {
        
        m_speedX = 0.0f;
        m_speedY = 0.0f;
    }
    
    // 初期状態または待機中で待機時間が経過している場合
    if ((m_state == kAKStateInit) || (m_state == kAKStateWait && (m_frame - m_work[0] > kAKWaitInterval))) {
        
        AKLog(kAKLogEnemy_3, "ジャンプ:frame=%d work[0]=%d", m_frame, m_work[0]);
        
        // 左方向へのスピードを設定する
        m_speedX = kAKMoveSpeed;
        
        // ジャンプする方向へ加速する
        if (!getImage()->isFlippedY()) {
            m_speedY = kAKJumpSpeed;
        }
        else {
            m_speedY = -kAKJumpSpeed;
        }
        
        // 左移動の状態へ遷移する
        m_state = kAKStateLeftMove;
        
        // 作業領域に状態遷移した時のフレーム数を保存する
        m_work[0] = m_frame;
    }

    // 弾発射間隔経過で自機に向かって1-way弾を発射する
    if ((m_frame + 1) % kAKShotInterval == 0) {
        
        AKEnemy::fireNWay(m_position,
                          1,
                          0.0f,
                          kAKShotSpeed,
                          data);
    }
    
    AKLog(kAKLogEnemy_3, "speed=(%f, %f)", m_speedX, m_speedY);
    
    // 画像表示位置の更新を行う
    updateImagePosition();
}

/*!
 @brief ハチの動作処理
 
 画面上半分から登場するときは左下に向かって一定時間進み、一時停止して弾を発射、その後左上に向かって飛んで行く。
 画面下半分から登城するときは左上に向かって一定時間進み、あとの処理は同じ。
 弾は5種類のスピードの弾を左方向に発射する。
 @param data ゲームデータ
 */
void AKEnemy::actionOfHornet(AKPlayDataInterface *data)
{
    // 弾の数
    const int kAKShotCount = 5;
    // 弾のスピード
    const float kAKShotSpeed[kAKShotCount] = {2.2f, 2.4f, 2.6f, 2.8f, 3.0f};
    // 登場時のx方向の移動スピード
    const float kAKMoveInSpeedX = -2.0f;
    // 退場時のx方向の移動スピード
    const float kAKMoveOutSpeedX = -2.6f;
    // 退場時のy方向の移動スピード
    const float kAKMoveOutSpeedY = 0.5f;
    // 登場時移動フレーム数
    const int kAKMoveInFrame = 30;
    // 弾発射までの待機フレーム数
    const int kAKWaitForFireFrame = 6;
    // 退場までの待機フレーム数
    const int kAKWaitForMoveOutFrame = 12;
    
    // 状態
    enum STATE {
        kAKStateInit = 0,   // 初期状態
        kAKStateMoveIn,     // 登場
        kAKStateFire,       // 弾発射
        kAKStateMoveOut     // 退場
    };
    
    // 状態によって処理を分岐する
    switch (m_state) {
        case kAKStateInit:      // 初期状態
            
            // 左方向へ移動する
            m_speedX = kAKMoveInSpeedX;
            
            // 自機に向かってy軸方向は移動する
            m_speedY = (data->getPlayerPosition()->y - m_position.y) / kAKMoveInFrame;
            
            // 登場の状態へ遷移する
            m_state = kAKStateMoveIn;
            
            break;
            
        case kAKStateMoveIn:    // 登場
            
            // 登場移動時間が経過している場合
            if (m_frame > kAKMoveInFrame) {
                
                // 停止する
                m_speedX = 0.0f;
                m_speedY = 0.0f;
                
                // 弾発射の状態へ遷移する
                m_state = kAKStateFire;
                
                // 経過フレーム数を初期化する
                m_frame = 0;
            }
            
            break;
            
        case kAKStateFire:      // 弾発射
            
            // 待機時間が経過している場合
            if (m_frame > kAKWaitForFireFrame) {
                
                // 左へ5種類の弾を発射する
                for (int i = 0; i < kAKShotCount; i++) {
                    AKEnemy::fireNWay(M_PI,
                                      m_position,
                                      3,
                                      M_PI / 32.0f,
                                      kAKShotSpeed[i],
                                      false,
                                      data);
                }

                // 退場の状態へ遷移する
                m_state = kAKStateMoveOut;
                
                // 経過フレーム数を初期化する
                m_frame = 0;
            }
            break;
            
        case kAKStateMoveOut:   // 退場
            
            // 待機時間が経過している場合
            if (m_frame > kAKWaitForMoveOutFrame) {
                // 左上へ移動する
                m_speedX = kAKMoveOutSpeedX;
                m_speedY = kAKMoveOutSpeedY;
            }
            
            break;
            
        default:
            AKAssert(false, "状態が異常:m_state=%d", m_state);
            break;
    }
}

/*!
 @brief ゴキブリの動作処理
 
 自機に向かって体当たりをしてくる。定周期で自機に向かって1-way弾を発射する。
 @param data ゲームデータ
 */
void AKEnemy::actionOfCockroach(AKPlayDataInterface *data)
{
    // 移動スピード
    const float kAKMoveSpeed = 1.5f;
    // 弾のスピード
    const float kAKShotSpeed = 2.0f;
    // 弾発射間隔
    const int kAKShotInterval = 20;

    // 障害物との当たり判定を有効にする
    m_blockHitAction = kAKBlockHitMove;

    // 自機との角度を求める
    float angle = AKAngle::calcDestAngle(m_position, *data->getPlayerPosition());
    
    // 縦横の速度を決定する
    m_speedX = kAKMoveSpeed * cosf(angle);
    m_speedY = kAKMoveSpeed * sinf(angle);
    
    // 画像を回転させる
    getImage()->setRotation(AKAngle::convertAngleRad2Scr(angle));
    
    // 一定時間経過しているときは自機を狙う1-way弾を発射する
    if ((m_frame + 1) % kAKShotInterval == 0) {
        
        // 自機へ向けて弾を発射する
        AKEnemy::fireNWay(m_position,
                          1,
                          0.0f,
                          kAKShotSpeed,
                          data);        
    }
}

/*! 
 @brief カタツムリの動作処理
 
 天井または地面に張り付いて歩く。
 左方向へゆっくり移動しながら、自機に向かって3-way弾を発射する。
 @param data ゲームデータ
 */
void AKEnemy::actionOfSnail(AKPlayDataInterface *data)
{
    // 弾のスピード
    const float kAKShotSpeed = 1.5f;
    // 移動スピード
    const float kAKMoveSpeed = 0.2f;
    // 弾発射間隔
    const int kAKShotInterval = 60;
    
    // 状態
    enum STATE {
        kAKStateInit = 0,           // 初期状態
        kAKStateLeftMove            // 左移動
    };
    
    // 縦方向の速度は状態にかかわらず0とする
    m_speedY = 0.0f;
    
    // スクロールに合わせて移動する
    m_scrollSpeed = 1.0f;
    
    // 状態によって処理を分岐する
    switch (m_state) {
        case kAKStateInit:     // 初期状態
            
            // 逆さま判定を行う
            checkReverse(*data->getBlocks());
            
            // 左方向へ移動する
            m_speedX = -kAKMoveSpeed;
            
            // 左移動の状態に遷移する
            m_state = kAKStateLeftMove;
            
            break;
            
        case kAKStateLeftMove:  // 左移動
                        
            // 一定時間経過したら弾を発射する
            if ((m_frame + 1) % kAKShotInterval == 0) {

                // 自機へ向けて弾を発射する
                AKEnemy::fireNWay(m_position,
                                  3,
                                  M_PI / 8.0f,
                                  kAKShotSpeed,
                                  data);
            }
            
            break;
        default:
            AKAssert(false, "状態が異常:m_state=%d", m_state);
            break;
    }
    
    // 障害物との衝突判定を行う
    m_position = AKEnemy::checkBlockPosition(m_position,
                                             getImage()->getContentSize(),
                                             getImage()->isFlippedY(),
                                             data);
    
    // 画像表示位置の更新を行う
    updateImagePosition();
}

/*!
 @brief クワガタの動作処理
 
 登場時に自機の方向へ進行方向を決めて真っ直ぐ飛ぶ。定周期で3-way弾を発射する。
 途中で方向転換を行い、自機の方向へ向き直す。ただし、後ろの方向には戻らないようにx方向は常に左にする。
 @param data ゲームデータ
 */
void AKEnemy::actionOfStagBeetle(AKPlayDataInterface *data)
{
    // 弾のスピード
    const float kAKShotSpeed = 2.0f;
    // 移動スピード
    const float kAKMoveSpeed = 1.5f;
    // 弾発射間隔
    const int kAKShotInterval = 60;
    // 方向転換を行うまでの間隔
    const int kAKChangeDirectionInterval = 120;

    // 状態
    enum STATE {
        kAKStateInit = 0,           // 初期状態
        kAKStateLeftMove            // 左移動
    };
    
    // 状態によって処理を分岐する
    switch (m_state) {
        case kAKStateInit:              // 初期状態
        {
            // 自機との角度を求める
            float angle = AKAngle::calcDestAngle(m_position,
                                                 *data->getPlayerPosition());
            
            // 縦横の速度を決定する
            // ただし、後ろには戻らないようにx方向は絶対値とする
            m_speedX = -1.0 * fabsf(kAKMoveSpeed * cosf(angle));
            m_speedY = kAKMoveSpeed * sinf(angle);
            
            // 左移動の状態へ遷移する
            m_state = kAKStateLeftMove;
        }
            break;
            
        case kAKStateLeftMove:     // 左へ移動
            
            // 弾発射間隔時間経過したら弾を発射する
            if ((m_frame + 1) % kAKShotInterval == 0) {
                
                // 自機へ向けて弾を発射する
                AKEnemy::fireNWay(m_position,
                                  3,
                                  M_PI / 8.0f,
                                  kAKShotSpeed,
                                  data);
            }
            
            // 方向転換間隔経過したら初期状態に遷移して角度を設定し直す
            if ((m_frame + 1) % kAKChangeDirectionInterval == 0) {
                
                m_state = kAKStateInit;
            }
            break;
            
        default:
            AKAssert(false, "状態が異常:m_state=%d", m_state);
            break;
    }
    
}

/*!
 @brief カブトムシの動作処理
 
 左に真っすぐ進み、画像がすべて表示できた部分で上下に移動する。
 
 攻撃パターン1:左方向にまっすぐ3-wayと自機を狙う1-way弾を発射する
 
 攻撃パターン2:定周期に一塊の5-way弾を発射する
 
 攻撃パターン3:全方位弾を発射する
 @param data ゲームデータ
 */
void AKEnemy::actionOfRhinocerosBeetle(AKPlayDataInterface *data)
{
    // 状態
    enum STATE {
        kAKStateInit = 0,           // 初期状態
        kAKStateLeftShot,           // 左方向へ弾発射
        kAKStateNWay,               // 自機へ向けてn-way弾発射
        kAKStateAllDirection,       // 全方位に弾発射
        kAKStateCount               // 状態の種類の数
    };
    
    // x座標最小位置
    const float kAKXMin = 300.0f;
    // y座標最小位置
    const float kAKYMin = 80.0f;
    // y座標最大位置
    const float kAKYMax = 210.0f;
    // 移動スピード
    const float kAKMoveSpeed = 1.0f;
    // 状態遷移間隔
    const int kAKStateInterval[kAKStateCount] = {1000000, 900, 900, 900};
    // 左方向への弾の発射間隔
    const int kAKLeftShotInterval = 30;
    // 左方向への弾の上下の発射位置
    const int kAKLeftShotPosition = 30;
    // 左方向への弾のスピード
    const float kAKLeftShotSpeed = 2.0f;
    // 左方向への弾発射時の1-way弾の発射間隔
    const int kAKLeftShot1WayInterval = 60;
    // 左方向への弾発射時の1-way弾のスピード
    const float kAKLeftShot1WaySpeed = 1.5f;
    // n-way弾の塊の発射間隔
    const int kAKNWayGroupShotInterval = 60;
    // n-way弾の発射間隔
    const int kAKNWayShotInterval = 6;
    // n-way弾の発射時間
    const int kAKNWayShotTime = 30;
    // n-way弾の弾数
    const int kAKNWayCount = 3;
    // n-way弾の角度の間隔
    const float kAKNwayAngle = M_PI / 8;
    // n-way弾のスピード
    const float kAKNWaySpeed = 2.0f;
    // 全方位弾の発射間隔
    const int kAKAllDirectionInterval = 30;
    // 全方位弾の弾数
    const float kAKAllDirectionCount = 12;
    // 全方位弾の角度の間隔
    const float kAKAllDirectionAngle = 2 * M_PI / kAKAllDirectionCount;
    // 全方位弾のスピード
    const float kAKAllDirectionSpeed = 2.0f;
    // 2周目追加ショット発射間隔
    const int kAK2ndLoopShotInterval = 120;
    // 2周目追加ショットスピード
    const float kAK2ndLoopShotSpeed = 1.0f;
    
    // x座標最小位置まで左に真っ直ぐ移動する
    if (m_position.x > kAKXMin) {
        m_speedX = -kAKMoveSpeed;
        m_speedY = 0.0f;
    }
    // x座標最小位置まで到達して上下に移動していない場合は横方向の移動を止めて、下方向に移動する
    else if (AKIsEqualFloat(m_speedY, 0.0f)) {
        m_speedX = 0.0f;
        m_speedY = -kAKMoveSpeed;
    }
    // 下方向へ移動中にy座標最小位置まで到達したら上方向へ移動する
    else if (m_speedY < 0.0f && m_position.y < kAKYMin) {
        m_speedY = kAKMoveSpeed;
    }
    // 上方向へ移動中にy座標最大位置まで到達したら下方向へ移動する
    else if (m_speedY > 0.0f && m_position.y > kAKYMax) {
        m_speedY = -kAKMoveSpeed;
    }
    // その他の場合はスピードはそのままとする
    else {
        // No operation.
    }
        
    // 状態によって処理を分岐する
    switch (m_state) {
        case kAKStateInit:  // 初期状態
            
            // 上下移動になった場合は次の状態へ進める
            if (AKIsEqualFloat(m_speedX, 0.0f)) {
                m_state++;                
            }
            
            break;
            
        case kAKStateLeftShot:  // 左方向へ弾発射
            
            // 左方向への弾発射間隔が経過している場合は弾を発射する
            if ((m_frame + 1) % kAKLeftShotInterval == 0) {
                
                // 3箇所から同時に弾を発射する
                Vec2 position(m_position.x, m_position.y);
                AKEnemy::fireNWay(M_PI,
                                  position,
                                  1,
                                  0.0f,
                                  kAKLeftShotSpeed,
                                  false,
                                  data);

                position.y = m_position.y - kAKLeftShotPosition;
                AKEnemy::fireNWay(M_PI,
                                  position,
                                  1,
                                  0.0f,
                                  kAKLeftShotSpeed,
                                  false,
                                  data);

                position.y = m_position.y + kAKLeftShotPosition;
                AKEnemy::fireNWay(M_PI,
                                  position,
                                  1,
                                  0.0f,
                                  kAKLeftShotSpeed,
                                  false,
                                  data);
            }
            
            // 1-way弾の弾発射間隔が経過している場合は弾を発射する
            if ((m_frame + 1) % kAKLeftShot1WayInterval == 0) {
                
                AKEnemy::fireNWay(m_position,
                                  1,
                                  0.0f,
                                  kAKLeftShot1WaySpeed,
                                  data);
            }
            
            break;
            
        case kAKStateNWay:      // 自機へ向けてn-way弾発射
            
            // n-way弾グループの発射間隔が経過している場合はn-way弾を発射し始める
            if (m_frame - m_work[0] > kAKNWayGroupShotInterval) {
                
                // n-way弾の発射間隔が経過している場合は弾を発射する
                if ((m_frame + 1) % kAKNWayShotInterval == 0) {
                    
                    AKEnemy::fireNWay(m_position,
                                      kAKNWayCount,
                                      kAKNwayAngle,
                                      kAKNWaySpeed,
                                      data);
                }
                
                // n-way弾の発射時間が経過している場合は作業領域に現在の経過フレーム数を入れて
                // n-way弾グループの発射間隔分待機する
                if (m_frame - m_work[0] > kAKNWayShotTime + kAKNWayGroupShotInterval) {
                    m_work[0] = m_frame;
                }
            }
            break;
            
        case kAKStateAllDirection:  // 全方位に弾発射
            
            // 全方位弾の発射間隔が経過している場合は弾を発射する
            if ((m_frame + 1) % kAKAllDirectionInterval == 0) {
                
                AKEnemy::fireNWay(M_PI,
                                  m_position,
                                  kAKAllDirectionCount,
                                  kAKAllDirectionAngle,
                                  kAKAllDirectionSpeed,
                                  true,
                                  data);
            }
            break;
            
        default:
            AKAssert(false, "状態が異常:m_state=%d", m_state);
            break;
    }
    
    // 2周目時は3-way弾を発射する
    if (data->is2ndLoop()) {
        
        // 弾発射状態の場合
        if (m_state != kAKStateInit) {
            
            // 2周目追加ショット発射間隔が経過している場合は弾を発射する
            if ((m_frame + 1) % kAK2ndLoopShotInterval == 0) {
                
                AKEnemy::fireNWay(m_position,
                                  3,
                                  M_PI / 8.0f,
                                  kAK2ndLoopShotSpeed,
                                  data);
            }
        }
    }
    
    // 状態遷移間隔が経過している場合は次の状態へ進める
    if (m_frame > kAKStateInterval[m_state]) {
        
        // 次の状態へ進める
        m_state++;
        
        // 状態が最大を超える場合は最初の状態へループする
        if (m_state >= kAKStateCount) {
            m_state = kAKStateInit + 1;
        }
        
        // 経過フレーム数と作業領域を初期化する
        m_work[0] = 0.0f;
        m_frame = 0.0f;
        
        AKLog(kAKLogEnemy_3, "m_state=%d", m_state);
    }
}

/*!
 @brief カマキリの動作処理
 
 スクロールに応じて移動する。攻撃パターンの状態にかかわらず、常に3-way弾を発射する。
 攻撃の前に鎌を振り上げ、攻撃と同時に鎌を片方ずつ下ろすアニメーションを行う。
 
 攻撃パターン1:弧の形に並んだ弾を自機に向けて発射する。
 
 攻撃パターン2:塊弾を自機に向けて発射する。自機の位置に到達すると塊弾は12-way弾として弾ける。
 
 攻撃パターン3:9-way弾と10-way弾を短い間隔で連続で発射する。
 @param data ゲームデータ
 */
void AKEnemy::actionOfMantis(AKPlayDataInterface *data)
{
    // 状態
    enum STATE {
        kAKStateInit = 0,           // 初期状態
        kAKStateArcShot,            // 弧形弾発射
        kAKStateBurstShot,          // 破裂弾発射
        kAKStateNWayShot,           // n-way弾発射
        kAKStateCount               // 状態の種類の数
    };
    // 地面の高さ
    const int kAKGround = 24;
    // 状態遷移間隔
    const int kAKStateInterval[kAKStateCount] = {340, 900, 900, 900};
    // 手の位置
    const Vec2 kAKHandPosition(-48.0f, 48.0f);
    // 定周期弾の発射間隔
    const int kAKCycleShotInterval = 60;
    // 定周期弾の弾数
    const int kAKCycleShotCount = 3;
    // 定周期弾の角度の間隔
    const float kAKCycleShotAngle = M_PI / 8;
    // 定周期弾のスピード
    const float kAKCycleShotSpeed = 2.0f;
    // 弧形段の弾数
    const int kAKArcShotCount = 9;
    // 弧形弾の配置位置
    const float kAKArcShotPosition[kAKArcShotCount][2] = {
        {0, 0}, {6, 12}, {14, 24}, {24, 34}, {36, 42}, {-2, -14}, {0, -28}, {4, -42}, {12, -54}
    };
    // 弧形弾の発射待機時間
    const int kAKArcShotWaitTime = 90;
    // 弧形弾の発射間隔
    const int kAKArcShotInterval = 60;
    // 弧形弾のスピード
    const float kAKArcShotSpeed = 2.0f;
    // 破裂弾の発射待機時間
    const int kAKBurstShotWaitTime = 90;
    // 破裂弾の発射間隔
    const int kAKBurstShotInterval = 60;
    // 破裂弾の破裂前のスピード
    const float kAKBurstShotBeforSpeed = 1.5f;
    // 破裂弾の破裂後のスピード
    const float kAKBurstShotAfterSpeed = 2.5f;
    // 破裂弾の破裂までの間隔
    const int kAKBurstShotBurstInterval = 80;
    // 破裂弾の弾数
    const int kAKBurstShotCount = 12;
    // 破裂弾の各弾の間隔
    const float kAKBurstShotAngleInterval = M_PI / 6.0f;
    // n-way弾の待機間隔
    const int kAKNWayShotWaitTime = 90;
    // n-way弾の発射間隔
    const int kAKNWayShotInterval = 20;
    // n-way弾のスピード
    const float kAKNWayShotSpeed = 2.0f;
    // n-way弾の弾数
    const int kAKNWayCount[2] = {9, 10};
    // n-way弾の角度の間隔
    const float kAKNWayAngleInterval = M_PI / 20.0f;
    // 2周目追加ショット発射間隔
    const int kAK2ndLoopShotInterval = 120;
    // 2周目追加ショットスピード
    const float kAK2ndLoopShotSpeed = 1.0f;
    
    // 状態によって処理を分岐する
    switch (m_state) {
        case kAKStateInit:      // 初期状態

            // スクロールに合わせて移動する
            m_scrollSpeed = 1.0f;
            
            // 地面の上の位置に高さを補正する
            m_position.y = getImage()->getContentSize().height / 2 + kAKGround;
            
            break;
            
        case kAKStateArcShot:   // 弧形弾発射
            
            // 弧形弾の待機時間が経過したら鎌を振り上げる
            if (m_work[0] == 0 && m_frame - m_work[1] >= kAKArcShotWaitTime) {
                
                // 作業領域0(振り上げている鎌の数)を2とする
                m_work[0] = 2;
                
                // 作業領域1(鎌を動かしてからの経過フレーム数)を現在のフレーム数に設定する
                m_work[1] = m_frame;
            }
            
            // 弧形弾の発射間隔が経過したら弾を発射する
            if (m_work[0] > 0 && m_frame - m_work[1] >= kAKArcShotInterval) {
                
                // 作業領域0(振り上げている鎌の数)を減らす
                m_work[0]--;

                // 作業領域1(鎌を動かしてからの経過フレーム数)を現在のフレーム数に設定する
                m_work[1] = m_frame;
                
                // 弧形弾を発射する
                AKEnemy::fireGroupShot(m_position, 
                                       kAKArcShotPosition,
                                       kAKArcShotCount,
                                       kAKArcShotSpeed,
                                       data);
            }
            
            break;
            
        case kAKStateBurstShot: // 破裂弾発射

            // 破裂弾の待機時間が経過したら鎌を振り上げる
            if (m_work[0] == 0 && m_frame - m_work[1] >= kAKBurstShotWaitTime) {
                
                // 作業領域0(振り上げている鎌の数)を2とする
                m_work[0] = 2;
                
                // 作業領域1(鎌を動かしてからの経過フレーム数)を現在のフレーム数に設定する
                m_work[1] = m_frame;
            }
            
            // 破裂弾の発射間隔が経過したら弾を発射する
            if (m_work[0] > 0 && m_frame - m_work[1] >= kAKBurstShotInterval) {
                
                // 作業領域0(振り上げている鎌の数)を減らす
                m_work[0]--;
                
                // 作業領域1(鎌を動かしてからの経過フレーム数)を現在のフレーム数に設定する
                m_work[1] = m_frame;
                
                // 破裂弾を発射する
                Vec2 position(m_position.x + kAKHandPosition.x,
                                 m_position.y + kAKHandPosition.y);
                AKEnemy::fireBurstShot(position,
                                       kAKBurstShotCount,
                                       kAKBurstShotAngleInterval,
                                       kAKBurstShotBeforSpeed,
                                       kAKBurstShotBurstInterval,
                                       kAKBurstShotAfterSpeed,
                                       data);
            }

            break;
            
        case kAKStateNWayShot:  // n-way弾発射
            
            // n-way弾の待機時間が経過したら鎌を振り上げる
            if (m_work[0] == 0 && m_frame - m_work[1] >= kAKNWayShotWaitTime) {
                
                // 作業領域0(振り上げている鎌の数)を2とする
                m_work[0] = 2;
                
                // 作業領域1(鎌を動かしてからの経過フレーム数)を現在のフレーム数に設定する
                m_work[1] = m_frame;
            }
            
            // n-way弾の発射間隔が経過したら弾を発射する
            if (m_work[0] > 0 && m_frame - m_work[1] >= kAKNWayShotInterval) {
                
                // 作業領域0(振り上げている鎌の数)を減らす
                m_work[0]--;
                
                // 作業領域1(鎌を動かしてからの経過フレーム数)を現在のフレーム数に設定する
                m_work[1] = m_frame;
                
                // n-way弾を発射する
                Vec2 position(m_position.x + kAKHandPosition.x,
                                 m_position.y + kAKHandPosition.y);
                AKEnemy::fireNWay(position,
                                  kAKNWayCount[1 - m_work[0]],
                                  kAKNWayAngleInterval,
                                  kAKNWayShotSpeed,
                                  data);
            }

            break;
            
        default:
            AKAssert(false, "状態が異常:m_state=%d", m_state);
            break;
    }
    
    // 初期状態以外の場合は定周期に3-way弾を発射する
    if (m_state != kAKStateInit && (m_frame + 1) % kAKCycleShotInterval == 0) {

        AKEnemy::fireNWay(m_position,
                          kAKCycleShotCount,
                          kAKCycleShotAngle,
                          kAKCycleShotSpeed,
                          data);
    }
    
    // 2周目時は3-way弾を発射する
    if (data->is2ndLoop()) {
        
        // 弾発射状態の場合
        if (m_state != kAKStateInit) {
            
            // 2周目追加ショット発射間隔が経過している場合は弾を発射する
            if ((m_frame + 1) % kAK2ndLoopShotInterval == 0) {
                
                AKEnemy::fireNWay(m_position,
                                  3,
                                  M_PI / 8.0f,
                                  kAK2ndLoopShotSpeed,
                                  data);
            }
        }
    }
    
    // 振り上げている鎌の数に応じてグラフィックを変更する
    setAnimationInitPattern(m_work[0] + 1);
    
    // 状態遷移間隔が経過している場合は次の状態へ進める
    if (m_frame > kAKStateInterval[m_state]) {
        
        // 登場時は無敵状態なので状態遷移時に防御力を0にする
        m_defence = 0;
        
        // 次の状態へ進める
        m_state++;
        
        // 状態が最大を超える場合は最初の状態へループする
        if (m_state >= kAKStateCount) {
            m_state = kAKStateInit + 1;
        }
        
        // 経過フレーム数と作業領域を初期化する
        m_work[0] = 0.0f;
        m_work[1] = 0.0f;
        m_frame = 0.0f;
        
        AKLog(kAKLogEnemy_3, "m_state=%d", m_state);
    }
}

/*!
 @brief ハチの巣の動作処理
 
 定周期でハチを登場させる。
 
 攻撃パターン1:5-way弾を自機に向けて発射する
 
 攻撃パターン2:全方位弾を角度を変えながら発射する
 
 攻撃パターン3:地面から上方向に画面全体に弾を発射する
 @param data ゲームデータ
 */
void AKEnemy::actionOfHoneycomb(AKPlayDataInterface *data)
{
    // 状態
    enum STATE {
        kAKStateInit = 0,           // 初期状態
        kAKState5WayShot,           // 5-way弾発射
        kAKStateAllDirectionShot,   // 全方位弾発射
        kAKStateAllRangeShot,       // 地面からの画面全体の弾発射
        kAKStateCount               // 状態の種類の数
    };
    // 状態遷移間隔
    const int kAKStateInterval[kAKStateCount] = {340, 900, 900, 900};
    // ハチを呼び出す間隔
    const int kAKCallHornetInterval = 60;
    // ハチの登場位置の数
    const int kAKCallHornetPositionCount = 3;
    // ハチの登場位置x座標
    const float kAKHornetXPosition = 384.0f;
    // ハチの登場位置y座標
    const float kAKHornetYPosition[kAKCallHornetPositionCount] = {200.0f, 144.0f, 88.0f};
    // 5-way弾発射間隔
    const int kAK5WayInterval = 20;
    // 5-way弾スピード
    const float kAK5WayShotSpeed = 1.5f;
    // 全方位弾の発射間隔
    const int kAKAllDirectionInterval = 40;
    // 全方位弾の弾数
    const float kAKAllDirectionCount = 24;
    // 全方位弾の角度の間隔
    const float kAKAllDirectionAngle = 2 * M_PI / kAKAllDirectionCount;
    // 全方位弾のスピード
    const float kAKAllDirectionSpeed = 1.5f;
    // 画面全体弾発射間隔
    const int kAKAllRangeInterval = 30;
    // 画面全体弾の弾数
    const int kAKAllRangeCount = 7;
    // 画面全体弾のスピード
    const float kAKAllRangeSpeed = 1.0f;
    // 画面全体弾の発射位置y座標
    const float kAKAllRangeYPosition = 36.0f;
    // 1-way弾発射間隔
    const int kAK1WayInterval = 20;
    // 1-way弾スピード
    const float kAK1WayShotSpeed = 2.0f;
    // 2周目追加ショット発射間隔
    const int kAK2ndLoopShotInterval = 120;
    // 2周目追加ショットスピード
    const float kAK2ndLoopShotSpeed = 1.0f;
    
    // 状態によって処理を分岐する
    switch (m_state) {
        case kAKStateInit:      // 初期状態
            
            // スクロールに合わせて移動する
            m_scrollSpeed = 1.0f;
            break;
            
        case kAKState5WayShot:  // 5-way弾発射
            
            // 弾発射間隔時間経過したら弾を発射する
            if ((m_frame + 1) % kAK5WayInterval == 0) {
                
                // 自機へ向けて弾を発射する
                AKEnemy::fireNWay(m_position,
                                  5,
                                  M_PI / 8.0f,
                                  kAK5WayShotSpeed,
                                  data);
            }
            
            break;
            
        case kAKStateAllDirectionShot:  // 全方位弾発射
            
            // 全方位弾の発射間隔が経過している場合は弾を発射する
            if ((m_frame + 1) % kAKAllDirectionInterval == 0) {
                
                // 1回毎に発射する角度を変える
                float angle = M_PI;
                if (((m_frame + 1) / kAKAllDirectionInterval) % 2 == 0) {
                    angle += kAKAllDirectionAngle / 2;
                }
                
                AKEnemy::fireNWay(angle,
                                  m_position,
                                  kAKAllDirectionCount,
                                  kAKAllDirectionAngle,
                                  kAKAllDirectionSpeed,
                                  true,
                                  data);
            }

            break;
            
        case kAKStateAllRangeShot:  // 地面からの画面全体の弾発射
            
            // 画面全体弾の発射間隔が経過している場合は弾を発射する
            if ((m_frame + 1) % kAKAllRangeInterval == 0) {

                // 左端の弾の座標を計算する
                float x = AKScreenSize::stageSize().width / (kAKAllRangeCount + 1);
                
                // 各弾を発射する
                for (int i = 0; i < kAKAllRangeCount; i++) {
                    
                    AKEnemy::fireNWay(M_PI / 2.0f,
                                      Vec2(x * (i + 1), kAKAllRangeYPosition),
                                      1,
                                      0.0f,
                                      kAKAllRangeSpeed,
                                      false,
                                      data);
                }
            }
            
            // 1-way弾発射間隔時間経過したら弾を発射する
            if ((m_frame + 1) % kAK1WayInterval == 0) {
                
                // 自機へ向けて弾を発射する
                AKEnemy::fireNWay(m_position,
                                  1,
                                  0,
                                  kAK1WayShotSpeed,
                                  data);
            }

            
            break;
            
        default:
            AKAssert(false, "状態が異常:m_state=%d", m_state);
            break;
    }
    
    // 初期状態以外の場合は定周期にハチを呼ぶ
    if (m_state != kAKStateInit && (m_frame + 1) % kAKCallHornetInterval == 0) {
        
        // ハチの位置を呼ぶたびに切り替える
        int position = ((m_frame + 1) / kAKCallHornetInterval) % kAKCallHornetPositionCount;
        
        // ハチを呼ぶ
        data->createEnemy(kAKEnemyHornet,
                          Vec2(kAKHornetXPosition, kAKHornetYPosition[position]),
                          0);

    }

    // 2周目時は3-way弾を発射する
    if (data->is2ndLoop()) {
        
        // 弾発射状態の場合
        if (m_state != kAKStateInit) {
            
            // 2周目追加ショット発射間隔が経過している場合は弾を発射する
            if ((m_frame + 1) % kAK2ndLoopShotInterval == 0) {
                
                AKEnemy::fireNWay(m_position,
                                  3,
                                  M_PI / 8.0f,
                                  kAK2ndLoopShotSpeed,
                                  data);
            }
        }
    }
    
    // 状態遷移間隔が経過している場合は次の状態へ進める
    if (m_frame > kAKStateInterval[m_state]) {
        
        // 登場時は無敵状態なので状態遷移時に防御力を0にする
        m_defence = 0;
        
        // 次の状態へ進める
        m_state++;
        
        // 状態が最大を超える場合は最初の状態へループする
        if (m_state >= kAKStateCount) {
            m_state = kAKStateInit + 1;
        }
        
        // 経過フレーム数と作業領域を初期化する
        m_work[0] = 0.0f;
        m_work[1] = 0.0f;
        m_frame = 0.0f;
        
        AKLog(kAKLogEnemy_3, "m_state=%d", m_state);
    }
}

/*!
 @brief クモの動作処理

 攻撃パターン1:右上、左下、左上、右下、右上と移動しながら弾を撃つ。
 高速な3-way弾と低速な13-way弾を発射する。
 
 攻撃パターン2:右上、左下、左上、右下、右上と移動しながら弾を撃つ。
 全方位弾と蜘蛛の巣状の弾を発射する。
 
 攻撃パターン3:自機に向けて2-way弾を隙間なく発射。その間に円形のグループ弾を発射する。
 @param data ゲームデータ
 */
void AKEnemy::actionOfSpider(AKPlayDataInterface *data)
{
    // 状態
    enum STATE {
        kAKStateInit = 0,           // 初期状態
        kAKStateNWayShot,           // n-way弾発射
        kAKStateNetShot,            // 蜘蛛の巣弾発射
        kAKStateSiege,              // 2-way弾による包囲弾発射
        kAKStateCount               // 状態の種類の数
    };
    // 作業領域の用途
    enum WORK {
        kAKWorkCurrentPosition = 0, // 現在の移動位置
        kAKWorkMoveTime,            // 移動位置に到達してからの時間
        kAKWorkSiegeAngle           // 包囲弾の現在の角度
    };
    // 状態遷移間隔
    const int kAKStateInterval[kAKStateCount] = {999, 900, 900, 900};
    // 移動位置の数
    const int kAKMovePositionCount = 4;
    // 移動位置
    const float kAKMovePosition[kAKMovePositionCount][2] = {
        {304, 192}, {212, 96}, {212, 192}, {304, 96}
    };
    // 蜘蛛の巣弾発射時の位置
    const float kAKMovePositionOfSiege[2] = {258, 144};
    // 移動スピード
    const float kAKMoveSpeed = 2.0f;
    // 移動位置変更の間隔
    const int kAKMoveInterval = 10;
    // 3-way弾発射間隔
    const int kAK3WayInterval = 40;
    // 3-way弾スピード
    const float kAK3WayShotSpeed = 1.8f;
    // 5-way弾発射間隔
    const int kAK5WayInterval = 40;
    // 5-way弾スピード
    const float kAK5WayShotSpeed = 1.2f;
    // 蜘蛛の巣弾の弾数
    const int kAKNetShotCount = 34;
    // 蜘蛛の巣弾の各弾の配置位置
    const float kAKNetShotPosition[kAKNetShotCount][2] = {
        {0, 6}, {0, 20}, {0, 34}, {0, 48},
        {0, -6}, {0, -20}, {0, -34}, {0, -48},
        {12, 10}, {12, 42}, {12, -10}, {12, -42},
        {-12, 10}, {-12, 42}, {-12, -10}, {-12, -42},
        {22, 18}, {22, 34}, {22, -18}, {22, -34},
        {-22, 18}, {-22, 34}, {-22, -18}, {-22, -34},
        {32, 26}, {38, 14}, {32, -26}, {38, -14},
        {-32, 26}, {-38, 14}, {-32, -26}, {-38, -14},
        {42, 0}, {-42, 0}
    };
    // 蜘蛛の巣弾発射間隔
    const int kAKNetShotInterval = 60;
    // 蜘蛛の巣弾のスピード
    const float kAKNetShotSpeed = 1.5f;
    // 全方位弾の発射間隔
    const int kAKAllDirectionInterval = 30;
    // 全方位弾の弾数
    const float kAKAllDirectionCount = 12;
    // 全方位弾の角度の間隔
    const float kAKAllDirectionAngle = 2 * M_PI / kAKAllDirectionCount;
    // 全方位弾のスピード
    const float kAKAllDirectionSpeed = 1.5f;
    // 包囲弾の発射間隔
    const int kAKSiegeShotInterval = 5;
    // 包囲弾のスピード
    const float kAKSiegeShotSpeed = 2.5f;
    // 包囲弾の初期角度
    const float kAKSiegeShotStartAngle = M_PI * 3.0f / 4.0f;
    // 包囲弾の最終角度
    const float kAKSiegeShotEndAngle = M_PI / 8.0f;
    // 包囲弾の角度変更スピード
    const float kAKSiegeShotAngleSpeed = 0.2f;
    // 包囲弾の1-way弾発射間隔
    const int kAK1WayInterval = 80;
    // 包囲弾の1-way弾スピード
    const float kAK1WayShotSpeed = 0.8f;
    // 包囲弾内のグループ弾発射間隔
    const int kAKGroupShotInterval = 40;
    // 包囲弾内のグループ弾スピード
    const float kAKGroupShotSpeed = 2.5f;
    // 包囲弾内のグループ弾の弾数
    const int kAKGroupShotCount = 4;
    // 包囲弾内のグループ弾の位置
    const float kAKGroupShotPosition[kAKGroupShotCount][2] = {
        {0, 8}, {0, -8}, {8, 0}, {-8, 0}
    };
    // 2周目追加ショット発射間隔
    const int kAK2ndLoopShotInterval = 120;
    // 2周目追加ショットスピード
    const float kAK2ndLoopShotSpeed = 1.0f;
    
    // 状態によって処理を分岐する
    Vec2 nextPosition;
    switch (m_state) {
        case kAKStateInit:      // 初期状態
            
            // 最初は右上に移動する
            nextPosition.setPoint(kAKMovePosition[0][0],
                                  kAKMovePosition[0][1]);
            
            // 移動位置に到達した場合は次の状態に遷移する
            if (AKIsEqualFloat(m_position.x, nextPosition.x) &&
                AKIsEqualFloat(m_position.y, nextPosition.y)) {
                
                m_state++;

                // 経過フレーム数と作業領域を初期化する
                m_frame = 0.0f;
                m_work[kAKWorkMoveTime] = 0;

            }
            
            break;
            
        case kAKStateNWayShot:  // n-way弾発射

            // 次の移動位置を決める
            nextPosition.setPoint(kAKMovePosition[m_work[kAKWorkCurrentPosition]][0],
                                  kAKMovePosition[m_work[kAKWorkCurrentPosition]][1]);
            
            // 弾発射間隔時間経過したら弾を発射する
            if ((m_frame + 1) % kAK5WayInterval == 0) {
                
                // 自機へ向けて弾を発射する
                AKEnemy::fireNWay(m_position,
                                  5,
                                  M_PI / 8.0f,
                                  kAK5WayShotSpeed,
                                  data);
            }
            
            // 弾発射間隔時間経過したら弾を発射する
            if ((m_frame + 1) % kAK3WayInterval == 0) {
                
                // 自機へ向けて弾を発射する
                AKEnemy::fireNWay(m_position,
                                  3,
                                  M_PI / 16.0f,
                                  kAK3WayShotSpeed,
                                  data);
            }

            break;
            
        case kAKStateNetShot:   // 蜘蛛の巣弾発射
            
            // 次の移動位置を決める
            nextPosition.setPoint(kAKMovePosition[m_work[kAKWorkCurrentPosition]][0],
                                  kAKMovePosition[m_work[kAKWorkCurrentPosition]][1]);
            
            // 蜘蛛の巣弾の発射間隔が経過したら弾を発射する
            if ((m_frame + 1) % kAKNetShotInterval == 0) {
                
                // 蜘蛛の巣弾を発射する
                AKEnemy::fireGroupShot(m_position,
                                       kAKNetShotPosition,
                                       kAKNetShotCount,
                                       kAKNetShotSpeed,
                                       data);
            }
            
            // 全方位弾の発射間隔が経過している場合は弾を発射する
            if ((m_frame + 1) % kAKAllDirectionInterval == 0) {
                
                AKEnemy::fireNWay(M_PI,
                                  m_position,
                                  kAKAllDirectionCount,
                                  kAKAllDirectionAngle,
                                  kAKAllDirectionSpeed,
                                  true,
                                  data);
            }

            break;
            
        case kAKStateSiege:     // 2-way弾による包囲弾発射
            
            // 包囲弾発射時は中央で固定とする
            nextPosition.setPoint(kAKMovePositionOfSiege[0],
                                  kAKMovePositionOfSiege[1]);
            
            // 移動時間はリセットする
            m_work[kAKWorkMoveTime] = 0;
            
            // 中央位置に到達したら弾を発射し始める
            if (AKIsEqualFloat(m_position.x, nextPosition.x) &&
                AKIsEqualFloat(m_position.y, nextPosition.y)) {
                
                // 包囲弾を発射する
                if ((m_frame + 1) % kAKSiegeShotInterval == 0) {

                    // 包囲弾の角度を求める
                    float angle = kAKSiegeShotStartAngle - m_work[kAKWorkSiegeAngle] * kAKSiegeShotAngleSpeed;
                    
                    // 最終角度に到達していれば最終角度を設定する
                    if (angle <= kAKSiegeShotEndAngle) {
                        angle = kAKSiegeShotEndAngle;
                    }
                    
                    AKEnemy::fireNWay(M_PI,
                                      m_position,
                                      2,
                                      angle,
                                      kAKSiegeShotSpeed,
                                      true,
                                      data);
                    
                    // 包囲弾の角度を狭める
                    m_work[kAKWorkSiegeAngle]++;
                }
                
                // 1グループ弾発射間隔時間経過したらグループ弾を発射する
                if ((m_frame + 1) % kAKGroupShotInterval == 0) {
                    
                    // 自機へ向けて弾を発射する
                    AKEnemy::fireGroupShot(m_position,
                                           kAKGroupShotPosition,
                                           kAKGroupShotCount,
                                           kAKGroupShotSpeed,
                                           data);
                }
                
                // 1-way弾発射間隔時間経過したら弾を発射する
                if ((m_frame + 1) % kAK1WayInterval == 0) {
                    
                    // 自機へ向けて弾を発射する
                    AKEnemy::fireNWay(m_position,
                                      1,
                                      0,
                                      kAK1WayShotSpeed,
                                      data);
                }
            }

            break;
            
        default:
            AKAssert(false, "状態が異常:m_state=%d", m_state);
            break;
    }
    
    // 2周目時は3-way弾を発射する
    if (data->is2ndLoop()) {
        
        // 弾発射状態の場合
        if (m_state != kAKStateInit) {
            
            // 2周目追加ショット発射間隔が経過している場合は弾を発射する
            if ((m_frame + 1) % kAK2ndLoopShotInterval == 0) {
                
                AKEnemy::fireNWay(m_position,
                                  3,
                                  M_PI / 8.0f,
                                  kAK2ndLoopShotSpeed,
                                  data);
            }
        }
    }
    
    // 移動先と現在位置が異なる場合は速度の設定を行う
    if (!AKIsEqualFloat(m_position.x, nextPosition.x) || !AKIsEqualFloat(m_position.y, nextPosition.y)) {
    
        // 移動先の角度を計算する
        float moveAngle = AKAngle::calcDestAngle(m_position, nextPosition);
    
        // 移動スピードを設定する
        m_speedX = kAKMoveSpeed * cosf(moveAngle);
        m_speedY = kAKMoveSpeed * sinf(moveAngle);
    
        // x方向の移動距離がスピードより小さい場合は通り過ぎないように移動先座標をセットする
        if (fabs(m_position.x - nextPosition.x) < m_speedX) {
            m_position.x = nextPosition.x;
            m_speedX = 0.0f;
        }
        
        // y方向の移動距離がスピードより小さい場合は通り過ぎないように移動先座標をセットする
        if (fabs(m_position.y - nextPosition.y) < m_speedY) {
            m_position.y = nextPosition.y;
            m_speedY = 0.0f;
        }
        
    }
    // 移動しない場合はスピードを0にする
    else {
        m_speedX = 0.0f;
        m_speedY = 0.0f;
        
        // 移動時間をカウントし、位置変更の間隔が経過した場合は移動位置を変更する
        m_work[kAKWorkMoveTime]++;
        if (m_work[kAKWorkMoveTime] >= kAKMoveInterval) {
            m_work[kAKWorkMoveTime] = 0;
            m_work[kAKWorkCurrentPosition] = (m_work[kAKWorkCurrentPosition] + 1) % kAKMovePositionCount;
        }
    }

    // 状態遷移間隔が経過している場合は次の状態へ進める
    if (m_frame > kAKStateInterval[m_state]) {
        
        // 次の状態へ進める
        m_state++;
        
        // 状態が最大を超える場合は最初の状態へループする
        if (m_state >= kAKStateCount) {
            m_state = kAKStateInit + 1;
        }
        
        // 経過フレーム数と作業領域を初期化する
        m_frame = 0.0f;
        m_work[kAKStateSiege] = 0;
        
        AKLog(kAKLogEnemy_3, "m_state=%d", m_state);
    }
}

/*!
 @brief ムカデ（頭）の動作処理

 初期状態:胴体、尻尾部分の作成する。
 
 登場:画面スクロールにあわせて移動する。
 
 移動:最初は左下に移動する。画面の端に到達すると進行方向の角度を変え、時計回りに回る。
 
 @param data ゲームデータ
 */
void AKEnemy::actionOfCentipedeHead(AKPlayDataInterface *data)
{
    // 状態
    enum STATE {
        kAKStateInit = 0,       // 初期状態
        kAKStateEntry,          // 登場
        kAKState5WayShot,       // 5-way弾発射
        kAKStateHighSpeedShot,  // 2-way弾による包囲弾発射
        kAKStateBodyShot,       // 胴体部分からのショット
        kAKStateCount           // 状態の種類の数
    };
    // 作業領域の用途
    enum WORK {
        kAKWorkMoveX = 0,   // 移動方向x方向(左:-1、右:1)
        kAKWorkMoveY,       // 移動方向y方向(下:-1、上:1)
        kAKWork1WayTime     // 1-way弾発射の待機時間
    };
    // 状態遷移間隔
    const int kAKStateInterval[kAKStateCount] = {999, 170, 900, 900, 900};
    // 移動方向変更の境界線x方向最小値
    const float kAKMoveChangePointXMin = 16.0f;
    // 移動方向変更の境界線x方向最大値
    const float kAKMoveChangePointXMax = 368.0f;
    // 移動方向変更の境界線y方向最小値
    const float kAKMoveChangePointYMin = 32.0f;
    // 移動方向変更の境界線y方向最大値
    const float kAKMoveChangePointYMax = 256.0f;
    // 移動スピード
    const float kAKMoveSpeed = 1.0f;
    // 胴体の数
    const int kAKBodyCount = 18;
    // 5-way弾発射間隔
    const int kAK5WayInterval = 40;
    // 5-way弾スピード
    const float kAK5WayShotSpeed = 1.2f;
    // 1-way弾の待機時間
    const int kAK1WayWaitTime = 10;
    // 1-way弾の発射時間
    const int kAK1WayShotTime = 80;
    // 1-way弾発射間隔
    const int kAK1WayInterval = 20;
    // 1-way弾スピード
    const float kAK1WayShotSpeed = 1.8f;
    // 2周目追加ショット発射間隔
    const int kAK2ndLoopShotInterval = 120;
    // 2周目追加ショットスピード
    const float kAK2ndLoopShotSpeed = 1.0f;
    
    // 状態によって処理を分岐する
    switch (m_state) {
        case kAKStateInit:      // 初期状態
            
        {
            // 胴体部分を作成する
            AKEnemy *parent = this;
            int animationFrame = 0;
            for (int i = 0; i < kAKBodyCount; i++) {
                
                // 胴体を作成する
                AKEnemy *body = data->createEnemy(kAKEnemyCentipedeBody, m_position, 0);
                
                // 胴体の前の部分を設定する
                body->setParentEnemy(parent);
                
                // 前の部分の体の次の部分に今回作成した体を設定する
                parent->setChildEnemy(body);
                
                // 足の動きが交互になるようにアニメーションフレームを設定する
                body->setAnimationFrame(animationFrame);
                if ((i + 1) % 4 < 2) {
                    animationFrame = 0;
                }
                else {
                    animationFrame = kAKEnemyDef[kAKEnemyCentipedeBody - 1].animationInterval;
                }
                
                // 今回作成した胴体を次の胴体の前の部分に使用する
                parent = body;
            }
            
            // 尻尾を作成する
            AKEnemy *tail = data->createEnemy(kAKEnemyCentipedeTail, m_position, 0);
            
            // 足の動きが交互になるようにアニメーションフレームを設定する
            tail->setAnimationFrame(animationFrame);

            // 尻尾の前の部分を設定する
            tail->setParentEnemy(parent);
            
            // 前の部分の体の次の部分に今回作成した体を設定する
            parent->setChildEnemy(tail);
        }
            
            // 状態を次に進める
            m_state = kAKStateEntry;
            
            // 初期移動方向は左下とする
            m_work[kAKWorkMoveX] = -1;
            m_work[kAKWorkMoveY] = -1;
            
            break;
            
        case kAKStateEntry:     // 登場
            
            // 左方向へ移動する
            m_speedX = -1.0f;
            m_speedY = 0.0f;
            
            // 画像を回転させる
            getImage()->setRotation(AKAngle::convertAngleRad2Scr(M_PI));
            
            break;
            
        case kAKState5WayShot:  // 5-way弾発射
            
            // 弾発射間隔時間経過したら弾を発射する
            if ((m_frame + 1) % kAK5WayInterval == 0) {
                
                // 自機へ向けて弾を発射する
                AKEnemy::fireNWay(m_position,
                                  5,
                                  M_PI / 8.0f,
                                  kAK5WayShotSpeed,
                                  data);
            }
            
            break;
            
        case kAKStateHighSpeedShot:     // 1-way弾による高速弾発射
            
            // 1-way弾グループの待機時間が経過している場合は1-way弾を発射し始める
            if (m_frame - m_work[kAKWork1WayTime] > kAK1WayWaitTime) {
                
                // 弾発射間隔時間経過したら弾を発射する
                if ((m_frame + 1) % kAK1WayInterval == 0) {
                    
                    // 自機へ向けて弾を発射する
                    AKEnemy::fireNWay(m_position,
                                      1,
                                      0.0f,
                                      kAK1WayShotSpeed,
                                      data);
                }
                
                // 1-way弾の発射時間が経過している場合は作業領域に現在の経過フレーム数を入れて
                // 1-way弾グループの発射間隔分待機する
                if (m_frame - m_work[kAKWork1WayTime] > kAK1WayShotTime + kAK1WayWaitTime) {
                    m_work[kAKWork1WayTime] = m_frame;
                }
            }
            
            break;
            
        case kAKStateBodyShot:  // 胴体部分からのショット
            
            break;
            
        default:
            AKAssert(false, "状態が異常:m_state=%d", m_state);
            break;
    }
    
    // 5-way弾発射と高速弾発射時の移動処理
    if (m_state > kAKStateEntry) {
        
        AKLog(kAKLogEnemy_3, "position=(%f, %f) moveX=%d, moveY=%d",
              m_position.x, m_position.y, m_work[kAKWorkMoveX], m_work[kAKWorkMoveY]);
        
        // 下方向へ移動中に下端に達したら向きを変える
        if (m_work[kAKWorkMoveY] < 0 &&
            m_position.y < kAKMoveChangePointYMin) {
            
            m_work[kAKWorkMoveY] *= -1;
            
            AKLog(kAKLogEnemy_3, "下端到達:moveY=%d", m_work[kAKWorkMoveY]);
        }
        // 上方向へ移動中に上端に達したら向きを変える
        else if (m_work[kAKWorkMoveY] > 0 &&
                 m_position.y > kAKMoveChangePointYMax) {
            
            m_work[kAKWorkMoveY] *= -1;
            
            AKLog(kAKLogEnemy_3, "上端到達:moveY=%d", m_work[kAKWorkMoveY]);
        }

        // 左方向へ移動中に左端に達したら向きを変える
        if (m_work[kAKWorkMoveX] < 0 &&
            m_position.x < kAKMoveChangePointXMin) {
            
            m_work[kAKWorkMoveX] *= -1;
            
            AKLog(kAKLogEnemy_3, "左端到達:moveX=%d", m_work[kAKWorkMoveX]);
        }
        // 右方向へ移動中に右端に達したら向きを変える
        else if (m_work[kAKWorkMoveX] > 0 &&
                 m_position.x > kAKMoveChangePointXMax) {
            
            m_work[kAKWorkMoveX] *= -1;
            
            AKLog(kAKLogEnemy_3, "右端到達:moveX=%d", m_work[kAKWorkMoveX]);
        }
        
        // スピードを設定する
        m_speedX = kAKMoveSpeed * m_work[kAKWorkMoveX];
        m_speedY = kAKMoveSpeed * m_work[kAKWorkMoveY];
        
        // 移動方向の角度を計算する
        float angle = atan2f(m_speedY, m_speedX);
        
        // 画像を回転させる
        getImage()->setRotation(AKAngle::convertAngleRad2Scr(angle));
    }
    
    // 2周目時は3-way弾を発射する
    if (data->is2ndLoop()) {
        
        // 弾発射状態の場合
        if (m_state > kAKStateEntry) {
            
            // 2周目追加ショット発射間隔が経過している場合は弾を発射する
            if ((m_frame + 1) % kAK2ndLoopShotInterval == 0) {
                
                AKEnemy::fireNWay(m_position,
                                  3,
                                  M_PI / 8.0f,
                                  kAK2ndLoopShotSpeed,
                                  data);
            }
        }
    }
    
    // 状態遷移間隔が経過している場合は次の状態へ進める
    if (m_frame > kAKStateInterval[m_state]) {
        
        // 次の状態へ進める
        m_state++;
        
        // 状態が最大を超える場合は最初の状態へループする
        if (m_state >= kAKStateCount) {
            m_state = kAKStateEntry + 1;
        }
        
        // 経過フレーム数と作業領域を初期化する
        m_frame = 0.0f;
        m_work[kAKWork1WayTime] = 0;
        
        // 子オブジェクトに現在の状態を通知する
        if (m_childEnemy != NULL) {
            m_childEnemy->setState(m_state);
        }
        
        AKLog(kAKLogEnemy_3, "m_state=%d", m_state);
    }
}

/*!
 @brief ムカデ（胴体）の動作処理
 
 一つ前の体の後ろについていく。
 
 @param data ゲームデータ
 */
void AKEnemy::actionOfCentipedeBody(AKPlayDataInterface *data)
{
    // 状態
    enum STATE {
        kAKStateInit = 0,       // 初期状態
        kAKStateEntry,          // 登場
        kAKState5WayShot,       // 5-way弾発射
        kAKStateHighSpeedShot,  // 2-way弾による包囲弾発射
        kAKStateBodyShot,       // 胴体部分からのショット
        kAKStateCount           // 状態の種類の数
    };
    // 弾発射間隔
    const int kAKShotInterval = 100;
    // 弾のスピード
    const float kAKShotSpeed = 1.5f;

    // 一つ前の体がメンバに設定されていない場合は処理しない
    if (m_parentEnemy == NULL) {
        AKAssert(false, "m_parentEnemy is null.");
        return;
    }
    
    // 一つ前の体の移動履歴を取得する
    const std::queue<Vec2> *history = m_parentEnemy->getMoveHistory();
    
    // 履歴がない場合は処理を終了する
    if (history->size() <= 0) {
        return;
    }
    
    // 移動履歴の末尾を自分の座標に設定する
    m_position = history->front();
    
    // 前回位置との差から体の向きを決める
    float dx = m_position.x - m_prevPosition.x;
    float dy = m_position.y - m_prevPosition.y;
    float angle = atan2f(dy, dx);
    
    // 画像を回転させる
    getImage()->setRotation(AKAngle::convertAngleRad2Scr(angle));
    
    // 画像の表示を更新する
    updateImagePosition();
    
    // 胴体部分からの発射の状態の時は弾を発射する
    if (m_state == kAKStateBodyShot) {
        
        // 弾発射間隔経過しているときは上下方向へ弾を発射する
        if ((m_frame + 1) % kAKShotInterval == 0) {
            
            // 上下へ弾を発射する
            AKEnemy::fireNWay(M_PI,
                              m_position,
                              2,
                              M_PI,
                              kAKShotSpeed,
                              false,
                              data);
        }

    }
    
    // 子オブジェクトに現在の状態を通知する
    if (m_childEnemy != NULL) {
        m_childEnemy->setState(m_state);
    }
}

/*!
 @brief ムカデ（尻尾）の動作処理
 
 一つ前の体の後ろについていく。
 3-way弾を自機に向けて発射する。
 
 @param data ゲームデータ
 */
void AKEnemy::actionOfCentipedeTail(AKPlayDataInterface *data)
{
    // 3-way弾の発射間隔
    const int kAK3WayShotInterval = 60;
    // 3-way弾の弾数
    const int kAK3WayShotCount = 3;
    // 3-way弾の角度の間隔
    const float kAK3WayShotAngle = M_PI / 8;
    // 3-way弾のスピード
    const float kAK3WayShotSpeed = 2.0f;
    // 3-way弾発射までの間
    const int kAK3WayShotWait = 250;
    
    // 一つ前の体がメンバに設定されていない場合は処理しない
    if (m_parentEnemy == NULL) {
        AKAssert(false, "m_parentEnemy is null.");
        return;
    }
    
    // 一つ前の体の移動履歴を取得する
    const std::queue<Vec2> *history = m_parentEnemy->getMoveHistory();
    
    // 履歴がない場合は処理を終了する
    if (history->size() <= 0) {
        return;
    }
    
    // 移動履歴の末尾を自分の座標に設定する
    m_position = history->front();
    
    // 前回位置との差から体の向きを決める
    float dx = m_position.x - m_prevPosition.x;
    float dy = m_position.y - m_prevPosition.y;
    float angle = atan2f(dy, dx);
    
    // 画像を回転させる
    getImage()->setRotation(AKAngle::convertAngleRad2Scr(angle));
    
    // 画像の表示を更新する
    updateImagePosition();

    // 定周期に3-way弾を発射する
    if (m_frame > kAK3WayShotWait && (m_frame + 1) % kAK3WayShotInterval == 0) {
        
        AKEnemy::fireNWay(m_position,
                          kAK3WayShotCount,
                          kAK3WayShotAngle,
                          kAK3WayShotSpeed,
                          data);
    }
}

/*!
 @brief ウジの動作処理
 
 定周期に3-way弾を発射する。
 生成されたときに設定されたstateに応じて発射間隔と画像の向きを変える。
 @param data ゲームデータ
 */
void AKEnemy::actionOfMaggot(AKPlayDataInterface *data)
{
    // 3-way弾の発射間隔
    const int kAK3WayShotInterval[8] = {120, 130, 140, 150, 160, 170, 180, 190};
    // 3-way弾の弾数
    const int kAK3WayShotCount = 3;
    // 3-way弾の角度の間隔
    const float kAK3WayShotAngle = M_PI / 8;
    // 3-way弾のスピード
    const float kAK3WayShotSpeed[8] = {0.8f, 0.8f, 0.9f, 0.9f, 1.0f, 1.0f, 1.1f, 1.1f};
    // 3-way弾発射までの間
    const int kAK3WayShotWait = 230;

    // スクロールに合わせて移動する
    m_scrollSpeed = 1.0f;
    
    // 向きを変更する
    getImage()->setRotation(AKAngle::convertAngleRad2Scr((M_PI / 4) * (m_state % 8)));

    // 定周期に3-way弾を発射する
    if (m_frame > kAK3WayShotWait &&
        ((m_frame + 1) % kAK3WayShotInterval[m_state % 8]) == 0) {
        
        AKEnemy::fireNWay(m_position,
                          kAK3WayShotCount,
                          kAK3WayShotAngle,
                          kAK3WayShotSpeed[m_state % 8],
                          data);
    }

    // 定周期に4-way弾を発射する
    if (m_frame > kAK3WayShotWait &&
        ((m_frame + 60) % kAK3WayShotInterval[(m_state + 3) % 8]) == 0) {
        
        AKEnemy::fireNWay(m_position,
                          kAK3WayShotCount + 1,
                          kAK3WayShotAngle,
                          kAK3WayShotSpeed[(m_state + 3) % 8],
                          data);
    }
}

/*!
 @brief ハエの動作処理

 初期状態:当たり判定をなくし、画像を非常時にして、ウジを生成する。
 
 待機状態:ウジがすべて倒されるまで待機する。(無処理)
 @param data ゲームデータ
 */
void AKEnemy::actionOfFly(AKPlayDataInterface *data)
{
    // 状態
    enum STATE {
        kAKStateInit = 0,       // 初期状態
        kAKStateWait,           // 待機状態
        kAKStateEntry1,         // 登場1
        kAKStateEntry2,         // 登場2
        kAKStateEntry3,         // 登場3
        kAKStateNWay,           // n-way弾発射
        kAKStateAllDirection,   // 全方位弾発射
        kAKStateAllRange,       // 全画面弾発射
        kAKStateCount           // 状態の種類の数
    };
    // 作業領域の用途
    enum WORK {
        kAKWorkAngle = 0,       // 進行方向
        kAKWorkNextPositionX,   // 次の位置x座標
        kAKWorkNextPositionY,   // 次の位置y座標
        kAKWorkMoveWait,        // 移動待機時間
        kAKWorkShot             // 弾発射作業変数
    };
    // 状態遷移間隔
    const int kAKStateInterval[kAKStateCount] = {999, 999, 0, 300, 300, 900, 900, 900};
    // ウジの数
    const int kAKMaggotCount = 16;
    // ウジの生成位置
    const float kAKMaggotPotision[kAKMaggotCount][2] = {
        {0, 0}, {0, 24}, {0, -24},
        {24, 0}, {24, 24}, {24, 48}, {24, -24}, {24, -48},
        {-24, 0}, {-24, 24}, {-24, 48}, {-24, -24}, {-24, -48},
        {-48, 0}, {48, 24}, {48, -24}
    };
    // 爆発の間隔
    const int kAKExplosionInterval = 20;
    // 移動速度
    const float kAKMoveSpeed = 2.0f;
    // 移動角速度
    const float kAKMoveAnglarVelocity = 0.05f;
    // 移動位置x座標
    const float kAKMovePositionX[2] = {212, 336};
    // 移動待機時間
    const int kAKMoveInterval = 60;
    // 移動位置到達判定範囲
    const float kAKMoveArrivalRange = 8;
    // n-way弾の発射間隔
    const int kAKNWayShotInterval = 30;
    // n-way弾の弾数
    const int kAKNWayShotCount = 13;
    // n-way弾の角度の間隔
    const float kAKNWayShotAngle = M_PI / 16;
    // n-way弾のスピード
    const float kAKNWayShotSpeed = 1.7f;
    // 矢印弾の待機時間
    const int kAKArrowShotWait = 120;
    // 矢印弾の発射時間
    const int kAKArrowShotTime = 90;
    // 矢印弾の発射間隔
    const int kAKArrowShotInterval = 30;
    // 矢印弾の数
    const int kAKArrowShotCount = 5;
    // 矢印弾のスピード
    const float kAKArrowShotSpeed[kAKArrowShotCount] = {2.2f, 2.4f, 2.6f, 2.8f, 3.0f};
    // 全方位弾の発射間隔
    const int kAKAllDirectionInterval = 10;
    // 全方位弾の弾数
    const float kAKAllDirectionCount = 8;
    // 全方位弾の角度の間隔
    const float kAKAllDirectionAngle = 2 * M_PI / kAKAllDirectionCount;
    // 全方位弾のスピード
    const float kAKAllDirectionSpeed = 1.2f;
    // 全方位弾の回転スピード
    const float kAKAllDirectionRotationSpeed = 0.1f;
    // 全画面弾の発射間隔
    const int kAKAllRangeInterval = 40;
    // 全画面弾の弾の位置の間隔
    const int kAKAllRangePositionInterval = 50;
    // 全画面弾のスピード
    const float kAKAllRangeSpeed = 1.2f;
    // 全画面弾の発射位置移動スピード
    const int kAKAllRangePositionSpeed = 3;
    // 3-way弾発射間隔
    const int kAK3WayInterval = 40;
    // 3-way弾スピード
    const float kAK3WayShotSpeed = 1.8f;
    // 2周目追加ショット発射間隔
    const int kAK2ndLoopShotInterval = 120;
    // 2周目追加ショットスピード
    const float kAK2ndLoopShotSpeed = 1.0f;
    // 縦方向の移動範囲の画面端からの距離
    const int kAKMaxMoveRangeFromEdge = 96;
    
    // 状態によって処理を分岐する
    switch (m_state) {
        case kAKStateInit:      // 初期状態
            
            // 当たり判定をなしにする
            m_size.width = 0;
            m_size.height = 0;
            
            // 画像を非表示にする
            getImage()->setVisible(false);
            
        {
            // ウジを生成する
            int animationFrame = 0;
            for (int i = 0; i < kAKMaggotCount; i++) {
                
                // 生成位置を決める
                Vec2 position(m_position.x + kAKMaggotPotision[i][0],
                                 m_position.y + kAKMaggotPotision[i][1]);
                
                // ウジを生成する
                AKEnemy *maggot = data->createEnemy(kAKEnemyMaggot, position, 1);
                
                // ウジの親キャラを設定する
                maggot->setParentEnemy(this);

                // 動きがバラバラになるようにアニメーションフレームを設定する
                maggot->setAnimationFrame(animationFrame);
                animationFrame += 3;
                animationFrame %= kAKEnemyDef[kAKEnemyMaggot - 1].animationInterval;
                
                // それぞれの動作を変えるために状態を変更する
                maggot->setState(i);
            }
        }
            // 生成したウジの数だけヒットポイントを加算する
            m_hitPoint += kAKMaggotCount;
            
            // 待機状態に移行する
            m_state = kAKStateWait;
            
            break;
            
        case kAKStateWait:      // 待機状態
            
            // 状態が進まないようにフレーム数をリセットする
            m_frame = 0;
            
            break;
            
        case kAKStateEntry1:    // 登場1
            
            // 無処理
            
            break;
            
        case kAKStateEntry2:    // 登場2
            
            // 爆発の間隔が経過している場合は爆発を発生させる
            if ((m_frame + 1) % kAKExplosionInterval == 0) {
                
                // 爆発発生位置を決める
                int w = kAKEnemyDef[kAKEnemyFly - 1].hitWidth;
                int h = kAKEnemyDef[kAKEnemyFly - 1].hitHeight;
                int x = rand() % (w * 2) - w;
                int y = rand() % (h * 2) - h;
                
                // 画面効果を生成する
                data->createEffect(1, Vec2(m_position.x + x, m_position.y + y));           
            }
            
            break;
            
        case kAKStateEntry3:    // 登場3
            
            // 画像を表示する
            if (!getImage()->isVisible()) {
                getImage()->setVisible(true);
                
                // 当たり判定を設定する
                m_size.width = kAKEnemyDef[kAKEnemyFly - 1].hitWidth;
                m_size.height = kAKEnemyDef[kAKEnemyFly - 1].hitHeight;
            }
            
            // 向きは上向きにする
            m_work[kAKWorkAngle] = M_PI / 2;
            
            // 次の移動位置を設定する
            m_work[kAKWorkNextPositionX] = kAKMovePositionX[0];
            m_work[kAKWorkNextPositionY] = data->getPlayerPosition()->y;
            
            break;
            
        case kAKStateNWay:          // n-way弾発射
            
            // 定周期にn-way弾を発射する
            if ((m_frame + 1) % kAKNWayShotInterval == 0) {
                
                AKEnemy::fireNWay(m_position,
                                  kAKNWayShotCount,
                                  kAKNWayShotAngle,
                                  kAKNWayShotSpeed,
                                  data);
            }

            // 矢印弾グループの待機時間が経過している場合は矢印弾を発射し始める
            if (m_frame - m_work[kAKWorkShot] > kAKArrowShotWait) {
                
                // 矢印弾の発射間隔が経過している場合は弾を発射する
                if ((m_frame + 1) % kAKArrowShotInterval == 0) {
                    
                    // 矢印弾を発射する
                    for (int i = 0; i < kAKArrowShotCount; i++) {
                        AKEnemy::fireNWay(m_position,
                                          5,
                                          M_PI / 32.0f,
                                          kAKArrowShotSpeed[i],
                                          data);
                    }
                }
                
                // 矢印弾の発射時間が経過している場合は作業領域に現在の経過フレーム数を入れて
                // 矢印弾グループの発射間隔分待機する
                if (m_frame - m_work[kAKWorkShot] > kAKArrowShotTime + kAKArrowShotWait) {
                    m_work[kAKWorkShot] = m_frame;
                }
            }

            break;
            
        case kAKStateAllDirection:  // 全方位弾発射
            
            // 全方位弾の発射間隔が経過している場合は弾を発射する
            if ((m_frame + 1) % kAKAllDirectionInterval == 0) {
                
                AKEnemy::fireNWay(m_work[kAKWorkShot] * kAKAllDirectionRotationSpeed,
                                  m_position,
                                  kAKAllDirectionCount,
                                  kAKAllDirectionAngle,
                                  kAKAllDirectionSpeed,
                                  true,
                                  data);
                
                AKEnemy::fireNWay(-m_work[kAKWorkShot] * kAKAllDirectionRotationSpeed,
                                  m_position,
                                  kAKAllDirectionCount,
                                  kAKAllDirectionAngle,
                                  kAKAllDirectionSpeed,
                                  true,
                                  data);
                // 角度を進める
                m_work[kAKWorkShot]++;
            }
            
            // 3-way弾発射間隔時間経過したら弾を発射する
            if ((m_frame + 1) % kAK3WayInterval == 0) {
                
                // 自機へ向けて弾を発射する
                AKEnemy::fireNWay(m_position,
                                  3,
                                  M_PI / 16.0f,
                                  kAK3WayShotSpeed,
                                  data);
            }
            
            break;
            
        case kAKStateAllRange:      // 全画面弾発射
            
            // 全画面弾の発射間隔が経過している場合は弾を発射する
            if ((m_frame + 1) % kAKAllRangeInterval == 0) {
                
                // 上方向から下方向への弾を発射する
                for (int i = -m_work[kAKWorkShot];
                     i < AKScreenSize::stageSize().width;
                     i += kAKAllRangePositionInterval) {
                    
                    // 画面外の座標は飛ばす
                    if (i < 0) {
                        continue;
                    }
                    
                    AKEnemy::fireNWay(-M_PI / 2,
                                      Vec2(i, AKScreenSize::stageSize().height - 36),
                                      1,
                                      0,
                                      kAKAllRangeSpeed,
                                      true,
                                      data);
                    
                }
                
                // 右方向から左方向への弾を発射する
                for (int i = -m_work[kAKWorkShot];
                     i < AKScreenSize::stageSize().height;
                     i += kAKAllRangePositionInterval) {

                    // 画面外の座標は飛ばす
                    if (i < 0) {
                        continue;
                    }
                    
                    AKEnemy::fireNWay(M_PI,
                                      Vec2(AKScreenSize::stageSize().width, i),
                                      1,
                                      0,
                                      kAKAllRangeSpeed,
                                      true,
                                      data);
                }
                
                // 発射位置の座標を移動する
                m_work[kAKWorkShot] += kAKAllRangePositionSpeed;
            }
            
            // 3-way弾発射間隔時間経過したら弾を発射する
            if ((m_frame + 1) % kAK3WayInterval == 0) {
                
                // 自機へ向けて弾を発射する
                AKEnemy::fireNWay(m_position,
                                  3,
                                  M_PI / 16.0f,
                                  kAK3WayShotSpeed,
                                  data);
            }
            
            break;
            
        default:
            AKAssert(false, "状態が異常:m_state=%d", m_state);
            break;
    }
    
    // 登場以降は移動を行う
    if (m_state > kAKStateEntry3) {
        
        // 待機時間が残っている場合
        if (m_work[kAKWorkMoveWait] > 0) {
            
            // 待機時間をカウントする
            m_work[kAKWorkMoveWait]--;
            
            // 待機時間がなくなった場合は次の位置を設定する
            if (m_work[kAKWorkMoveWait] <= 0) {
                
                // x座標は2箇所を交互に設定する
                if (m_work[kAKWorkNextPositionX] == kAKMovePositionX[0]) {
                    m_work[kAKWorkNextPositionX] = kAKMovePositionX[1];
                }
                else {
                    m_work[kAKWorkNextPositionX] = kAKMovePositionX[0];
                }
                
                // y座標は自機の位置を設定する
                m_work[kAKWorkNextPositionY] = data->getPlayerPosition()->y;
                
                AKLog(kAKLogEnemy_3, "y=%d", m_work[kAKWorkNextPositionY]);
                // 画面端まで移動すると、回転して方向を変えるときに画面外に出てしまうため
                // 上下限を決める
                if (m_work[kAKWorkNextPositionY] < kAKMaxMoveRangeFromEdge) {
                    m_work[kAKWorkNextPositionY] = kAKMaxMoveRangeFromEdge;
                    AKLog(kAKLogEnemy_3, "下限補正 : y=%d", m_work[kAKWorkNextPositionY]);
                }
                else if (m_work[kAKWorkNextPositionY] > AKScreenSize::stageSize().height - kAKMaxMoveRangeFromEdge) {
                    m_work[kAKWorkNextPositionY] = AKScreenSize::stageSize().height - kAKMaxMoveRangeFromEdge;
                    AKLog(kAKLogEnemy_3, "上限補正 : y=%d", m_work[kAKWorkNextPositionY]);
                }
                else {
                    // Do nothing.
                }
            }
            
            // 速度を0に設定する
            m_speedX = 0;
            m_speedY = 0;
        }
        // 移動位置に到達している場合
        else if (std::abs(m_position.x - m_work[kAKWorkNextPositionX]) < kAKMoveArrivalRange &&
                 std::abs(m_position.y - m_work[kAKWorkNextPositionY]) < kAKMoveArrivalRange) {
            
            // 待機時間を設定する
            m_work[kAKWorkMoveWait] = kAKMoveInterval;
            
            // 速度を0に設定する
            m_speedX = 0;
            m_speedY = 0;
        }
        // 移動中の場合
        else {
            
            // 目的地の角度を求める
            float destAngle = AKAngle::calcDestAngle(m_position,
                                                     Vec2(m_work[kAKWorkNextPositionX],
                                                             m_work[kAKWorkNextPositionY]));
            
            // 現在の角度を求める
            float currentAngle = AKAngle::convertAngleScr2Rad(getImage()->getRotation());
            
            // 現在の角度を正規化する
            currentAngle = AKAngle::normalize(currentAngle);
            
            // 回転する方向を決める
            int rotationDirection = AKAngle::calcRotationDirection(currentAngle, destAngle);
            
            // 回転する方向に角度を動かす
            float nextAngle = currentAngle + rotationDirection * kAKMoveAnglarVelocity;
            if (fabsf(currentAngle - destAngle) < kAKMoveAnglarVelocity) {
                nextAngle = destAngle;
            }
            
            AKLog(kAKLogEnemy_3, "current=%f, dest=%f, next=%f", currentAngle, destAngle, nextAngle);
            
            // 画像を回転する
            getImage()->setRotation(AKAngle::convertAngleRad2Scr(nextAngle));
            
            // 速度を向きから決定する
            m_speedX = cosf(nextAngle) * kAKMoveSpeed;
            m_speedY = sinf(nextAngle) * kAKMoveSpeed;
        }
        
    }

    // 2周目時は3-way弾を発射する
    if (data->is2ndLoop()) {
        
        // 弾発射状態の場合
        if (m_state > kAKStateEntry3) {
            
            // 2周目追加ショット発射間隔が経過している場合は弾を発射する
            if ((m_frame + 1) % kAK2ndLoopShotInterval == 0) {
                
                AKEnemy::fireNWay(m_position,
                                  3,
                                  M_PI / 8.0f,
                                  kAK2ndLoopShotSpeed,
                                  data);
            }
        }
    }
    
    // 状態遷移間隔が経過している場合は次の状態へ進める
    if (m_frame > kAKStateInterval[m_state]) {
        
        // 次の状態へ進める
        m_state++;
        
        // 状態が最大を超える場合は最初の状態へループする
        if (m_state >= kAKStateCount) {
            m_state = kAKStateEntry3 + 1;
        }
        
        // 経過フレーム数と作業領域を初期化する
        m_frame = 0.0f;
        m_work[kAKWorkShot] = 0;
        
        AKLog(kAKLogEnemy_3, "m_state=%d", m_state);
    }
}

/*!
 @brief 破壊処理なし
 
 破壊エフェクトを発生させない
 @param data ゲームデータ
 */
void AKEnemy::destroyNone(AKPlayDataInterface *data)
{
    // 無処理
}

/*!
 @brief 雑魚敵の破壊処理
 
 破壊エフェクトを発生させる。
 @param data ゲームデータ
 */
void AKEnemy::destroyNormal(AKPlayDataInterface *data)
{
    const float kAKShotSpeed = 1.0f;
    
    AKLog(kAKLogEnemy_1, "start");
    
    // 画面効果を生成する
    data->createEffect(1, m_position);
    
    // 破壊の効果音を鳴らす
    SimpleAudioEngine::getInstance()->playEffect(kAKBombMinSEFileName);
    
    // 2周目時は3-way弾を発射する
    if (data->is2ndLoop()) {
        AKEnemy::fireNWay(m_position,
                          3,
                          M_PI / 8.0f,
                          kAKShotSpeed,
                          data);
    }
}

/*!
 @brief ボス敵の破壊処理
 
 一定期間爆発エフェクトを発生させる。
 @param data ゲームデータ
 */
void AKEnemy::destroyOfBoss(AKPlayDataInterface *data)
{
    // 爆発の間隔
    const int kAKExplosionInterval = 20;
    // 状態遷移間隔
    const int kAKStateInterval = 300;
    
    // 爆発の間隔が経過している場合は爆発を発生させる
    if ((m_destroyframe + 1) % kAKExplosionInterval == 0) {
        
        // 爆発発生位置を決める
        int w = kAKEnemyDef[kAKEnemyFly - 1].hitWidth;
        int h = kAKEnemyDef[kAKEnemyFly - 1].hitHeight;
        int x = rand() % (w * 2) - w;
        int y = rand() % (h * 2) - h;
        
        // 画面効果を生成する
        data->createEffect(1, Vec2(m_position.x + x, m_position.y + y));
        
        // 破壊の効果音を鳴らす
        SimpleAudioEngine::getInstance()->playEffect(kAKBombMinSEFileName);
    }
    
    // 状態遷移間隔を経過するまでは死亡フラグを立てない
    if (m_destroyframe < kAKStateInterval) {
        m_isDead = false;
    }
    else {
        m_isDead = true;
    }
}

/*!
 @brief ムカデの破壊処理
 
 尻尾が破壊された時に胴体を一つ短くして尻尾を復活させる。
 胴体がない場合はムカデ撃破とする。
 @param data ゲームデータ
 */
void AKEnemy::destroyOfCentipede(AKPlayDataInterface *data)
{
    // 2個前の体が存在する場合（1個前が胴体の場合）
    if (m_parentEnemy->m_parentEnemy != NULL) {
        
        // 画面効果を生成する
        data->createEffect(1, m_position);
        
        // 1個前の胴体の位置に尻尾を作成する
        AKEnemy *tail = data->createEnemy(kAKEnemyCentipedeTail, *m_parentEnemy->getPosition(), 0);
            
        // アニメーションフレームは1個前の胴体のものを引き継ぐ
        tail->setAnimationFrame(m_parentEnemy->getAnimationFrame());

        // 尻尾の前の部分を設定する
        tail->setParentEnemy(m_parentEnemy->m_parentEnemy);
            
        // 前の部分の体の次の部分に今回作成した体を設定する
        m_parentEnemy->m_parentEnemy->setChildEnemy(tail);
        
        // 頭を検索する
        AKEnemy *head = m_parentEnemy->m_parentEnemy;
        while (head->m_parentEnemy != NULL) {
            head = head->m_parentEnemy;
        }
        
        // 頭のヒットポイントを減らす
        head->setHitPoint(head->getHitPoint() - 1);
        
        // 1個前の体を削除する
        m_parentEnemy->setHitPoint(0);
    }
    // 1個前が頭の場合
    else {
        
        // 頭を破壊する
        m_parentEnemy->setHitPoint(0);
        
    }
}

/*!
 @brief ウジの破壊処理
 
 親キャラクターのハエのヒットポイントを減らす。
 破壊エフェクトは通常のザコ敵の処理を行う。
 @param data ゲームデータ
 */
void AKEnemy::destroyOfMaggot(AKPlayDataInterface *data)
{
    // 状態
    enum STATE {
        kAKStateInit = 0,       // 初期状態
        kAKStateWait,           // 待機状態
        kAKStateEntry1,         // 登場1
        kAKStateEntry2,         // 登場2
        kAKStateEntry3,         // 登場3
        kAKStateCount           // 状態の種類の数
    };

    // 親キャラクターが設定されていない場合はエラー
    if (m_parentEnemy == NULL) {
        AKAssert(false, "m_parentEnemy is NULL.");
        return;
    }
    
    // 親キャラクターのヒットポイントを減らす
    m_parentEnemy->setHitPoint(m_parentEnemy->getHitPoint() - 1);
    
    AKLog(kAKLogEnemy_3, "parentHP=%d", m_parentEnemy->getHitPoint());
    
    // ハエのヒットポイントの初期値になった場合はすべてのウジが倒されたものとして
    // ハエの状態を遷移する
    if (m_parentEnemy->getHitPoint() <= kAKEnemyDef[kAKEnemyFly - 1].hitPoint) {
        
        AKLog(kAKLogEnemy_3, "ハエ登場");
        
        // ハエの状態を登場状態に遷移する
        m_parentEnemy->setState(kAKStateEntry1);
        
        // ハエの位置を自分の位置に設定する
        m_parentEnemy->setPosition(m_position);
    }
    
    // ザコ敵の破壊処理を行う
    destroyNormal(data);
}

/*!
 @brief 逆さま判定
 
 上方向にある障害物と下方向にある障害物の近い方へ位置を移動する。
 上方向の方が近い場合は天井張り付き、下方向の方が近い場合は床に張り付きとする。
 存在しない場合は無限遠にあるものとして判定し、上下同じ場合は下側を優先する。
 @param blocks 障害物
 */
void AKEnemy::checkReverse(const std::vector<AKBlock*> &blocks)
{
    // 上方向距離と下方向距離の初期値を設定する
    float upDistance = FLT_MAX;
    float downDistance = FLT_MAX;
    
    // 移動先位置の初期値を設定する
    float upPosition = getImage()->getContentSize().height / 2;
    float downPosition = getImage()->getContentSize().height / 2;

    // 各障害物との距離を調べる
    for (AKCharacter *block : blocks) {

        // 配置されていないブロックは処理を飛ばす
        if (!block->isStaged()) {
            continue;
        }
        
        // x軸方向に重なりがない場合は処理を飛ばす
        if (fabsf(m_position.x - block->getPosition()->x) >
                (getImage()->getContentSize().width + block->getImage()->getContentSize().width) / 2) {
            continue;
        }
        
        // 障害物が上方向にある場合は上方向距離を更新する
        if (m_position.y < block->getPosition()->y) {
            
            // 距離が小さい場合は距離と移動先位置を更新する
            if (block->getPosition()->y - m_position.y < upDistance) {
                
                upDistance = block->getPosition()->y - m_position.y;
                upPosition = block->getPosition()->y - (block->getSize()->height + getImage()->getContentSize().height) / 2;
            }
        }
        // 上方向にない場合は下方向距離を更新する
        else {
            
            // 距離が小さい場合は距離と移動先位置を更新する
            if (m_position.y - block->getPosition()->y < downDistance) {
                
                downDistance = m_position.y - block->getPosition()->y;
                downPosition = block->getPosition()->y + (block->getSize()->height + getImage()->getContentSize().height) / 2;
            }
        }
    }
    
    // 上方向の距離が小さい場合は上方向に移動して、逆さにする
    if (upDistance < downDistance) {
        m_position.y = upPosition;
        getImage()->setFlippedY(true);
    }
    // 下方向の距離が小さい場合は下方向に移動する
    else {
        m_position.y = downPosition;
    }
}
