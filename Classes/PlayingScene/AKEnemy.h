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
 @file AKEnemy.h
 @brief 敵クラス定義
 
 敵キャラクターのクラスの定義をする。
 */

#ifndef AKENEMY_H
#define AKENEMY_H

#include <queue>
#include "AKCharacter.h"
#include "AKNWayAngle.h"

/*!
 @brief 敵クラス
 
 敵キャラクターのクラス。
 */
class AKEnemy : public AKCharacter {
public:
    /// 動作処理関数
    using AKActionFunc = void (AKEnemy::*)(AKPlayDataInterface *data);
    /// 作業領域の要素数
    static const int kAKEnemyWorkCount = 5;

private:
    /// 敵の種類の数
    static const int kAKEnemyDefCount = 40;

    /// 敵の定義
    static const struct AKEnemyDef kAKEnemyDef[kAKEnemyDefCount];

private:
    // 自機を狙うn-way弾発射
    static void fireNWay(cocos2d::CCPoint position,
                         int count,
                         float interval,
                         float speed,
                         AKPlayDataInterface *data);
    // 角度指定によるn-way弾発射
    static void fireNWay(float angle,
                         cocos2d::CCPoint position,
                         int count,
                         float interval,
                         float speed,
                         bool isScroll,
                         AKPlayDataInterface *data);
    // 自機を狙うグループ弾発射
    static void fireGroupShot(cocos2d::CCPoint position,
                              const float distance[][2],
                              int count,
                              float speed,
                              AKPlayDataInterface *data);
    // 破裂弾発射
    static void fireBurstShot(cocos2d::CCPoint position,
                              int count,
                              float interval,
                              float speed,
                              float burstInterval,
                              float burstSpeed,
                              AKPlayDataInterface *data);
    // 障害物との衝突判定
    static cocos2d::CCPoint checkBlockPosition(const cocos2d::CCPoint &current,
                                               const cocos2d::CCSize &size,
                                               bool isReverse,
                                               AKPlayDataInterface *data);
    // 足元の障害物を取得する
    static AKCharacter* getBlockAtFeet(float x,
                                       float from,
                                       bool isReverse,
                                       const std::vector<AKBlock*> &blocks);

private:
    /// 動作開始からの経過フレーム数(各敵種別で使用)
    int m_frame;
    /// 動作状態(各敵種別で使用)
    int m_state;
    /// 作業領域(各敵種別で使用)
    int m_work[kAKEnemyWorkCount];
    /// 動作処理のセレクタ
    AKActionFunc m_action;
    /// 破壊処理のセレクタ
    AKActionFunc m_destroy;
    /// スコア
    int m_score;
    /// 倒した時に進む進行度
    int m_progress;
    /// 親キャラクター
    AKEnemy *m_parentEnemy;
    /// 子キャラクター
    AKEnemy *m_childEnemy;
    /// 移動履歴を保存する個数
    int m_historyCount;
    /// 移動履歴
    std::queue<cocos2d::CCPoint> m_moveHistory;
    
public:
    // 生成処理
    void createEnemy(int type, const cocos2d::CCPoint &position, int progress, cocos2d::CCNode *parent);
    // 親キャラクター設定
    void setParentEnemy(AKEnemy *parent);
    // 小キャラクター設定
    void setChildEnemy(AKEnemy *child);
    // 状態設定
    void setState(int state);
    // 移動履歴取得
    const std::queue<cocos2d::CCPoint>* getMoveHistory();

protected:
    // キャラクター固有の動作
    virtual void action(AKPlayDataInterface *data);
    // 破壊処理
    virtual void destroy(AKPlayDataInterface *data);

private:
    // トンボの動作処理
    void actionOfDragonfly(AKPlayDataInterface *data);
    // アリの動作処理
    void actionOfAnt(AKPlayDataInterface *data);
    // チョウの動作処理
    void actionOfButterfly(AKPlayDataInterface *data);
    // テントウムシの動作処理
    void actionOfLadybug(AKPlayDataInterface *data);
    // ミノムシの動作処理
    void actionOfBagworm(AKPlayDataInterface *data);
    // セミの動作処理
    void actionOfCicada(AKPlayDataInterface *data);
    // バッタの動作処理
    void actionOfGrasshopper(AKPlayDataInterface *data);
    // ハチの動作処理
    void actionOfHornet(AKPlayDataInterface *data);
    // ゴキブリの動作処理
    void actionOfCockroach(AKPlayDataInterface *data);
    // カタツムリの動作処理
    void actionOfSnail(AKPlayDataInterface *data);
    // クワガタの動作処理
    void actionOfStagBeetle(AKPlayDataInterface *data);
    // カブトムシの動作処理
    void actionOfRhinocerosBeetle(AKPlayDataInterface *data);
    // カマキリの動作処理
    void actionOfMantis(AKPlayDataInterface *data);
    // ハチの巣の動作処理
    void actionOfHoneycomb(AKPlayDataInterface *data);
    // クモの動作処理
    void actionOfSpider(AKPlayDataInterface *data);
    // ムカデ（頭）の動作処理
    void actionOfCentipedeHead(AKPlayDataInterface *data);
    // ムカデ（胴体）の動作処理
    void actionOfCentipedeBody(AKPlayDataInterface *data);
    // ムカデ（尻尾）の動作処理
    void actionOfCentipedeTail(AKPlayDataInterface *data);
    // 雑魚敵の破壊処理
    void destroyNormal(AKPlayDataInterface *data);
    // 逆さま判定
    void checkReverse(const std::vector<AKBlock*> &blocks);
};

#endif
