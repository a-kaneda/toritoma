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
 @file AKPlayData.h
 @brief ゲームデータ
 
 プレイ画面シーンのデータを管理するクラスを定義する。
 */

#ifndef AKPLAYDATA_H
#define AKPLAYDATA_H

#include "AKToritoma.h"
#include "AKPlayer.h"
#include "AKPlayerShot.h"
#include "AKTileMap.h"
#include "AKCharacterPool.h"
#include "AKEnemyShot.h"
#include "AKEnemy.h"
#include "AKEffect.h"
#include "AKBlock.h"
#include "AKPlayDataInterface.h"

// ステージの数
extern const int kAKStageCount;

class AKPlayingScene;

/*!
 @brief ゲームデータ
 
 プレイ画面のゲームデータを管理する。
 */
class AKPlayData : public AKPlayDataInterface {
private:
    /// シーンクラス(弱い参照)
    AKPlayingScene *m_scene;
    /// ステージ番号
    int m_stage;
    /// クリア後の待機フレーム数
    int m_clearWait;
    /// 復活待機フレーム数
    int m_rebirthWait;
    /// 残機
    int m_life;
    /// スコア
    int m_score;
    /// ハイスコア
    int m_hiScore;
    /// スクリプト情報
    AKTileMap *m_tileMap;
    /// 自機
    AKPlayer *m_player;
    /// 自機弾プール
    AKCharacterPool<AKPlayerShot> m_playerShotPool;
    /// 反射弾プール
    AKCharacterPool<AKEnemyShot> m_reflectShotPool;
    /// 敵キャラプール
    AKCharacterPool<AKEnemy> m_enemyPool;
    /// 敵弾プール
    AKCharacterPool<AKEnemyShot> m_enemyShotPool;
    /// 画面効果プール
    AKCharacterPool<AKEffect> m_effectPool;
    /// 障害物プール
    AKCharacterPool<AKBlock> m_blockPool;
    /// キャラクター配置バッチノード
    std::vector<cocos2d::SpriteBatchNode*> m_batches;
    /// シールドモード
    bool m_shield;
    /// ホールドモード
    bool m_hold;
    /// x軸方向のスクロールスピード
    float m_scrollSpeedX;
    /// y軸方向のスクロールスピード
    float m_scrollSpeedY;
    /// x軸方向の自機スピード
    float m_playerSpeedX;
    /// y軸方向の自機スピード
    float m_playerSpeedY;
    /// ボスキャラ
    AKEnemy *m_boss;
    /// ボスキャラの初期HP
    int m_bossHP;
    /// 何周目か
    int m_loopCount;

private:
    // デフォルトコンストラクタは使用禁止にする
    AKPlayData();

public:
    // シーンを指定したコンストラクタ
    AKPlayData(AKPlayingScene *scene);
    // デストラクタ
    ~AKPlayData();
    // x軸方向のスクロールスピード取得
    virtual float getScrollSpeedX();
    // x軸方向のスクロールスピード設定
    virtual void setScrollSpeedX(float speed);
    // y軸方向のスクロールスピード取得
    virtual float getScrollSpeedY();
    // y軸方向のスクロールスピード設定
    virtual void setScrollSpeedY(float speed);
    // 自機の位置情報取得
    virtual const cocos2d::Vec2* getPlayerPosition();
    // 障害物キャラクター取得
    virtual const std::vector<AKBlock*>* getBlocks();
    // デバイス座標からタイル座標の取得
    virtual cocos2d::Vec2 convertDevicePositionToTilePosition(cocos2d::Vec2 devicePosition);
    // 自機弾生成
    virtual void createPlayerShot(cocos2d::Vec2 position);
    // オプション弾生成
    virtual void createOptionShot(cocos2d::Vec2 position);
    // 反射弾生成
    virtual void createReflectShot(AKEnemyShot *enemyShot);
    // 敵生成
    virtual AKEnemy* createEnemy(int type, cocos2d::Vec2 position, int progress);
    // 敵弾インスタンスの取得
    virtual AKEnemyShot* getEnemyShot();
    // 敵弾配置ノードの取得
    virtual cocos2d::Node* getEnemyShotParent();
    // 画面効果生成
    virtual void createEffect(int type, cocos2d::Vec2 position);
    // 障害物生成
    virtual void createBlock(int type, cocos2d::Vec2 position);
    // 失敗時処理
    virtual void miss();
    // スコア加算
    virtual void addScore(int score);
    // 進行度を進める
    virtual void addProgress(int progress);
    // チキンゲージ増加
    virtual void addChickenGauge(int inc);
    // 2周目かどうか
    virtual bool is2ndLoop();
    // 状態更新
    void update();
    // 自機の移動
    void movePlayer(float dx, float dy);
    // 自機のx方向速度設定
    void setPlayerSpeedX(float speedX);
    // 自機のy方向速度設定
    void setPlayerSpeedY(float speedY);
    // ポーズ
    void pause();
    // ハイスコアファイル書込
    void writeHiScore();
    // スクリプト読み込み
    void readScript(int stage);
    // ゲーム再開
    void resume();
    // シールドモード設定
    void setShield(bool shield);
    // ホールドモード切替
    void changeHoldMode();
    // ステージ再開
    void restartStage(int stage);
    // ツイートメッセージの作成
    std::string makeTweet();

private:
    // メンバオブジェクト生成処理
    void createMember();
    // 初期値設定処理
    void clearPlayData(bool resetScore);
    // ハイスコアファイル読込
    void readHiScore();
    // 残機設定
    void setLife(int life);
    // 敵弾削除
    void clearEnemyShot();
    // ボス体力ゲージ表示更新
    void updateBossLifeGage();
    // ステージ変更
    void changeStage(int stage);
};

#endif
