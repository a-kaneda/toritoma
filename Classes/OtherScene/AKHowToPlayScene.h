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
 @file AKHowToPlayScene.h
 @brief プレイ方法画面シーンクラスの定義
 
 プレイ方法画面シーンクラスを定義する。
 */

#ifndef AKHOWTOPLAYSCENE_H
#define AKHOWTOPLAYSCENE_H

#include "AKToritoma.h"
#include "base/CCController.h"

/*!
 @brief プレイ方法画面シーン
 
 プレイ方法画面のシーンを実現する。
 */
class AKHowToPlayScene : public cocos2d::Scene, AKMenuEventHandler {
public:
    // コンビニエンスコンストラクタ
    static AKHowToPlayScene* create();

private:
    /// ページ番号
    int m_pageNo;
    /// 説明画像
    cocos2d::Sprite *m_image;
    /// Bボタン画像
    cocos2d::Sprite *m_bButton;
    /// Lボタン画像
    cocos2d::Sprite *m_lButton;
    /// Rボタン画像
    cocos2d::Sprite *m_rButton;
    /// コントローラが接続されているかどうか
    bool m_isConnectController;

public:
    // デストラクタ
    ~AKHowToPlayScene();
    // 初期化処理
    virtual bool init();
    // イベント実行
    virtual void execEvent(const AKMenuItem *item);
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
    // インターフェース取得
    AKInterface* getInterface();
    // 前ページボタン取得
    cocos2d::Node* getPrevButton();
    // 次ページボタン取得
    cocos2d::Node* getNextButton();
    // ページ番号ラベル取得
    AKLabel* getPageLabel();
    // メッセージラベルの取得
    AKLabel* getMessageLabel();
    // ページ番号設定
    void setPageNo(int pageNo);
    // 前ページ次ページボタン表示非表示更新
    void updatePageButton();
    // ページ番号表示更新
    void updatePageLabel();
    // 前ページ表示
    void goPrevPage();
    // 次ページ表示
    void goNextPage();
    // タイトルへ戻る
    void backToTitle();
};

#endif
