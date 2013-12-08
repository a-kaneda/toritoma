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
 @file AKPlayingScene.h
 @brief プレイシーンの定義
 
 プレイシーンクラスを定義する。
 */

#ifndef AKPLAYINGSCENE_H
#define AKPLAYINGSCENE_H

#include "AKToritoma.h"
#include "AKPlayingSceneIF.h"
#include "AKPlayData.h"
#include "AKChickenGauge.h"
#include "AKLife.h"
#include "AKTitleScene.h"

/// ゲームプレイの状態
enum AKGameState {
    kAKGameStatePreLoad = 0,    ///< ゲームシーン読み込み前
    kAKGameStateStart,          ///< ゲーム開始時
    kAKGameStatePlaying,        ///< プレイ中
    kAKGameStateStageClear,     ///< ステージクリア後
    kAKGameStateResult,         ///< リザルト画面
    kAKGameStateGameOver,       ///< ゲームオーバー
    kAKGameStateGameClear,      ///< ゲームクリア時
    kAKGameStatePause,          ///< 一時停止中
    kAKGameStateQuitMenu,       ///< 終了メニュー
    kAKGameStateWait,           ///< アクション終了待機中
    kAKGameStateSleep           ///< スリープ処理中
};

/*!
 @brief プレイシーンクラス
 
 プレイ中画面のシーンを管理する。
 */
class AKPlayingScene : public cocos2d::CCScene, AKMenuEventHandler {
public:
    // コンビニエンスコンストラクタ
    static AKPlayingScene* create();

private:
    /// ゲームデータ
    AKPlayData *m_data;
    /// ゲームプレイの状態
    enum AKGameState m_state;
    /// スリープ後に遷移する状態
    enum AKGameState m_nextState;
    /// スリープフレーム数
    int m_sleepFrame;
    /// 背景レイヤー
    cocos2d::CCLayer *m_backgroundLayer;
    /// キャラクターレイヤー
    cocos2d::CCLayer *m_characterLayer;
    /// 情報レイヤー
    cocos2d::CCLayer *m_infoLayer;
    /// インターフェースレイヤー
    AKPlayingSceneIF *m_interfaceLayer;
    /// スコア表示
    AKLabel *m_score;
    /// 残機表示
    AKLife *m_life;
    /// チキンゲージ
    AKChickenGauge *m_chickenGauge;

private:
    // コンストラクタ
    AKPlayingScene();

public:
    // デストラクタ
    ~AKPlayingScene();
    // バックグラウンド移行処理
    void onWillEnterForeground();
    // ゲームプレイの状態設定
    void setState(enum AKGameState state);
    // 背景レイヤー取得
    cocos2d::CCLayer* getBackgroundLayer();
    // キャラクターレイヤー取得
    cocos2d::CCLayer* getCharacterLayer();
    // 情報レイヤー取得
    cocos2d::CCLayer* getInfoLayer();
    // インターフェースレイヤー取得
    AKPlayingSceneIF* getInterfaceLayer();
    // 残機表示取得
    AKLife* getLife();
    // シールドボタン表示切替
    void setShieldButtonSelected(bool selected);
    // トランジション終了時の処理
    virtual void onEnterTransitionDidFinish();
    // 更新処理
    virtual void update(float delta);
    // イベント実行
    virtual void execEvent(const AKMenuItem *item);
    // チキンゲージ取得
    AKChickenGauge* getChickenGauge();
    // スコアラベル取得
    AKLabel* getScore();
    // ゲームオーバーかどうか取得
    bool isGameOver();
    // ゲームオーバー
    void gameOver();
    // ステージクリア
    void stageClear();
    // 次のステージへ進める
    void nextStage();
    // スコアラベル更新
    void setScoreLabel(int score);
    
private:
    // 自機の移動
    void movePlayer(const AKMenuItem *item);
    // シールドボタン選択処理
    void touchShieldButton(const AKMenuItem *item);
    // ポーズボタン選択処理
    void touchPauseButton();
    // 再開ボタン選択処理
    void touchResumeButton();
    // 終了ボタン選択処理
    void touchQuitButton();
    // 終了メニューYESボタン選択処理
    void touchQuitYesButton();
    // 終了メニューNOボタン選択処理
    void touchQuitNoButton();
    // 背景レイヤー作成
    void createBackGround();
    // キャラクターレイヤー作成
    void createCharacterLayer();
    // 情報レイヤー作成
    void createInfoLayer();
    // インターフェースレイヤー作成
    void createInterface();
    // 枠レイヤー作成
    void createFrame();
    // 枠背景作成
    void createFrameBack();
    // 枠の棒作成
    void createFrameBar();
    // 枠ブロック配置
    void createFrameBlock(cocos2d::CCNode *node,
                          const char *name,
                          int size,
                          const cocos2d::CCRect &rect);
    // ツイートボタン選択処理
    void touchTweetButton();
    // ゲーム開始時の更新処理
    void updateStart();
    // プレイ中の更新処理
    void updatePlaying();
    // スリープ処理中の更新処理
    void updateSleep();
    // ゲーム再開
    void resume();
    // 終了メニュー表示
    void viewQuitMenu();
    // 一時停止メニュー表示
    void viewPauseMenu();
};

#endif
