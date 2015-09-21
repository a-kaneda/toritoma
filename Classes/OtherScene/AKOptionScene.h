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
 @file AKOptionScene.h
 @brief オプション画面シーンクラスの定義
 
 オプション画面シーンクラスを定義する。
 */

#ifndef AKOPTIONSCENE_H
#define AKOPTIONSCENE_H

#include "AKToritoma.h"
#include "PageScene.h"
#include "PaymentDelegate.h"
#include "ActivityIndicator.h"

/*!
 @brief オプション画面シーン
 
 オプション画面のシーン。
 */
class AKOptionScene : public PageScene, aklib::PaymentDelegate {
public:
    
    /*!
     @brief コンビニエンスコンストラクタ
     
     インスタンスを生成し、初期化処理を行い、autoreleaseを行う。
     @return 生成したインスタンス
     */
    static AKOptionScene* create();
    
    /*!
     @brief コンストラクタ
     
     初期化処理を行う。
     */
    AKOptionScene();
    
    /*!
     @brief 課金完了
     
     課金処理完了時の処理を行う。
     通信中状態を終了し、操作可能にする。
     */
    virtual void completePayment();
    
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
     @brief コントローラのLスティックを左に倒した時の処理
     
     コントローラのLスティックを左に倒した時の処理を行う。
     @param pageNo ページ番号
     @param cursorPosition 入力前のカーソル位置
     @return 入力後のカーソル位置
     */
    virtual int onLStickLeft(int pageNo, int cursorPosition);
    
    /*!
     @brief コントローラのLスティックを右に倒した時の処理
     
     コントローラのLスティックを右に倒した時の処理を行う。
     @param pageNo ページ番号
     @param cursorPosition 入力前のカーソル位置
     @return 入力後のカーソル位置
     */
    virtual int onLStickRight(int pageNo, int cursorPosition);
    
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
    
    /// Game Centerページのカーソル位置
    enum CursorPositionAtGameCenterPage {
        CursorPositionLeaderboard = 0,  ///< Leaderboardボタンのカーソル位置
        CursorPositionAchievements      ///< Achievementsボタンのカーソル位置
    };
    
    /// Storeページのカーソル位置
    enum CursorPositionAtStorePage {
        CursorPositionBuy = 0,          ///< 購入ボタンのカーソル位置
        CursorPositionRestore           ///< リストアボタンのカーソル位置
    };
    
    /// z座標
    enum ItemZPositionEx {
        ZPositionConnectionLayer = ZPositionCount,  ///< 通信中レイヤー
        ZPositionIndicator                          ///< 通信中インジケータ
    };

    /// Leaderboardボタン
    AKLabel *m_leaderboardButton;
    /// Achievementsボタン
    AKLabel *m_achievementsButton;
    /// 購入ボタン
    AKLabel *m_buyButton;
    /// リストアボタン
    AKLabel *m_restoreButton;
    /// 通信中レイヤー
    cocos2d::LayerColor *m_connectingLayer;
    /// 通信中インジケータ
    ActivityIndicator *m_indicator;
    
    /*!
     @brief Game Centerページの項目作成
     
     Game Centerページの項目を作成する。
     Game Centerのラベル、Leaderboardボタン、Achievementsボタンを作成する。
     @param interface インターフェースレイヤー
     */
    void initGameCenterPage(AKInterface *interface);
    
    /*!
     @brief Storeページの項目作成
     
     Storeページの項目を作成する。
     Storeラベル、説明ラベル、購入ボタンを作成する。
     @param interface インターフェースレイヤー
     */
    void initStorePage(AKInterface *interface);
    
    /*!
     @brief Game CenterページでコントローラのAボタンを押した時の処理
     
     Game CenterページでコントローラのAボタンを押した時の処理を行う。
     @param cursorPosition カーソル位置
     */
    void onKeyDownAButtonAtGameCenterPage(int cursorPosition);
    
    /*!
     @brief StoreページでコントローラのAボタンを押した時の処理
     
     StoreページでコントローラのAボタンを押した時の処理を行う。
     @param cursorPosition カーソル位置
     */
    void onKeyDownAButtonAtStorePage(int cursorPosition);
    
    /*!
     @brief Game Centerページでのカーソル位置取得
     
     Game Centerページでのカーソル位置を取得する。
     @param positionID カーソル位置ID
     @return カーソル位置座標
     */
    cocos2d::Vec2 getCursorPositionAtGameCenterPage(int positionID);
    
    /*!
     @brief Storeページでのカーソル位置取得
     
     Storeページでのカーソル位置を取得する。
     @param positionID カーソル位置ID
     @return カーソル位置座標
     */
    cocos2d::Vec2 getCursorPositionAtStorePage(int positionID);

    /*!
     @brief Leaderboardボタン選択時の処理
     
     Leaderboardボタン選択時の処理。
     Leaderboardを表示する。
     */
    void selectLeaderboard();
    
    /*!
     @brief Achievementsボタン選択時の処理
     
     Achievementsボタン選択時の処理
     Achievementsを表示する。
     */
    void selectAchievements();
    
    /*!
     @brief Leaderboard表示
     
     Leaderboardを表示する。
     ボタンのブリンクアクション終了時に呼ばれるので、Leaderboard表示前にボタンのvisibleを表示に変更する。
     */
    void showLeaderboard();
    
    /*!
     @brief Achievements表示
     
     Achievementsを表示する。
     ボタンのブリンクアクション終了時に呼ばれるので、Achievements表示前にボタンのvisibleを表示に変更する。
     */
    void showAchievements();
    
    /*!
     @brief 購入ボタン選択時の処理
     
     購入ボタン選択時の処理を行う。
     ボタン選択時の効果音とエフェクトを発生させる。
     In App Purchaseの購入処理を行う。
     */
    void selectBuy();
    
    /*!
     @brief リストアボタン選択時の処理
     
     リストアボタン選択時の処理を行う。
     ボタン選択時の効果音とエフェクトを発生させる。
     In App Purchaseのリストア処理を行う。
     */
    void selectRestore();
    
    /*!
     @brief 通信開始
     
     通信開始時の処理を行う。
     通信中のビューを表示し、画面入力を無視するようにする。
     */
    void startConnect();
    
    /*!
     @brief 通信中レイヤー作成
     
     通信中に画面全体を暗くするためのレイヤーを作成し、シーンに配置する。
     */
    void createConnectingLayer();
    
    /*!
     @brief 通信中インジケータ作成
     
     通信中に回転するインジケータを作成し、シーンに配置する。
     */
    void createConnectingIndicator();
};

#endif
