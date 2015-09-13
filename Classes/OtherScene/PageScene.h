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
 @file PageScene.h
 @brief ページを持ったシーン
 
 ページを持ったシーンを表示するクラス。
 */

#ifndef __toritoma__PageScene__
#define __toritoma__PageScene__

#include "AKToritoma.h"
#include "base/CCController.h"

class PageScene : public cocos2d::Scene, AKMenuEventHandler {
public:
    
    /*!
     @brief デストラクタ
     
     未使用のスプライトフレームを解放する。
     */
    ~PageScene();
    
    /*!
     @brief 初期化処理
     
     初期化処理を行う。
     @return 正常終了時 true、異常終了時 false
     */
    virtual bool init();
    
    /*!
     @brief イベント処理
     
     イベントを処理する。
     @param item 選択されたメニュー項目
     */
    void execEvent(const AKMenuItem *item);
    
    /*!
     @brief コントローラー接続時処理
     
     コントローラーが接続された時の処理を行う。
     @param controller コントローラー
     @param event イベント
     */
     void onConnectedController(cocos2d::Controller* controller, cocos2d::Event* event);
    
    /*!
     @brief コントローラー切断時処理
     
     コントローラーが切断された時の処理を行う。
     @param controller コントローラー
     @param event イベント
     */
    void onDisconnectedController(cocos2d::Controller* controller, cocos2d::Event* event);
    
    /*!
     @brief コントローラーのボタンを押した時の処理
     
     コントローラーがボタンを押した時の処理を行う。
     @param controller コントローラー
     @param keyCode キーの種類
     @param event イベント
     */
    void onKeyDown(cocos2d::Controller* controller, int keyCode, cocos2d::Event* event);
    
    /*!
     @brief コントローラーのボタンを離した時の処理
     
     コントローラーがボタンを離した時の処理を行う。
     @param controller コントローラー
     @param keyCode キーの種類
     @param event イベント
     */
    void onKeyUp(cocos2d::Controller* controller, int keyCode, cocos2d::Event* event);
    
    /*!
     @brief コントローラーの方向キー入力処理
     
     コントローラーが方向キーを入力した時の処理を行う。
     @param controller コントローラー
     @param keyCode キーの種類
     @param event イベント
     */
    void onAxisEvent(cocos2d::Controller* controller, int keyCode, cocos2d::Event* event);
    
protected:
    
    /// イベント番号
    enum EventTouchNumber {
        EventTouchPrevButton = 0,   ///< 前ページボタン
        EventTouchNextButton,       ///< 次ページボタン
        EventTouchBackButton,       ///< 戻るボタン
        EventTouchCount             ///< イベントの個数
    };
    /// z座標
    enum ItemZPosition {
        ZPositionBack = 0,  ///< 背景のz座標
        ZPositionInterface, ///< インターフェースのz座標
        ZPositionItem       ///< 表示項目のz座標
    };
    
    /// 前ページボタンのタグ
    static const unsigned int MenuTagPrev;
    /// 次ページボタンのタグ
    static const unsigned int MenuTagNext;
    /// 戻るボタンのタグ
    static const unsigned int MenuTagBack;
    
    /*!
     @brief コンストラクタ
     
     メンバ変数の初期化を行う。
     @param maxPage ページ数
     */
    PageScene(int maxPage);
    
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
    
    /*!
     @brief インタフェース取得
     
     インタフェースを取得する。
     @return インタフェース
     */
    AKInterface* getInterface();
    
    /*!
     @brief カーソル画像の位置のマージン取得
     
     カーソル画像の位置のマージンを取得する。
     @return カーソル画像の位置のマージン
     */
    float getCursorPositionMargin();
    
    /*!
     @brief ページ数設定
     
     ページ数を設定する。
     @param maxPage ページ数
     */
    void setMaxPage(int maxPage);
    
private:
    
    /// ページ表示のフォーマット
    static const char *PageFormat;
    /// ページ表示位置、上からの位置
    static const float PagePositionTopPoint;
    /// 前ページボタンの位置、左からの位置
    static const float PrevPositionLeftPoint;
    /// 次ページボタンの位置、右からの位置
    static const float NextPositionRightPoint;
    /// 戻るボタンの位置、右からの位置
    static const float BackPositionRightPoint;
    /// 戻るボタンの位置、上からの位置
    static const float BackPositionTopPoint;
    /// 前ページボタンの画像ファイル名
    static const char *PrevImage;
    /// 次ページボタンの画像ファイル名
    static const char *NextImage;
    /// 戻るボタンの画像ファイル名
    static const char *BackImage;
    /// Lボタンの位置、左からの位置
    static const float LButtonPosLeftPoint;
    /// Lボタンの位置、中心からの縦方向の位置
    static const float LButtonPosVerticalCenterPoint;
    /// Rボタンの位置、右からの位置
    static const float RButtonPosRightPoint;
    /// Rボタンの位置、中心からの縦方向の位置
    static const float RButtonPosVerticalCenterPoint;
    /// Bボタンの位置、右からの位置
    static const float BButtonPosRightPoint;
    /// Bボタンの位置、上からの位置
    static const float BButtonPosTopPoint;
    /// カーソル画像の位置のボタンとの重なりの幅
    static const float CursorPosOverlap;
    
    /// ページ数
    int _maxPage;
    /// ページ番号
    int _pageNo;
    /// ページ番号のラベル
    AKLabel *_pageLabel;
    // インターフェースを作成する
    AKInterface *_interface;
    /// Bボタン画像
    cocos2d::Sprite *_bButton;
    /// Lボタン画像
    cocos2d::Sprite *_lButton;
    /// Rボタン画像
    cocos2d::Sprite *_rButton;
    /// Lスティックが上に入力されているか
    bool _isInputUp;
    /// Lスティックが下に入力されているか
    bool _isInputDown;
    /// Lスティックが左に入力されているか
    bool _isInputLeft;
    /// Lスティックが右に入力されているか
    bool _isInputRight;
    /// カーソル位置
    int _cursorPosition;
    /// カーソル画像
    cocos2d::Sprite *_cursorImage;
    
    /*!
     @brief デフォルトコンストラクタ
     
     デフォルトコンストラクタは使用禁止にする。
     */
    PageScene();
    
    /*!
     @brief 背景作成
     
     背景色レイヤーを作成し、シーンに追加する。
     */
    void createBackground();
    
    /*!
     @brief ページラベル作成
     
     ページラベルを作成し、シーンに追加する。
     */
    void createPageLabel();
    
    /*!
     @brief インターフェース作成
     
     インターフェースを作成し、シーンに追加する。
     */
    void createInterface();
    
    /*!
     @brief 前ページボタン作成
     
     前ページボタンを作成し、インターフェースに追加する。
     */
    void createPrevPageButton();
    
    /*!
     @brief 次ページボタン作成
     
     次ページボタンを作成し、インターフェースに追加する。
     */
    void createNextPageButton();
    
    /*!
     @brief 戻るボタン作成
     
     戻るボタンを作成し、インターフェースに追加する。
     */
    void createBackButton();
    
    /*!
     @brief ゲームコントローラ初期処理
     
     ゲームコントローラ使用の初期処理を行う。
     */
    void initController();
    
    /*!
     @brief Bボタン画像作成
     
     Bボタンを作成し、シーンに追加する。
     */
    void createBButtonImage();
    
    /*!
     @brief Lボタン画像作成
     
     Lボタンを作成し、シーンに追加する。
     */
    void createLButtonImage();
    
    /*!
     @brief Rボタン画像作成
     
     Rボタンを作成し、シーンに追加する。
     */
    void createRButtonImage();
    
    /*!
     @brief カーソル画像作成
     
     カーソル画像を作成し、シーンに追加する。
     */
    void createCursorImage();
    
    /*!
     @brief ページ番号設定
     
     ページ番号を設定する。
     @param pageNo ページ番号
     */
    void setPageNo(int pageNo);
    
    /*!
     @brief カーソル位置設定
     
     カーソル位置を設定する。
     @param cursorPosition カーソル位置
     */
    void setCursorPosition(int cursorPosition);
    
    /*!
     @brief 前ページ次ページボタン表示非表示更新
     
     前ページ、次ページボタンの表示非表示を現在のページ番号に応じて更新する。
     最初のページの場合は前ページボタンを無効にする。
     最後のページの場合は次ページボタンを無効にする。
     @return 有効にするインターフェースタグ
     */
    unsigned int updatePageButton();
    
    /*!
     @brief ページ番号表示更新
     
     ページ番号のラベルを更新する。
     */
    void updatePageLabel();
    
    /*!
     @brief コントローラ用画像表示更新
     
     コントローラ用画像の表示を更新する。
     */
    void updateControllerImage();

    /*!
     @brief 前ページ表示
     
     前ページを表示する。ページ番号を一つ減らす。
     */
    void goPrevPage();
    
    /*!
     @brief 次ページ表示
     
     次ページを表示する。ページ番号を一つ増やす。
     */
    void goNextPage();
    
    /*!
     @brief タイトルへ戻る
     
     タイトル画面シーンへ遷移する。
     */
    void backToTitle();
};

#endif /* defined(__toritoma__PageScene__) */
