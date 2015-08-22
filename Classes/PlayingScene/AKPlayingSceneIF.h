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
 @file AKPlayingSceneIF.h
 @brief プレイシーンインターフェースの定義
 
 プレイシーンのインターフェースクラスを定義する。
 */

#ifndef AKPLAYINGSCENEIF_H
#define AKPLAYINGSCENEIF_H

#include "AKToritoma.h"

// プレイ中メニュー項目のタグ
extern const unsigned int kAKMenuTagPlaying;
// ポーズ中メニュー項目のタグ
extern const unsigned int kAKMenuTagPause;
// 終了メニュー項目のタグ
extern const unsigned int kAKMenuTagQuit;
// ゲームオーバー時メニュー項目のタグ
extern const unsigned int kAKMenuTagGameOver;
// ステージクリア時メニュー項目のタグ
extern const unsigned int kAKMenuTagStageClear;
// 全ステージクリア待機時メニュー項目のタグ
extern const unsigned int kAKMenuTagAllStageWaitClear;
// 全ステージクリア時メニュー項目のタグ
extern const unsigned int kAKMenuTagAllStageClear;

// メニュー項目のイベント番号
enum {
    kAKEventTouchShieldButton = 0,  ///< シールドボタン
    kAKEventTouchHoldButton,        ///< ホールドボタン
    kAKEventTouchPauseButton,       ///< ポーズボタン
    kAKEventSlide,                  ///< 画面スライド
    kAKEventTouchResumeButton,      ///< ポーズ解除ボタン
    kAKEventTouchQuitButton,        ///< 終了ボタン
    kAKEventTouchQuitYesButton,     ///< 終了メニューYESボタン
    kAKEventTouchQuitNoButton,      ///< 終了メニューNOボタン
    kAKEventTouchBackButton,        ///< タイトルへ戻るボタン
    kAKEventTouchTwitterButton,     ///< Twitterボタン
    kAKEventTouchContinuePlayingButton  ///< 2周目続行ボタン
};

/*!
 @brief プレイシーンインターフェース
 
 プレイシーンのインターフェースを管理する。
 */
class AKPlayingSceneIF : public AKInterface {
public:
    // コンビニエンスコンストラクタ
    static AKPlayingSceneIF* createInterface(AKMenuEventHandler * const eventHandler);
    
private:
    /// シールドボタン
    cocos2d::Sprite *m_shieldButton;
    /// ホールドボタン
    cocos2d::Sprite *m_holdButton;
    /// ポーズ解除ボタン
    AKLabel *m_resumeButton;
    /// 終了ボタン
    AKLabel *m_quitButton;
    /// 終了メニューYesボタン
    AKLabel *m_quitYesButton;
    /// 終了メニューNoボタン
    AKLabel *m_quitNoButton;
    /// 2周目続行ボタン
    AKLabel *m_continuePlayingButton;
    /// ポーズメニューのカーソル
    cocos2d::Sprite *m_pauseCursor;
    /// 終了メニューのカーソル
    cocos2d::Sprite *m_quitCursor;
    /// ゲームオーバーメニューのカーソル
    cocos2d::Sprite *m_gameOverCursor;
    /// ゲームクリアメニューのカーソル
    cocos2d::Sprite *m_clearCursor;
    /// カーソルを表示するかどうか
    bool m_visibleCursor;
    /// カーソル位置
    int m_cursorPosition;
    
private:
    //デフォルトコンストラクタは使用禁止にする
    AKPlayingSceneIF();
    
public:
    // デストラクタ
    virtual ~AKPlayingSceneIF();
    // ポーズ解除ボタン取得
    AKLabel* getResumeButton();
    // 終了ボタン取得
    AKLabel* getQuitButton();
    // 終了メニューNoボタン取得
    AKLabel* getQuitNoButton();
    // 2周目続行ボタン取得
    AKLabel* getContinuePlayingButton();
    // シールドボタン表示切替
    void setShieldButtonSelected(bool selected);
    // ホールドボタン表示切替
    void setHoldButtonSelected(bool selected);
    // カーソル表示設定
    void setVisibleCursor(bool isVisible);
    // カーソル位置取得
    int getCursorPosition();
    // カーソル位置設定
    void setCursorPosition(int pos);
    
protected:
    // 項目数を指定したコンストラクタ
    AKPlayingSceneIF(AKMenuEventHandler * const eventHandler);
    
private:
    // プレイ中のメニュー項目作成
    void createPlayingMenu();
    // ポーズ時のメニュー項目作成
    void createPauseMenu();
    // 終了メニュー項目作成
    void createQuitMenu();
    // ゲームオーバー時のメニュー項目作成
    void createGameOverMenu();
    // ステージクリア時のメニュー項目作成
    void createStageClear();
    // 全ステージクリア時のメニュー項目作成
    void createAllStageClear();
    // メニュー項目個別表示設定
    virtual void updateVisible(cocos2d::Node *item);
};

#endif
