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
 @file AKTitleScene.h
 @brief タイトルシーンクラスの定義
 
 タイトルシーンを管理するクラスを定義する。
 */

#ifndef AKTITLESCENE_H
#define AKTITLESCENE_H

#include "AKToritoma.h"
#include "AKMenuEventHandler.h"
#include "base/CCController.h"

/*!
 @brief タイトルシーンクラス
 
 タイトルシーンを管理する。
 */
class AKTitleScene : public cocos2d::Scene, AKMenuEventHandler {
private:
    
    /// メニュー項目
    enum MenuItem {
        MenuNone = 0,       ///< メニュー未選択（コントローラー未接続）
        MenuStartGame,      ///< ゲーム開始
        MenuHowToPlay,      ///< 遊び方
        MenuOption,         ///< オプション
        MenuCredit,         ///< クレジット
        MenuSentinel        ///< メニュー項目終端
    };
    
public:
    // コンビニエンスコンストラクタ
    static AKTitleScene* create();

private:
    /// インターフェースレイヤー
    AKInterface *m_interface;
    /// インタースティシャル広告を表示するかどうか
    bool m_isViewInterstitial;
    /// カーソル位置
    MenuItem m_selectMenu;
    /// カーソル画像
    cocos2d::Sprite *m_cursor;

public:
    // イベント実行
    virtual void execEvent(const AKMenuItem *item);
    // 初期化処理
    virtual bool init();
    // 画面遷移終了時処理
    virtual void onEnterTransitionDidFinish();
    // 更新処理
    virtual void update(float delta);
    // インタースティシャル広告表示を行う
    void viewInterstitial();
    // コントローラー接続時処理
    void onConnectedController(cocos2d::Controller* controller, cocos2d::Event* event);
    // コントローラー切断時処理
    void onDisconnectedController(cocos2d::Controller* controller, cocos2d::Event* event);
    // コントローラーのボタンを押した時の処理
    void onKeyDown(cocos2d::Controller* controller, int keyCode, cocos2d::Event* event);
    // コントローラーのボタンを離した時の処理
    void onKeyUp(cocos2d::Controller* controller, int keyCode, cocos2d::Event* event);
    // コントローラーの方向キー入力処理
    void onAxisEvent(cocos2d::Controller* controller, int keyCode, cocos2d::Event* event);
    
private:
    // コンストラクタ
    AKTitleScene();
    // ゲーム開始ボタンタッチ
    void touchGameStartButton();
    // 遊び方ボタンタッチ
    void touchHowToButton();
    // オプションボタンタッチ
    void touchOptionButton();
    // クレジットボタンタッチ
    void touchCreditButton();
    // ボタン選択エフェクト
    void selectButton(int tag);
    // メニュー項目選択
    void selectMenuItem(MenuItem item);
};

#endif
