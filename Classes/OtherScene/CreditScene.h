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
#include "PageScene.h"
#include "base/CCController.h"

/*!
 @brief クレジットシーンクラス
 
 クレジットシーンを表示する。
 */
class CreditScene : public PageScene {
public:
    
    /*!
     @brief コンビニエンスコンストラクタ
     
     インスタンスを生成し、初期化処理を行い、autoreleaseを行う。
     @return 生成したインスタンス
     */
    static CreditScene* create();

    /*!
     @brief コンストラクタ
     
     初期化処理を行う。
     */
    CreditScene();
    
protected:
    
    /*!
     @brief 派生クラスの初期化処理
     
     派生クラスの初期化処理を行う。
     */
    virtual bool initSub();

    /*!
     @brief 派生クラスのイベント処理
     
     派生クラスのイベント処理を行う。
     @param pageNo ページ番号
     @param item 選択されたメニュー項目
     */
    virtual void execSubEvent(int pageNo, const AKMenuItem *item);
    
    /*!
     @brief コントローラのAボタンを押した時の処理
     
     コントローラのAボタンを押した時の処理を行う。
     @param pageNo ページ番号
     @param cursorPosition カーソル位置
     */
    virtual void onKeyDownAButton(int pageNo, int cursorPosition);
    
    /*!
     @brief コントローラのLスティックを上に倒した時の処理
     
     コントローラのLスティックを上に倒した時の処理を行う。
     @param pageNo ページ番号
     @param cursorPosition 入力前のカーソル位置
     @return 入力後のカーソル位置
     */
    virtual int onLStickUp(int pageNo, int cursorPosition);
    
    /*!
     @brief コントローラのLスティックを下に倒した時の処理
     
     コントローラのLスティックを下に倒した時の処理を行う。
     @param pageNo ページ番号
     @param cursorPosition 入力前のカーソル位置
     @return 入力後のカーソル位置
     */
    virtual int onLStickDown(int pageNo, int cursorPosition);

    /*!
     @brief ページ表示内容更新
     
     ページ番号に合わせて、ページ表示内容を更新する。
     @param pageNo ページ番号
     @return 有効にするインターフェースタグ
     */
    virtual unsigned int updatePageContents(int pageNo);

    /*!
     @brief カーソル表示有無取得
     
     カーソル表示を行うかどうかを取得する。
     @param pageNo ページ番号
     @return 表示する場合true、表示しない場合false
     */
    virtual bool isVisibleCursor(int pageNo);
    
    /*!
     @brief カーソル位置取得
     
     カーソル位置を取得する。
     @param pageNo ページ番号
     @param positionID カーソル位置ID
     @return カーソル位置座標
     */
    virtual cocos2d::Vec2 getCursorPosition(int pageNo, int positionID);
    
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
    
    // カーソル画像の位置のボタンとの重なりの幅
    static const float CursorPosOverlap;
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
    // リンクボタンのタグ
    static const unsigned int LinkTag[];
    
    /// クレジットラベル
    AKLabel *m_creditLabel[LinkNumOfPage];
    /// リンクボタン
    AKLabel *m_link[LinkNumOfPage];
    
    /*!
     @brief リンクボタン押下
     
     選択したリンクを開く。
     @param pageNo ページ番号
     @param linkNumber リンクボタンの番号
     */
    void openLink(int pageNo, int linkNumber);
    
    /*!
     @brief リンクボタン表示更新
     
     ページ番号に応じてリンクボタンの表示を更新する。
     @param pageNo ページ番号
     @return 有効にするインターフェースタグ
     */
    unsigned int updateLinkButton(int pageNo);
    
    /*!
     @brief クレジットラベル表示更新
     
     表示しているページに合わせてクレジットラベルの表示内容を更新する。
     @param pageNo ページ番号
     */
    void updateCreditLabel(int pageNo);
    
    /*!
     @brief ページ内項目数計算
     
     指定ページのリンク項目数を計算する。
     @return ページ内項目数
     */
    int getLinkNum(int pageNo);
    
    // メニュー項目選択
    void selectMenuItem(int item);
};

#endif /* defined(__toritoma__CreditScene__) */
