/*
 * Copyright (c) 2015 Akihiro Kaneda.
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
 @file CreditScene.h
 @brief クレジットシーンクラスの定義
 
 クレジットシーンを表示するクラスを定義する。
 */

#ifndef __toritoma__CreditScene__
#define __toritoma__CreditScene__

#include "AKToritoma.h"
#include "AKMenuEventHandler.h"
#include "base/CCController.h"

/*!
 @brief クレジットシーンクラス
 
 クレジットシーンを表示する。
 */
class CreditScene : public cocos2d::Scene, AKMenuEventHandler {
private:
    
    /// メニューイベント番号
    enum MenuEvent {
        MenuBackToTitle = 0,    ///< タイトルへ戻るボタン
        MenuPrevPage,           ///< 前ページボタン
        MenuNextPage,           ///< 次ページボタン
        MenuLink1,              ///< リンクボタン1
        MenuLink2,              ///< リンクボタン2
        MenuLink3               ///< リンクボタン3
    };
    
    /// シーンに配置するノードのz座標
    enum PosZ {
        PosZBackGround = 0, ///< 背景のz座標
        PosZItems,          ///< ボタンのz座標
        PosZInterface,      ///< インターフェースのz座標
        PosZCursor          ///< カーソルのz座標
    };
    
    // 前ページボタンの位置、左からの位置
    static const float PrevPosLeftPoint;
    // 次ページボタンの位置、右からの位置
    static const float NextPosRightPoint;
    // ページ番号の位置、上からの位置
    static const float PagePosTopPoint;
    // 戻るボタンの位置、右からの位置
    static const float BackPosRightPoint;
    // 戻るボタンの位置、上からの位置
    static const float BackPosTopPoint;
    // Lボタンの位置、左からの位置
    static const float LButtonPosLeftPoint;
    // Lボタンの位置、中心からの縦方向の位置
    static const float LButtonPosVerticalCenterPoint;
    // Rボタンの位置、右からの位置
    static const float RButtonPosRightPoint;
    // Rボタンの位置、中心からの縦方向の位置
    static const float RButtonPosVerticalCenterPoint;
    // Bボタンの位置、右からの位置
    static const float BButtonPosRightPoint;
    // Bボタンの位置、上からの位置
    static const float BButtonPosTopPoint;
    // カーソル画像の位置のボタンとの重なりの幅
    static const float CursorPosOverlap;
    // 前ページボタンの画像ファイル名
    static const char *PrevImage;
    // 次ページボタンの画像ファイル名
    static const char *NextImage;
    // 戻るボタンの画像ファイル名
    static const char *BackImage;
    // ページ数表示のフォーマット
    static const char *PageFormat;
    // リンクの個数
    static const int LinkNum;
    // 1ページあたりのリンクの個数
    static const int LinkNumOfPage = 3;
    // 最大ページ番号
    static const int MaxPageNum;
    // クレジットラベルの位置
    static const float CreditPosLeftRatio;
    // クレジットラベルの行数
    static const int CreditLineNum;
    // クレジットラベルの1行の文字数
    static const int CreditLineLength;
    // クレジットラベルテキストのキー
    static const char *CreditTextKey;
    /// クレジットURLテキストのキー
    static const char *UrlTextKey;
    // リンクボタンのキャプション
    static const char *LinkCaption;
    // リンクボタンの位置、左からの比率
    static const float LinkPosLeftRatio;
    // リンクボタンの位置、上からの比率
    static const float LinkPosTopRatio[];
    // 前ページボタンのタグ
    static const unsigned int PrevTag;
    // 次ページボタンのタグ
    static const unsigned int NextTag;
    // 戻るボタンのタグ
    static const unsigned int BackTag;
    // リンクボタンのタグ
    static const unsigned int LinkTag[];
    
private:
    /// インターフェースレイヤー
    AKInterface *m_interface;
    /// ページ番号ラベル
    AKLabel *m_pageLabel;
    /// クレジットラベル
    AKLabel *m_creditLabel[LinkNumOfPage];
    /// リンクボタン
    AKLabel *m_link[LinkNumOfPage];
    /// ページ番号
    int m_pageNo;
    /// Bボタン画像
    cocos2d::Sprite *m_bButton;
    /// Lボタン画像
    cocos2d::Sprite *m_lButton;
    /// Rボタン画像
    cocos2d::Sprite *m_rButton;
    /// カーソル位置
    int m_selectMenu;
    /// カーソル画像
    cocos2d::Sprite *m_cursor;
    
public:
    // コンビニエンスコンストラクタ
    static CreditScene* create();
    // デストラクタ
    ~CreditScene();
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
    // タイトルへ戻る
    void backToTitle();
    // 前ページ表示
    void goPrevPage();
    // 次ページ表示
    void goNextPage();
    // リンクボタン押下
    void openLink(int linkNumber);
    // ページ番号設定
    void setPageNo(int pageNo);
    // ボタン表示非表示更新
    void updateButton();
    // ページ番号表示更新
    void updatePageLabel();
    // クレジットラベル表示更新
    void updateCreditLabel();
    // メニュー項目選択
    void selectMenuItem(int item);
};

#endif /* defined(__toritoma__CreditScene__) */
