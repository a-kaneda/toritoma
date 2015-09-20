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
 @file AKPlayingSceneIF.cpp
 @brief プレイシーンインターフェースの定義
 
 プレイシーンのインターフェースクラスを定義する。
 */

#include "AKPlayingSceneIF.h"

using cocos2d::Node;
using cocos2d::Vec2;
using cocos2d::SpriteFrameCache;
using cocos2d::Rect;
using cocos2d::Sprite;

/// プレイ中メニュー項目のタグ
const unsigned int kAKMenuTagPlaying = 0x01;
/// ポーズ中メニュー項目のタグ
const unsigned int kAKMenuTagPause = 0x02;
/// 終了メニュー項目のタグ
const unsigned int kAKMenuTagQuit = 0x04;
/// ゲームオーバー時メニュー項目のタグ
const unsigned int kAKMenuTagGameOver = 0x08;
/// ステージクリア時メニュー項目のタグ
const unsigned int kAKMenuTagStageClear = 0x10;
/// 全ステージクリア待機時メニュー項目のタグ
const unsigned int kAKMenuTagAllStageWaitClear = 0x20;
/// 全ステージクリア時メニュー項目のタグ
const unsigned int kAKMenuTagAllStageClear = 0x40;
/// カーソルのタグ
const unsigned int kAKMenuTagCursor = 0x80;

//======================================================================
// プレイ中のメニュー項目
//======================================================================
/// シールドボタン配置位置、右からの座標
static const float kAKShieldButtonPosFromRightPoint = 50.0f;
/// シールドボタン配置位置、下からの座標
static const float kAKShieldButtonPosFromBottomPoint = 50.0f;
/// シールドボタン非選択時の画像名
static const char *kAKShiledButtonNoSelectImage = "ShieldButton_01.png";
/// シールドボタン選択時の画像名
static const char *kAKShiledButtonSelectedImage = "ShieldButton_02.png";
/// ホールドボタン配置位置、右からの座標
//static const float kAKHoldButtonPosFromRightPoint = 50.0f;
/// ホールドボタン配置位置、下からの座標
//static const float kAKHoldButtonPosFromBottomPoint = 125.0f;
/// ホールドボタン非選択時の画像名
static const char *kAKHoldButtonNoSelectImage = "HoldButton_01.png";
/// ホールドボタン選択時の画像名
static const char *kAKHoldButtonSelectedImage = "HoldButton_02.png";
/// ポーズボタンの配置位置、右からの位置
static const float kAKPauseButtonPosRightPoint = 22.0f;
/// ポーズボタンの配置位置、上からの位置
static const float kAKPauseButtonPosTopPoint = 22.0f;
/// ポーズボタンの画像ファイル名
static const char *kAKPauseButtonImage = "PauseButton.png";

//======================================================================
// ポーズ時のメニュー項目
//======================================================================
/// 一時停止中の表示文字列
static const char *kAKPauseString = "  PAUSE  ";
/// 一時停止解除のボタンの文字列
static const char *kAKResumeString = "RESUME";
/// 終了ボタンの文字列
static const char *kAKQuitButtonString = " QUIT ";
/// 一時停止メッセージの表示位置、下からの比率
static const float kAKPauseMessagePosBottomRatio = 0.6f;
/// 一時停止メニュー項目の表示位置、下からの比率
static const float kAKPauseMenuPosBottomRatio = 0.4f;
/// レジュームボタンの表示位置、左からの比率
static const float kAKResumeButtonPosLeftRatio = 0.3f;
/// 終了ボタンの表示位置、右からの比率
static const float kAKQuitButtonPosRightRatio = 0.3f;
/// カーソル画像の位置のボタンとの重なりの幅
static const float CursorPosOverlap = 8.0f;

//======================================================================
// 終了メニューのメニュー項目
//======================================================================
/// 終了確認メッセージ
static const char *kAKQuitMessageString = " QUIT GAME? ";
/// YESボタンの文字列
static const char *kAKQuitYesString = " YES ";
/// NOボタンの文字列
static const char *kAKQuitNoString = " N O ";
/// 終了メニューキャプションの表示位置、下からの比率
static const float kAKQuitMessagePosBottomRatio = 0.6f;
/// 終了メニューボタンの表示位置、下からの比率
static const float kAKQuitButtonPosBottomRatio = 0.4f;
/// 終了メニューYESボタンの表示位置、左からの比率
static const float kAKQuitYesButtonPosLeftRatio = 0.3f;
/// 終了メニューNOボタンの表示位置、右からの比率
static const float kAKQuitNoButtonPosRightRatio = 0.3f;

//======================================================================
// ゲームオーバー時のメニュー項目
//======================================================================
/// ゲームオーバー時の表示文字列
static const char *kAKGameOverString = "GAME OVER";
/// タイトルへ戻るボタンのキャプション
static const char *kAKGameOverQuitButtonCaption = "QUIT";
/// Twitterボタンの画像ファイル名
static const char *kAKTwitterButtonImageFile = "TwitterButton.png";
/// ゲームオーバーキャプションの表示位置、下からの比率
static const float kAKGameOverCaptionPosBottomRatio = 0.6f;
/// タイトルへ戻るボタンの位置、下からの比率
static const float kAKGameOverQuitButtonPosBottomRatio = 0.4f;
/// Twitterボタンの配置位置、中心からの横方向の位置
static const float kAKTwitterButtonPosHorizontalCenterPoint = 120.0f;
/// Twitterボタンの配置位置、下からの位置
static const float kAKTwitterButtonPosBottomRatio = 0.6f;

//======================================================================
// ステージクリア時のメニュー項目
//======================================================================
/// ステージクリア時のメッセージ
static const char *kAKStageClearString = "STAGE CLEAR";

//======================================================================
// 全ステージクリア時のメニュー項目
//======================================================================
/// 全ステージクリア時のメッセージ
static const char *kAKAllStageClearString = "ALL STAGE CLEAR";
/// 2周目続行ボタンのキャプション
static const char *kAKContinuePlayingButtonString = "CONTINUE";
/// ステージクリアキャプションの表示位置、下からの比率
static const float kAKAllStageClearCaptionPosBottomRatio = 0.6f;
/// 2周目続行ボタンの位置、下からの比率
static const float kAKContinuePlayingButtonPosBottomRatio = 0.4f;

/*!
 @brief コンビニエンスコンストラクタ
 
 インスタンスを生成し、初期化処理を行い、autoreleaseを行う。
 @param eventHandler イベントを処理するクラス
 @param capacity メニュー項目数
 @return 生成したインスタンス
 */
AKPlayingSceneIF* AKPlayingSceneIF::createInterface(AKMenuEventHandler * const eventHandler)
{
    AKPlayingSceneIF *instance = new AKPlayingSceneIF(eventHandler);
    if (instance->init()) {
        instance->autorelease();
        return instance;
    }
    else {
        CC_SAFE_DELETE(instance);
        return NULL;
    }
}

/*!
 @brief デストラクタ
 
 メンバの解放を行う。
 */
AKPlayingSceneIF::~AKPlayingSceneIF()
{
    AKLog(kAKLogPlayingSceneIF_1, "start");
    
    // メンバを解放する
    m_shieldButton->removeFromParentAndCleanup(true);
    m_shieldButton->release();
//    m_holdButton->removeFromParentAndCleanup(true);
//    m_holdButton->release();
    m_resumeButton->release();
    m_quitButton->release();
    m_quitNoButton->release();
    
    AKLog(kAKLogPlayingSceneIF_1, "end");
}

/*!
 @brief ポーズ解除ボタン取得
 
 ポーズ解除ボタンを取得する。
 @return ポーズ解除ボタン
 */
AKLabel* AKPlayingSceneIF::getResumeButton()
{
    return m_resumeButton;
}

/*!
 @brief 終了ボタン取得
 
 終了ボタンを取得する。
 @return 終了ボタン
 */
AKLabel* AKPlayingSceneIF::getQuitButton()
{
    return m_quitButton;
}

/*!
 @brief 終了メニューNoボタン取得
 
 終了メニューのNoボタンを取得する。
 @return 終了メニューNoボタン
 */
AKLabel* AKPlayingSceneIF::getQuitNoButton()
{
    return m_quitNoButton;
}

/*!
 @brief 2周目続行ボタン取得
 
 ゲームクリアメニューの2周目続行ボタンを取得する。
 @return 2周目続行ボタン
 */
AKLabel* AKPlayingSceneIF::getContinuePlayingButton()
{
    return m_continuePlayingButton;
}

/*!
 @brief コンストラクタ
 
 各メニュー項目の作成を行う。
 @param eventHandler イベントを処理するクラス
 @param capacity メニュー項目数
 */
AKPlayingSceneIF::AKPlayingSceneIF(AKMenuEventHandler * const eventHandler) :
AKInterface(eventHandler),
m_visibleCursor(false)
{
    // プレイ中のメニュー項目を作成する
    createPlayingMenu();
    
    // ポーズ時のメニュー項目を作成する
    createPauseMenu();
    
    // 終了メニューを作成する
    createQuitMenu();
    
    // ゲームオーバー時のメニュー項目を作成する
    createGameOverMenu();

    // ステージクリア時のメニュー項目を作成する
    createStageClear();
    
    // 全ステージクリア時のメニュー項目を作成する
    createAllStageClear();
}

/*!
 @brief プレイ中のメニュー項目作成
 
 プレイ中のメニュー項目を作成する。
 */
void AKPlayingSceneIF::createPlayingMenu()
{
    AKLog(true, "start createPlayingMenu()");
    
    // シールドボタンを作成する
    float x = AKScreenSize::positionFromRightPoint(kAKShieldButtonPosFromRightPoint);
    float y = AKScreenSize::positionFromBottomPoint(kAKShieldButtonPosFromBottomPoint);
    m_shieldButton = addSpriteMenu(kAKShiledButtonNoSelectImage,
                                   Vec2(x, y),
                                   0,
                                   kAKEventTouchShieldButton,
                                   kAKMenuTagPlaying | kAKMenuTagStageClear,
                                   kAKMenuTypeMomentary);
    m_shieldButton->retain();
    
    // ホールドボタンを作成する
//    x = AKScreenSize::positionFromRightPoint(kAKHoldButtonPosFromRightPoint);
//    y = AKScreenSize::positionFromBottomPoint(kAKHoldButtonPosFromBottomPoint);
//    m_holdButton = addSpriteMenu(kAKHoldButtonNoSelectImage,
//                                   Vec2(x, y),
//                                   0,
//                                   kAKEventTouchHoldButton,
//                                   kAKMenuTagPlaying | kAKMenuTagStageClear,
//                                   kAKMenuTypeButton);
//    m_holdButton->retain();
    
    // ポーズボタンを追加する
    x = AKScreenSize::positionFromRightPoint(kAKPauseButtonPosRightPoint);
    y = AKScreenSize::positionFromTopPoint(kAKPauseButtonPosTopPoint);
    addSpriteMenu(kAKPauseButtonImage,
                  Vec2(x, y),
                  0,
                  kAKEventTouchPauseButton,
                  kAKMenuTagPlaying | kAKMenuTagStageClear,
                  kAKMenuTypeButton);
    
    // スライド入力を画面全体に配置する
    Rect rect(0.0f,
              0.0f,
              AKScreenSize::screenSize().width,
              AKScreenSize::screenSize().height);
    addSlideMenu(rect,kAKEventSlide, kAKMenuTagPlaying | kAKMenuTagStageClear | kAKMenuTagAllStageWaitClear);

    AKLog(true, "end createPlayingMenu()");
}

/*!
 @brief ポーズ時のメニュー項目作成
 
 ポーズ時のメニュー項目を作成する。
 */
void AKPlayingSceneIF::createPauseMenu()
{
    // ポーズラベルを生成する
    AKLabel *label = AKLabel::createLabel(kAKPauseString,
                                          (int)strlen(kAKPauseString),
                                          1,
                                          kAKLabelFrameMessage,
                                          AKLabel::ControlFont);
    
    // ポーズラベルの位置を設定する
    float x = AKScreenSize::center().x;
    float y = AKScreenSize::positionFromBottomRatio(kAKPauseMessagePosBottomRatio);
    label->setPosition(Vec2(x, y));
    
    // ポーズラベルをレイヤーに配置する
    addChild(label, 0, kAKMenuTagPause);
    
    // ポーズ解除ボタンを作成する
    x = AKScreenSize::positionFromLeftRatio(kAKResumeButtonPosLeftRatio);
    y = AKScreenSize::positionFromBottomRatio(kAKPauseMenuPosBottomRatio);
    m_resumeButton = addLabelMenu(kAKResumeString,
                                  Vec2(x, y),
                                  0,
                                  kAKEventTouchResumeButton,
                                  kAKMenuTagPause,
                                  true);
    m_resumeButton->retain();
    
    // 終了ボタンを作成する
    x = AKScreenSize::positionFromRightRatio(kAKQuitButtonPosRightRatio);
    y = AKScreenSize::positionFromBottomRatio(kAKPauseMenuPosBottomRatio);
    m_quitButton = addLabelMenu(kAKQuitButtonString,
                                Vec2(x, y),
                                0,
                                kAKEventTouchQuitButton,
                                kAKMenuTagPause,
                                true);
    m_quitButton->retain();
    
    // ポーズ時のカーソルを作成する
    m_pauseCursor = Sprite::createWithSpriteFrameName(CursorImageFileName);

    // ポーズ解除ボタンの左側に配置する
    x = m_resumeButton->getPosition().x - m_resumeButton->getWidth() / 2 - m_pauseCursor->getContentSize().width / 2 + CursorPosOverlap;
    y = m_resumeButton->getPosition().y;
    m_pauseCursor->setPosition(x, y);

    // レイヤーに配置する
    addChild(m_pauseCursor, 1, kAKMenuTagPause | kAKMenuTagCursor);
}

/*!
 @brief 終了メニュー項目作成
 
 終了メニューを作成する。
 */
void AKPlayingSceneIF::createQuitMenu()
{
    // 終了確認メッセージラベルを生成する
    AKLabel *label = AKLabel::createLabel(kAKQuitMessageString,
                                          (int)strlen(kAKQuitMessageString),
                                          1,
                                          kAKLabelFrameMessage,
                                          AKLabel::ControlFont);
    
    // 終了確認メッセージラベルの位置を設定する
    float x = AKScreenSize::center().x;
    float y = AKScreenSize::positionFromBottomRatio(kAKQuitMessagePosBottomRatio);
    label->setPosition(Vec2(x, y));
    
    // 終了確認メッセージラベルをレイヤーに配置する
    addChild(label, 0, kAKMenuTagQuit);
    
    // YESボタンを作成する
    x = AKScreenSize::positionFromLeftRatio(kAKQuitYesButtonPosLeftRatio);
    y = AKScreenSize::positionFromBottomRatio(kAKQuitButtonPosBottomRatio);
    m_quitYesButton = addLabelMenu(kAKQuitYesString,
                                   Vec2(x, y),
                                   0,
                                   kAKEventTouchQuitYesButton,
                                   kAKMenuTagQuit,
                                   true);
    
    // NOボタンを作成する
    x = AKScreenSize::positionFromRightRatio(kAKQuitNoButtonPosRightRatio);
    y = AKScreenSize::positionFromBottomRatio(kAKQuitButtonPosBottomRatio);
    m_quitNoButton = addLabelMenu(kAKQuitNoString,
                                  Vec2(x, y),
                                  0,
                                  kAKEventTouchQuitNoButton,
                                  kAKMenuTagQuit,
                                  true);
    m_quitButton->retain();

    // 終了メニューのカーソルを作成する
    m_quitCursor = Sprite::createWithSpriteFrameName(CursorImageFileName);
    
    // NOボタンの左側に配置する
    x = m_quitNoButton->getPosition().x - m_quitNoButton->getWidth() / 2 - m_quitCursor->getContentSize().width / 2 + CursorPosOverlap;
    y = m_quitNoButton->getPosition().y;
    m_quitCursor->setPosition(x, y);
    
    // レイヤーに配置する
    addChild(m_quitCursor, 1, kAKMenuTagQuit | kAKMenuTagCursor);
}

/*!
 @brief ゲームオーバー時のメニュー項目作成
 
 ゲームオーバー時のメニュー項目を作成する。
 */
void AKPlayingSceneIF::createGameOverMenu()
{
    // ゲームオーバーラベルを生成する
    AKLabel *label = AKLabel::createLabel(kAKGameOverString,
                                          (int)strlen(kAKGameOverString),
                                          1,
                                          kAKLabelFrameMessage,
                                          AKLabel::ControlFont);
    
    // ゲームオーバーラベルの位置を設定する
    float x = AKScreenSize::center().x;
    float y = AKScreenSize::positionFromBottomRatio(kAKGameOverCaptionPosBottomRatio);
    label->setPosition(Vec2(x, y));
    
    // ゲームオーバーラベルをレイヤーに配置する
    addChild(label, 0, kAKMenuTagGameOver);
    
    // タイトルへ戻るボタンを作成する
    x = AKScreenSize::center().x;
    y = AKScreenSize::positionFromBottomRatio(kAKGameOverQuitButtonPosBottomRatio);
    AKLabel *backToTitleButton = addLabelMenu(kAKGameOverQuitButtonCaption,
                                              Vec2(x, y),
                                              0,
                                              kAKEventTouchQuitYesButton,
                                              kAKMenuTagGameOver,
                                              true);
    
    // Twitterボタンを作成する
    x = AKScreenSize::positionFromHorizontalCenterPoint(kAKTwitterButtonPosHorizontalCenterPoint);
    y = AKScreenSize::positionFromBottomRatio(kAKTwitterButtonPosBottomRatio);
    addSpriteMenu(kAKTwitterButtonImageFile,
                  Vec2(x, y),
                  0,
                  kAKEventTouchTwitterButton,
                  kAKMenuTagGameOver,
                  kAKMenuTypeButton);

    // ゲームオーバー時のカーソルを作成する
    m_gameOverCursor = Sprite::createWithSpriteFrameName(CursorImageFileName);
    
    // タイトルへ戻るボタンの左側に配置する
    x = backToTitleButton->getPosition().x - backToTitleButton->getWidth() / 2 - m_gameOverCursor->getContentSize().width / 2 + CursorPosOverlap;
    y = backToTitleButton->getPosition().y;
    m_gameOverCursor->setPosition(x, y);
    
    // レイヤーに配置する
    addChild(m_gameOverCursor, 1, kAKMenuTagGameOver | kAKMenuTagCursor);
}

/*!
 @brief ステージクリア時のメニュー項目作成
 
 ステージクリア時のメニュー項目を作成する。
 */
void AKPlayingSceneIF::createStageClear()
{
    // ステージクリアラベルを生成する
    AKLabel *label = AKLabel::createLabel(kAKStageClearString,
                                          (int)strlen(kAKStageClearString),
                                          1,
                                          kAKLabelFrameNone,
                                          AKLabel::ControlFont);
    
    // ステージクリアラベルの位置を設定する
    float x = AKScreenSize::center().x;
    float y = AKScreenSize::center().y;
    label->setPosition(Vec2(x, y));

    // ステージクリアラベルをレイヤーに配置する
    addChild(label, 0, kAKMenuTagStageClear);
}

/*!
 @brief 全ステージクリア時のメニュー項目作成
 
 全ステージクリア時のメニュー項目を作成する。
 */
void AKPlayingSceneIF::createAllStageClear()
{
    // ステージクリアラベルを生成する
    AKLabel *label = AKLabel::createLabel(kAKAllStageClearString,
                                          (int)strlen(kAKAllStageClearString),
                                          1,
                                          kAKLabelFrameNone,
                                          AKLabel::ControlFont);
    
    // ステージクリアラベルの位置を設定する
    float x = AKScreenSize::center().x;
    float y = AKScreenSize::positionFromBottomRatio(kAKAllStageClearCaptionPosBottomRatio);
    label->setPosition(Vec2(x, y));
    
    // ステージクリアラベルをレイヤーに配置する
    addChild(label, 0, kAKMenuTagAllStageWaitClear | kAKMenuTagAllStageClear);
    
    // 2周目続行ボタンを作成する
    x = AKScreenSize::center().x;
    y = AKScreenSize::positionFromBottomRatio(kAKContinuePlayingButtonPosBottomRatio);
    m_continuePlayingButton = addLabelMenu(kAKContinuePlayingButtonString,
                                                  Vec2(x, y),
                                                  0,
                                                  kAKEventTouchContinuePlayingButton,
                                                  kAKMenuTagAllStageClear,
                                                  true);

    // ゲームクリアメニューのカーソルを作成する
    m_clearCursor = Sprite::createWithSpriteFrameName(CursorImageFileName);
    
    // 2周目続行ボタンの左側に配置する
    x = m_continuePlayingButton->getPosition().x - m_continuePlayingButton->getWidth() / 2 - m_clearCursor->getContentSize().width / 2 + CursorPosOverlap;
    y = m_continuePlayingButton->getPosition().y;
    m_clearCursor->setPosition(x, y);
    
    // レイヤーに配置する
    addChild(m_clearCursor, 1, kAKMenuTagAllStageClear | kAKMenuTagCursor);
}

/*!
 @brief シールドボタン表示切替
 
 シールドボタンの表示を選択・非選択状態で切り替えを行う。
 @param seleted 選択状態かどうか
 */
void AKPlayingSceneIF::setShieldButtonSelected(bool selected)
{
    // スプライトフレームキャッシュを取得する
    SpriteFrameCache *spriteFrameCache = SpriteFrameCache::getInstance();

    // 選択中かどうかで画像を切り替える
    if (selected) {
        m_shieldButton->setDisplayFrame(spriteFrameCache->getSpriteFrameByName(kAKShiledButtonSelectedImage));
    }
    else {
        m_shieldButton->setDisplayFrame(spriteFrameCache->getSpriteFrameByName(kAKShiledButtonNoSelectImage));
    }
}

/*!
 @brief ホールドボタン表示切替
 
 ホールドボタンの表示を選択・非選択状態で切り替えを行う。
 @param seleted 選択状態かどうか
 */
void AKPlayingSceneIF::setHoldButtonSelected(bool selected)
{
    // スプライトフレームキャッシュを取得する
    SpriteFrameCache *spriteFrameCache = SpriteFrameCache::getInstance();

    // 選択中かどうかで画像を切り替える
    if (selected) {
        m_holdButton->setDisplayFrame(spriteFrameCache->getSpriteFrameByName(kAKHoldButtonSelectedImage));
    }
    else {
        m_holdButton->setDisplayFrame(spriteFrameCache->getSpriteFrameByName(kAKHoldButtonNoSelectImage));
    }
}

/*!
 @brief カーソル表示設定
 
 カーソルの表示・非表示を設定する。
 @param isVisible 表示するかどうか
 */
void AKPlayingSceneIF::setVisibleCursor(bool isVisible)
{
    // メンバに設定する
    m_visibleCursor = isVisible;
    
    // 項目の表示非表示を更新する
    AKInterface::updateVisible();
}

/*!
 @brief カーソル位置取得
 
 カーソル位置を取得する。
 @return カーソル位置
 */
int AKPlayingSceneIF::getCursorPosition()
{
    return m_cursorPosition;
}

/*!
 @brief カーソル位置設定
 
 カーソル位置を設定する。
 @param pos カーソル位置
 */
void AKPlayingSceneIF::setCursorPosition(int pos)
{
    // 表示状態に応じて処理を分ける
    switch (getEnableTag()) {
        case kAKMenuTagPause:           // ポーズメニュー
            
            // 0以下は0とする
            if (pos <= 0) {
                
                // カーソル位置は0とする
                m_cursorPosition = 0;
                
                // ポーズ解除ボタンの左側に配置する
                float x = m_resumeButton->getPosition().x - m_resumeButton->getWidth() / 2 - m_pauseCursor->getContentSize().width / 2 + CursorPosOverlap;
                float y = m_resumeButton->getPosition().y;
                m_pauseCursor->setPosition(x, y);
            }
            // 1以上は1とする
            else {
                
                // カーソル位置は1とする
                m_cursorPosition = 1;
                
                // 終了ボタンの左側に配置する
                float x = m_quitButton->getPosition().x - m_quitButton->getWidth() / 2 - m_pauseCursor->getContentSize().width / 2 + CursorPosOverlap;
                float y = m_quitButton->getPosition().y;
                m_pauseCursor->setPosition(x, y);
            }
            
            break;
            
        case kAKMenuTagQuit:            // ゲーム終了メニュー

            // 0以下は0とする
            if (pos <= 0) {
                
                // カーソル位置は0とする
                m_cursorPosition = 0;
                
                // YESボタンの左側に配置する
                float x = m_quitYesButton->getPosition().x - m_quitYesButton->getWidth() / 2 - m_quitCursor->getContentSize().width / 2 + CursorPosOverlap;
                float y = m_quitYesButton->getPosition().y;
                m_quitCursor->setPosition(x, y);
            }
            // 1以上は1とする
            else {
                
                // カーソル位置は1とする
                m_cursorPosition = 1;
                
                // NOボタンの左側に配置する
                float x = m_quitNoButton->getPosition().x - m_quitNoButton->getWidth() / 2 - m_quitCursor->getContentSize().width / 2 + CursorPosOverlap;
                float y = m_quitNoButton->getPosition().y;
                m_quitCursor->setPosition(x, y);
            }
            
            break;
            
        default:
            
            // その他の場合はカーソル位置は0固定とする
            m_cursorPosition = 0;
            
            break;
    }
}

/*!
 @brief メニュー項目個別表示設定
 
 メニュー項目の表示非表示を有効タグとは別に設定したい場合に個別に設定を行う。
 カーソルの表示非表示を切り替える。
 @param item 設定するメニュー項目
 */
void AKPlayingSceneIF::updateVisible(Node *item)
{
    // カーソルの場合、カーソル非表示となっている場合は表示しない
    if ((item->getTag() & kAKMenuTagCursor) && !m_visibleCursor) {
        item->setVisible(false);
    }
    // その他の場合はそのままとする
    else {
        // No operation.
    }
}
