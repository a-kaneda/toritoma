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

using cocos2d::CCPoint;
using cocos2d::CCSize;
using cocos2d::CCNode;

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
    int hitPoint;                   ///< ヒットポイント
    int score;                      ///< スコア
};

/// 画像名のフォーマット
static const char *kAKImageNameFormat = "Enemy_%02d";
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
    //破壊,画像,フレーム数,フレーム間隔,幅,高さ,HP,スコア
    {&AKEnemy::actionOfDragonfly, &AKEnemy::destroyNormal, 1, 2, 30, 32, 32, 0, 0, 3, 100},     // トンボ
    {&AKEnemy::actionOfAnt, &AKEnemy::destroyNormal, 2, 2, 30, 32, 16, 0, 0, 3, 100},           // アリ
    {&AKEnemy::actionOfButterfly, &AKEnemy::destroyNormal, 3, 2, 30, 32, 32, 0, 0, 3, 100},     // チョウ
    {&AKEnemy::actionOfLadybug, &AKEnemy::destroyNormal, 4, 2, 6, 32, 32, 0, 0, 5, 100},        // テントウムシ
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備5
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備6
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備7
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備8
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備9
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備10
    {&AKEnemy::actionOfBagworm, &AKEnemy::destroyNormal, 11, 1, 0, 32, 32, 0, 0, 10, 100},      // ミノムシ
    {&AKEnemy::actionOfCicada, &AKEnemy::destroyNormal, 12, 1, 0, 32, 32, 0, 0, 5, 100},        // セミ
    {&AKEnemy::actionOfGrasshopper, &AKEnemy::destroyNormal, 13, 1, 0, 32, 32, 0, 0, 3, 100},   // バッタ
    {&AKEnemy::actionOfHornet, &AKEnemy::destroyNormal, 14, 2, 6, 32, 32, 0, 0, 5, 100},        // ハチ
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備15
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備16
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備17
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備18
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備19
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備20
    {&AKEnemy::actionOfCockroach, &AKEnemy::destroyNormal, 21, 2, 6, 32, 32, 0, 0, 5, 100},     // ゴキブリ
    {&AKEnemy::actionOfSnail, &AKEnemy::destroyNormal, 22, 2, 30, 32, 32, 0, 0, 5, 100},        // カタツムリ
    {&AKEnemy::actionOfStagBeetle, &AKEnemy::destroyNormal, 23, 2, 6, 32, 32, 0, 0, 5, 100},    // クワガタ
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備24
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備25
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備26
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備27
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備28
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備29
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // 予備30
    {&AKEnemy::actionOfRhinocerosBeetle, &AKEnemy::destroyNormal, 31, 2, 3, 64, 40, 0, 0, 1000, 10000}, // カブトムシ
    {&AKEnemy::actionOfMantis, &AKEnemy::destroyNormal, 32, 0, 0, 64, 64, 0, 0, 1000, 10000},           // カマキリ
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // ハチの巣
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // クモ
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // ムカデ（頭）
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // ムカデ（胴体）
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // ムカデ（尾）
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // ウジ
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0},         // ハエ
    {NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0}          // 予備40
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
void AKEnemy::fireNWay(CCPoint position,
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
                       CCPoint position,
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
void AKEnemy::fireGroupShot(CCPoint position,
                            const float distance[][2],
                            int count,
                            float interval,
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
        
        // 通常弾を生成する
        CCPoint shotPosition(position.x + distance[i][0],
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
void AKEnemy::fireBurstShot(CCPoint position,
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
        
        // 破裂弾を生成する
        CCPoint shotPosition(position.x + cosf(angle) * kAKDistance,
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
CCPoint AKEnemy::checkBlockPosition(const CCPoint &current,
                                    const CCSize &size,
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
                                                     top,
                                                     isReverse,
                                                     *data->getBlocks());
    
    // 右端の座標を計算する
    float right = current.x+ size.width / 2.0f;

    // 左側の足元の障害物を取得する
    AKCharacter *rightBlock = AKEnemy::getBlockAtFeet(right,
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
        
        // 近い方のブロックに移動する
        if  (rightBlock->getPosition()->x - current.x < current.x - leftBlock->getPosition()->x) {
            newX = rightBlock->getPosition()->x - rightBlock->getSize()->width / 2.0f + size.width / 2.0f;
            blockAtFeet = rightBlock;
        }
        else {
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
    
    AKLog(kAKLogEnemy_2, "(%.0f, %.0f)->(%.0f, %.0f)", current.x, current.y, newX, newY);
    
    // 移動先の座標を返す
    return CCPoint(newX, newY);
}

/*!
 @brief 足元の障害物を取得する
 
 足元の障害物を取得する。
 指定したx座標で一番上にある障害物を取得する。ただし、頭よりも上にある障害物は除外する。
 逆さまになっている場合は上下を逆にして検索を行う。
 @param x x座標
 @param top 頭の位置
 @param isReverse 逆さまになっているかどうか
 @param blocks 障害物
 @return 足元の障害物
 */
AKCharacter* AKEnemy::getBlockAtFeet(float x,
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
        
        // 障害物の幅の範囲内に指定座標が入っている場合
        if (roundf(block->getPosition()->x - block->getSize()->width / 2) <= roundf(x) &&
            roundf(block->getPosition()->x + block->getSize()->width / 2) >= roundf(x)) {
            
            // 逆さまでない場合
            if (!isReverse) {
                
                // 障害物の下端が自分の上端よりも下にあるものの内、
                // 一番上にあるものを採用する
                if (roundf(block->getPosition()->y - block->getSize()->height / 2) <= roundf(from) &&
                    (blockAtFeet == NULL || block->getPosition()->y + block->getSize()->height / 2 > blockAtFeet->getPosition()->y + blockAtFeet->getSize()->height / 2)) {
                    
                    blockAtFeet = block;
                }
            }
            // 逆さまの場合
            else {
                
                // 障害物の上端が自分の下端より上にあるものの内、
                // 一番下にあるものを採用する
                if (roundf(block->getPosition()->y + block->getSize()->height / 2) > roundf(from) &&
                    (blockAtFeet == NULL || block->getPosition()->y - block->getSize()->height / 2 < blockAtFeet->getPosition()->y - blockAtFeet->getSize()->height / 2)) {
                    
                    blockAtFeet = block;
                }
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
// 生成処理
void AKEnemy::createEnemy(int type,
                          const CCPoint &position,
                          int progress,
                          CCNode *parent)
{
    // パラメータの内容をメンバに設定する
    m_position = position;
    m_progress = progress;
    
    // 配置フラグを立てる
    m_isStaged = true;
    
    // 各メンバ変数を初期化する
    m_frame = 0;
    m_state = 0;
    for (int i = 0; i < kAKEnemyWorkCount; i++) {
        m_work[i] = 0;
    }
    
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
    
    // TODO:当たり判定のオフセットを設定する
    
    // ヒットポイントを設定する
    m_hitPoint = kAKEnemyDef[type - 1].hitPoint;
    
    // スコアを設定する
    m_score = kAKEnemyDef[type - 1].score;
    
    // 画像の回転をリセットする
    getImage()->setRotation(0.0f);
    
    // レイヤーに配置する
    parent->addChild(getImage());
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
    // 破壊時の効果音を鳴らす
//    [[SimpleAudioEngine sharedEngine] playEffect:kAKHitSE];
    
    // スコアを加算する
    data->addScore(m_score);
    
    // 進行度を進める
    data->addProgress(m_progress);
    
    // 敵種別ごとの処理を実行
    (this->*m_destroy)(data);
    
    // スーパークラスの処理を行う
    AKCharacter::destroy(data);
}

/*!
 @brief トンボの動作処理
 
 まっすぐ進む。一定間隔で左方向へ1-way弾発射。
 @param data ゲームデータ
 */
void AKEnemy::actionOfDragonfly(AKPlayDataInterface *data)
{
    // 弾のスピード
    const float kAKShotSpeed = 3.0f;
    // 移動スピード
    const float kAKMoveSpeed = -1.5f;
    // 弾発射間隔
    const int kAKShotInterval = 60;

    // 左へ直進する
    m_speedX = kAKMoveSpeed;
    m_speedY = 0.0f;
    
    // 一定時間経過しているときは左方向へ1-way弾を発射する
    if ((m_frame + 1) % kAKShotInterval == 0) {
        
        // 左へ弾を発射する
        AKEnemy::fireNWay(M_PI,
                          CCPoint(m_position.x, m_position.y),
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
    const float kAKShotSpeed = 2.0f;
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
            getImage()->setFlipY(false);
            
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
            getImage()->setFlipY(true);
            
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
                getImage()->setFlipY(true);
            }
            else {
                getImage()->setFlipY(false);
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
                                             getImage()->isFlipY(),
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
    const float kAKShotSpeed = 3.0f;
    
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
    const float kAKShotSpeed = 2.0f;

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
    const float kAKShotSpeed = 2.0f;

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
    const float kAKShotSpeed = 2.0f;

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
            float angle = AKNWayAngle::calcDestAngle(m_position, *data->getPlayerPosition());
            
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
    const float kAKShotSpeed = 2.0f;
    
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
        if (!getImage()->isFlipY()) {
            m_speedY -= kAKGravitationAlacceleration;
        }
        else {
            m_speedY += kAKGravitationAlacceleration;
        }

        // 一定時間経過したら次の状態へ進める
        if (m_frame - m_work[0] > kAKLeftMoveInterval) {
            
            // 弾発射の状態へ遷移する
            m_state= kAKStateWait;
            
            // 停止する
            m_speedX = 0;
            
            // 作業領域に状態遷移した時のフレーム数を保存する
            m_work[0] = m_frame;
        }
    }
    
    // 落ちていく方向の障害物に接触している場合、着地したとしてスピードを0にする。
    if ((!getImage()->isFlipY() && (m_blockHitSide & kAKHitSideBottom)) ||
        (getImage()->isFlipY() && (m_blockHitSide & kAKHitSideTop))) {
        
        m_speedX = 0.0f;
        m_speedY = 0.0f;
    }
    
    // 初期状態または待機中で待機時間が経過している場合
    if ((m_state == kAKStateInit) || (m_state == kAKStateWait && (m_frame - m_work[0] > kAKWaitInterval))) {
        
        AKLog(kAKLogEnemy_3, "ジャンプ:frame=%d work[0]=%d", m_frame, m_work[0]);
        
        // 左方向へのスピードを設定する
        m_speedX = kAKMoveSpeed;
        
        // ジャンプする方向へ加速する
        if (!getImage()->isFlipY()) {
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
    const float kAKShotSpeed[kAKShotCount] = {3.2f, 3.4f, 3.6f, 3.8f, 4.0f};
    // 登場時のx方向の移動スピード
    const float kAKMoveInSpeedX = -2.0f;
    // 登場時のy方向の移動スピード
    const float kAKMoveInSpeedY = 1.0f;
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
            
            // 画面下半分に配置されている場合
            if (m_position.y < AKScreenSize::stageSize().height / 2) {
                // 上方向へ移動する
                m_speedY = kAKMoveInSpeedY;
            }
            // 画面上半分に配置されている場合
            else {
                // 下方向へ移動する
                m_speedY = -kAKMoveInSpeedY;
            }
            
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
    const float kAKMoveSpeed = 2.0f;
    // 弾のスピード
    const float kAKShotSpeed = 3.0f;
    // 弾発射間隔
    const int kAKShotInterval = 60;

    // 自機との角度を求める
    float angle = AKNWayAngle::calcDestAngle(m_position, *data->getPlayerPosition());
    
    // 縦横の速度を決定する
    m_speedX = kAKMoveSpeed * cosf(angle);
    m_speedY = kAKMoveSpeed * sinf(angle);
    
    // 画像を回転させる
    getImage()->setRotation(AKCnvAngleRad2Scr(angle));
    
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
    const float kAKShotSpeed = 2.0f;
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
                                             getImage()->isFlipY(),
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
    const float kAKShotSpeed = 3.0f;
    // 移動スピード
    const float kAKMoveSpeed = 2.0f;
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
            float angle = AKNWayAngle::calcDestAngle(m_position,
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
    const float kAKLeftShotSpeed = 3.0f;
    // 左方向への弾発射時の1-way弾の発射間隔
    const int kAKLeftShot1WayInterval = 60;
    // 左方向への弾発射時の1-way弾のスピード
    const float kAKLeftShot1WaySpeed = 2.0f;
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
    const float kAKNWaySpeed = 3.0f;
    // 全方位弾の発射間隔
    const int kAKAllDirectionInterval = 30;
    // 全方位弾の弾数
    const float kAKAllDirectionCount = 12;
    // 全方位弾の角度の間隔
    const float kAKAllDirectionAngle = 2 * M_PI / kAKAllDirectionCount;
    // 全方位弾のスピード
    const float kAKAllDirectionSpeed = 3.0f;
    
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
                CCPoint position(m_position.x, m_position.y);
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
    const CCPoint kAKHandPosition(-48.0f, 48.0f);
    // 定周期弾の発射間隔
    const int kAKCycleShotInterval = 60;
    // 定周期弾の弾数
    const int kAKCycleShotCount = 3;
    // 定周期弾の角度の間隔
    const float kAKCycleShotAngle = M_PI / 8;
    // 定周期弾のスピード
    const float kAKCycleShotSpeed = 3.0f;
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
    const float kAKArcShotSpeed = 2.5f;
    // 破裂弾の発射待機時間
    const int kAKBurstShotWaitTime = 90;
    // 破裂弾の発射間隔
    const int kAKBurstShotInterval = 60;
    // 破裂弾の破裂前のスピード
    const float kAKBurstShotBeforSpeed = 2.0f;
    // 破裂弾の破裂後のスピード
    const float kAKBurstShotAfterSpeed = 3.5f;
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
    const float kAKNWayShotSpeed = 2.5f;
    // n-way弾の弾数
    const int kAKNWayCount[2] = {9, 10};
    // n-way弾の角度の間隔
    const float kAKNWayAngleInterval = M_PI / 20;
    
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
                                       kAKArcShotInterval,
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
                CCPoint position(m_position.x + kAKHandPosition.x,
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
                CCPoint position(m_position.x + kAKHandPosition.x,
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
    
    // 振り上げている鎌の数に応じてグラフィックを変更する
    setAnimationInitPattern(m_work[0] + 1);
    
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
        m_work[1] = 0.0f;
        m_frame = 0.0f;
        
        AKLog(kAKLogEnemy_3, "m_state=%d", m_state);
    }
}

/*!
 @brief 雑魚敵の破壊処理
 
 破壊エフェクトを発生させる。
 @param data ゲームデータ
 */
void AKEnemy::destroyNormal(AKPlayDataInterface *data)
{
    AKLog(kAKLogEnemy_1, "start");
    
    // 画面効果を生成する
    data->createEffect(1, m_position);
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
        getImage()->setFlipY(true);
    }
    // 下方向の距離が小さい場合は下方向に移動する
    else {
        m_position.y = downPosition;
    }
}
