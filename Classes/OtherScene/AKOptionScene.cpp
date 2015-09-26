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
 @file AKOptionScene.cpp
 @brief オプション画面シーンクラスの定義
 
 オプション画面シーンクラスを定義する。
 */

#include "AKOptionScene.h"
#include "AKTitleScene.h"
#include "Payment.h"
#include "OnlineScore.h"
#include "SettingFileIO.h"
#include "ID.h"
#include "Advertisement.h"
#include "AKStringSplitter.h"

using std::string;
using cocos2d::Blink;
using cocos2d::CallFunc;
using cocos2d::Color4B;
using cocos2d::Director;
using cocos2d::LayerColor;
using cocos2d::Sequence;
using cocos2d::Size;
using cocos2d::SpriteFrameCache;
using cocos2d::TransitionFade;
using cocos2d::Vec2;
using CocosDenshion::SimpleAudioEngine;
using aklib::Payment;
using aklib::LocalizedResource;
using aklib::Advertisement;

// オプション画面シーンに配置するノードのタグ
enum {
    kAKOptionSceneBackColor = 0,    ///< 背景色のタグ
    kAKOptionSceneInterface         ///< インターフェースレイヤーのタグ
};

/// Game Cetnerのページ
static const int kAKPageGameCenter = 1;
/// Storeのページ
static const int kAKPageStore = 2;
/// Game Centerページの項目のタグ
static const unsigned int kAKMenuGameCenter = 0x04;
/// Storeページ(共通)の項目のタグ
static const unsigned int kAKMenuStore = 0x08;
/// Storeページ(購入前)の項目のタグ
static const unsigned int kAKMenuStoreBeforePurchase = 0x10;
/// Storeページ(購入後)の項目のタグ
static const unsigned int kAKMenuStoreAfterPurcase = 0x20;
/// Option画面ページ数
static const int kAKMenuPageCount = 2;

/// Game Centerのキャプション
static const char *kAKGameCenterCaption = " GAME CENTER ";
/// Leaderboardボタンのキャプション
static const char *kAKLeaderboardCaption = "LEADERBOARD";
/// Game Centerのキャプション位置、上からの比率
static const float kAKGameCenterCaptionPosTopRatio = 0.2f;
/// Leaderboardボタンの位置、上からの比率
static const float kAKLeaderboardPosTopRatio = 0.4f;

/// Storeのキャプション
static const char *kAKStoreCaption = "STORE";
/// 購入ボタンのキャプション
static const char *kAKBuyButtonCaption = "  BUY  ";
/// リストアボタンのキャプション
static const char *kAKRestoreButtonCaption = "RESTORE";
/// Storeメッセージのキー
static const char *kAKStoreMessageKey = "StoreMessage";
/// 購入済みのキャプションのキー
static const char *kAKStorePurchasedKey = "StorePurchased";
/// Storeのキャプション位置、上からの比率
static const float kAKStoreCaptionPosTopRatio = 0.2f;
/// ボタンの位置、上からの比率
static const float kAKButtonPosTopRatio = 0.4f;
/// 購入ボタンの位置、左からの比率
static const float kAKBuyButtonPosLeftRatio = 0.32f;
/// リストアボタンの位置、左からの比率
static const float kAKRestoreButtonPosLeftRatio = 0.68f;
/// メッセージボックスの1行の文字数
static const int kAKMsgLength = 20;
/// メッセージボックスの行数
static const int kAKMsgLineCount = 4;
/// メッセージボックスの位置、下からの位置
static const float kAKMsgPosBottomPoint = 100.0f;
/// 購入済みのキャプションの位置、上からの比率
static const float kAKPurchasedCaptionPosTopRatio = 0.4f;

// コンビニエンスコンストラクタ
AKOptionScene* AKOptionScene::create()
{
    AKOptionScene *instance = new AKOptionScene();
    if (instance->init()) {
        instance->autorelease();
        return instance;
    }
    else {
        CC_SAFE_DELETE(instance);
        return NULL;
    }
}

// コンストラクタ
AKOptionScene::AKOptionScene() : PageScene(kAKMenuPageCount)
{
}

// 課金完了
void AKOptionScene::completePayment()
{
    // 課金済みの場合は広告を無効化する
    if (SettingFileIO::GetInstance().IsPurchased()) {
        Advertisement::disableAds();
    }

    // 通信中レイヤーを削除する
    removeChild(m_connectingLayer);
    m_connectingLayer = NULL;
    
    // 通信中インジケータを削除する
    removeChild(m_indicator);
    m_indicator = NULL;
    
    // 購入ボタン、リストアボタンの表示・非表示切替のため、表示更新を行う
    updateView();
    
    // 画面入力を有効化する
    enableOperation();
}

// 派生クラスの初期化処理
bool AKOptionScene::initSub()
{
    // アプリ課金が禁止されている場合はページをひとつ減らす
    if (!Payment::CanMakePayments()) {
        setMaxPage(kAKMenuPageCount - 1);
    }
    
    // Game Centerページの項目を作成する
    initGameCenterPage(getInterface());
    
    // Storeページの項目を作成する
    initStorePage(getInterface());
    
    return true;
}

// 派生クラスのイベント処理
void AKOptionScene::execSubEvent(int pageNo, const AKMenuItem *item)
{
    // 選択された項目に応じて処理を行う
    switch (item->getEventNo()) {
        case EventTouchLeaderboard:
            selectLeaderboard();
            break;
            
        case EventTouchBuy:
            selectBuy();
            break;
            
        case EventTouchRestore:
            selectRestore();
            break;
            
        default:
            AKAssert(false, "event number is wrong :%d", item->getEventNo());
            break;
    }
}

// コントローラのAボタンを押した時の処理
void AKOptionScene::onKeyDownAButton(int pageNo, int cursorPosition)
{
    // ページごとに処理を分岐する
    switch (pageNo) {
        case kAKPageGameCenter:
            onKeyDownAButtonAtGameCenterPage(cursorPosition);
            break;
            
        case kAKPageStore:
            onKeyDownAButtonAtStorePage(cursorPosition);
            break;
            
        default:
            AKAssert(false, "pageNo is wrong : %d", pageNo);
            break;
    }
}

// コントローラのLスティックを上に倒した時の処理
int AKOptionScene::onLStickUp(int pageNo, int cursorPosition)
{
    // Game Centerページ以外は処理しない
    if (pageNo != kAKPageGameCenter) {
        return cursorPosition;
    }
    
    // ボタンは1つしかないので、常にLeaderboardの位置を返す
    return CursorPositionLeaderboard;
}

// コントローラのLスティックを下に倒した時の処理
int AKOptionScene::onLStickDown(int pageNo, int cursorPosition)
{
    // Game Centerページ以外は処理しない
    if (pageNo != kAKPageGameCenter) {
        return cursorPosition;
    }
    
    // ボタンは1つしかないので、常にLeaderboardの位置を返す
    return CursorPositionLeaderboard;
}

// コントローラのLスティックを左に倒した時の処理
int AKOptionScene::onLStickLeft(int pageNo, int cursorPosition)
{
    // Storeページ以外は処理しない
    if (pageNo != kAKPageStore) {
        return cursorPosition;
    }
    
    // 購入済みの場合は処理しない
    SettingFileIO &setting = SettingFileIO::GetInstance();
    if (setting.IsPurchased()) {
        return cursorPosition;
    }
    
    // カーソル移動時の効果音を鳴らす
    SimpleAudioEngine::getInstance()->playEffect(kAKCursorSEFileName);
    
    // ボタンは2つしかないので、逆側のボタンを選択する
    if (cursorPosition == CursorPositionBuy) {
        return CursorPositionRestore;
    }
    else {
        return CursorPositionBuy;
    }
}

// コントローラのLスティックを右に倒した時の処理
int AKOptionScene::onLStickRight(int pageNo, int cursorPosition)
{
    // Storeページ以外は処理しない
    if (pageNo != kAKPageStore) {
        return cursorPosition;
    }
    
    // 購入済みの場合は処理しない
    SettingFileIO &setting = SettingFileIO::GetInstance();
    if (setting.IsPurchased()) {
        return cursorPosition;
    }
    
    // カーソル移動時の効果音を鳴らす
    SimpleAudioEngine::getInstance()->playEffect(kAKCursorSEFileName);
    
    // ボタンは2つしかないので、逆側のボタンを選択する
    if (cursorPosition == CursorPositionBuy) {
        return CursorPositionRestore;
    }
    else {
        return CursorPositionBuy;
    }
}

// ページ表示内容更新
unsigned int AKOptionScene::updatePageContents(int pageNo)
{
    // ページによってインターフェースの有効タグを判定する
    switch (pageNo) {
        case kAKPageGameCenter:     // Game Centerのページ
            return kAKMenuGameCenter;
            
        case kAKPageStore:          // Storeのページ
            
        {
            // 購入済みかどうかで有効化する項目を変える
            SettingFileIO &setting = SettingFileIO::GetInstance();
            if (setting.IsPurchased()) {
                return kAKMenuStore | kAKMenuStoreAfterPurcase;
            }
            else {
                return kAKMenuStore | kAKMenuStoreBeforePurchase;
            }
        }
            
        default:                    // その他のページは存在しない
            AKAssert(false, "pageNo is wrong : %d", pageNo);
            return kAKMenuGameCenter;
    }
}

// カーソル表示有無取得
bool AKOptionScene::isVisibleCursor(int pageNo)
{
    // ページによって処理を分岐する
    switch (pageNo) {
        case kAKPageGameCenter:
            // Game Centerページは常にカーソルを表示する
            return true;
            
        case kAKPageStore:
        {
            // 購入済みの場合はカーソルを非表示にし、そうでない場合は表示する
            SettingFileIO &setting = SettingFileIO::GetInstance();
            if (setting.IsPurchased()) {
                return false;
            }
            else {
                return true;
            }
        }
            
        default:
            AKAssert(false, "pageNo is wrong : %d", pageNo);
            return false;
    }
}

// カーソル位置取得
Vec2 AKOptionScene::getCursorPosition(int pageNo, int positionID)
{
    // ページによって処理を分岐する
    switch (pageNo) {
        case kAKPageGameCenter:
            return getCursorPositionAtGameCenterPage(positionID);
            
        case kAKPageStore:
            return getCursorPositionAtStorePage(positionID);
            
        default:
            AKAssert(false, "pageNo is wrong : %d", pageNo);
            return Vec2(0, 0);
    }
}

// Game Centerページの項目作成
void AKOptionScene::initGameCenterPage(AKInterface *interface)
{
    // Game Centerのラベルを作成する
    AKLabel *gameCenterLabel = AKLabel::createLabel(kAKGameCenterCaption,
                                                    (int)strlen(kAKGameCenterCaption),
                                                    1,
                                                    kAKLabelFrameNone,
                                                    AKLabel::ControlFont);
    
    // Game Centerのラベルの位置を設定する
    float x = AKScreenSize::center().x;
    float y = AKScreenSize::positionFromTopRatio(kAKGameCenterCaptionPosTopRatio);
    gameCenterLabel->setPosition(Vec2(x, y));
    
    // Game Centerのラベルを配置する
    interface->addChild(gameCenterLabel, 0, kAKMenuGameCenter);
    
    // Leaderboardのメニューを作成する
    x = AKScreenSize::center().x;
    y = AKScreenSize::positionFromTopRatio(kAKLeaderboardPosTopRatio);
    m_leaderboardButton = interface->addLabelMenu(kAKLeaderboardCaption,
                                                  Vec2(x, y),
                                                  0,
                                                  EventTouchLeaderboard,
                                                  kAKMenuGameCenter,
                                                  true);
}

// Storeページの項目作成
void AKOptionScene::initStorePage(AKInterface *interface)
{
    // Storeのラベルを作成する
    AKLabel *storeLabel = AKLabel::createLabel(kAKStoreCaption,
                                               (int)strlen(kAKStoreCaption),
                                               1,
                                               kAKLabelFrameNone,
                                               AKLabel::ControlFont);
    
    // Storeのラベルの位置を設定する
    float x = AKScreenSize::center().x;
    float y = AKScreenSize::positionFromTopRatio(kAKStoreCaptionPosTopRatio);
    storeLabel->setPosition(Vec2(x, y));
    
    // Storeのラベルを配置する
    interface->addChild(storeLabel, 0, kAKMenuStore);
    
    // 購入ボタンを作成する
    x = AKScreenSize::positionFromLeftRatio(kAKBuyButtonPosLeftRatio);
    y = AKScreenSize::positionFromTopRatio(kAKButtonPosTopRatio);
    m_buyButton = interface->addLabelMenu(kAKBuyButtonCaption,
                                          Vec2(x, y),
                                          0,
                                          EventTouchBuy,
                                          kAKMenuStoreBeforePurchase,
                                          true);

    // リストアボタンを作成する
    x = AKScreenSize::positionFromLeftRatio(kAKRestoreButtonPosLeftRatio);
    y = AKScreenSize::positionFromTopRatio(kAKButtonPosTopRatio);
    m_restoreButton = interface->addLabelMenu(kAKRestoreButtonCaption,
                                              Vec2(x, y),
                                              0,
                                              EventTouchRestore,
                                              kAKMenuStoreBeforePurchase,
                                              true);

    // メッセージボックスを作成する
    AKLabel *message = AKLabel::createLabel("",
                                            kAKMsgLength,
                                            kAKMsgLineCount,
                                            kAKLabelFrameMessage,
                                            AKLabel::MessageFont);
    
    // 配置位置を設定する
    x = AKScreenSize::center().x;
    y = AKScreenSize::positionFromBottomPoint(kAKMsgPosBottomPoint);
    message->setPosition(Vec2(x, y));
    
    // 表示文字列を設定する
    string storeMessage = LocalizedResource::getInstance().getString(kAKStoreMessageKey);
    
    // 価格を付加する
    string priceString = Payment::GetPriceString(ProductIDRemoveAd);
    if (priceString.length() > 0) {
        storeMessage.append("\n・");
        storeMessage.append(priceString);
    }
    
    message->setString(storeMessage);
    
    // レイヤーに配置する
    interface->addChild(message, 0, kAKMenuStore);
    
    // 購入済みの文字列を取得する
    string purchasedMessage = LocalizedResource::getInstance().getString(kAKStorePurchasedKey);
    
    // 文字数を取得する。全角文字も1文字として扱う。
    int purhcaseMessageLength = AKStringSplitter::getStringLength(purchasedMessage.c_str());
    
    // 購入済みのラベルを作成する
    AKLabel *purchasedLabel = AKLabel::createLabel(purchasedMessage,
                                                   purhcaseMessageLength,
                                                   1,
                                                   kAKLabelFrameNone,
                                                   AKLabel::ControlFont);
    
    // 購入済みのラベルの位置を設定する
    x = AKScreenSize::center().x;
    y = AKScreenSize::positionFromTopRatio(kAKPurchasedCaptionPosTopRatio);
    purchasedLabel->setPosition(Vec2(x, y));
    
    // レイヤーに配置する
    interface->addChild(purchasedLabel, 0, kAKMenuStoreAfterPurcase);
}

// Game CenterページでコントローラのAボタンを押した時の処理
void AKOptionScene::onKeyDownAButtonAtGameCenterPage(int cursorPosition)
{
    // カーソル位置ごとに処理を分岐する
    switch (cursorPosition) {
        case CursorPositionLeaderboard:
            selectLeaderboard();
            break;
            
        default:
            AKAssert(false, "cursorPosition is wrong : %d", cursorPosition);
            break;
    }
}

// StoreページでコントローラのAボタンを押した時の処理
void AKOptionScene::onKeyDownAButtonAtStorePage(int cursorPosition)
{
    // 購入済みの場合は処理しない
    SettingFileIO &setting = SettingFileIO::GetInstance();
    if (setting.IsPurchased()) {
        return;
    }
    
    // カーソル位置ごとに処理を分岐する
    switch (cursorPosition) {
        case CursorPositionBuy:
            selectBuy();
            break;
            
        case CursorPositionRestore:
            selectRestore();
            break;
            
        default:
            AKAssert(false, "cursorPosition is wrong : %d", cursorPosition);
            break;
    }
}

// Game Centerページでのカーソル位置取得
Vec2 AKOptionScene::getCursorPositionAtGameCenterPage(int positionID)
{
    float x = 0.0f;
    float y = 0.0f;
    
    // カーソル位置IDごとに処理を分岐する
    switch (positionID) {
        case CursorPositionLeaderboard:
            x = m_leaderboardButton->getPosition().x - m_leaderboardButton->getWidth() / 2 + getCursorPositionMargin();
            y = m_leaderboardButton->getPosition().y;
            break;
            
        default:
            AKAssert(false, "positionID is wrong : %d", positionID);
            break;
    }
    
    return Vec2(x, y);
}

// Storeページでのカーソル位置取得
Vec2 AKOptionScene::getCursorPositionAtStorePage(int positionID)
{
    float x = 0.0f;
    float y = 0.0f;
    
    // カーソル位置IDごとに処理を分岐する
    switch (positionID) {
        case CursorPositionBuy:
            x = m_buyButton->getPosition().x - m_buyButton->getWidth() / 2 + getCursorPositionMargin();
            y = m_buyButton->getPosition().y;
            break;
            
        case CursorPositionRestore:
            x = m_restoreButton->getPosition().x - m_restoreButton->getWidth() / 2 + getCursorPositionMargin();
            y = m_restoreButton->getPosition().y;
            break;
            
        default:
            AKAssert(false, "positionID is wrong : %d", positionID);
            break;
    }
    
    return Vec2(x, y);
}

// Leaderboardボタン選択時の処理
void AKOptionScene::selectLeaderboard()
{
    // メニュー選択時の効果音を鳴らす
    SimpleAudioEngine::getInstance()->playEffect(kAKSelectSEFileName);
    
    // ボタンのブリンクアクションを作成する。
    // ブリンクアクション終了後にLeaderboardを表示する。
    // ブリンクアクションの途中でViewを表示させると、消えた状態でアニメーションが止まることがあるため。
    Blink *blink = Blink::create(0.2f, 2);
    CallFunc *callFunc = CallFunc::create(std::bind(std::mem_fun(&AKOptionScene::showLeaderboard), this));
    Sequence *action = Sequence::create(blink, callFunc, NULL);
    
    // ブリンクアクションを開始する
    m_leaderboardButton->runAction(action);
}

// Leaderboard表示
void AKOptionScene::showLeaderboard()
{
    // ブリンク終了直後はボタン非表示になっているため、表示を元に戻す
    m_leaderboardButton->setVisible(true);
    
    // ランキング画面を表示する
    aklib::OnlineScore::openRanking();
}

// 購入ボタン選択時の処理
void AKOptionScene::selectBuy()
{
    // メニュー選択時の効果音を鳴らす
    SimpleAudioEngine::getInstance()->playEffect(kAKSelectSEFileName);
    
    // ボタンのブリンクアクションを作成する
    Blink *action = Blink::create(0.2f, 2);
    
    // ブリンクアクションを開始する
    m_buyButton->runAction(action);
    
    // 購入処理を行う
    if (Payment::Pay(ProductIDRemoveAd, this)) {
        
        // 通信ビューを表示する
        startConnect();
    }
}

// リストアボタン選択時の処理
void AKOptionScene::selectRestore()
{
    // メニュー選択時の効果音を鳴らす
    SimpleAudioEngine::getInstance()->playEffect(kAKSelectSEFileName);
    
    // ボタンのブリンクアクションを作成する
    Blink *action = Blink::create(0.2f, 2);
    
    // ブリンクアクションを開始する
    m_restoreButton->runAction(action);
    
    // リストア処理を行う
    if (Payment::Restore(this)) {
        
        // 通信ビューを表示する
        startConnect();
    }
}

// 通信開始
void AKOptionScene::startConnect()
{
    AKLog(kAKLogOptionScene_1, "start");

    // 画面入力を無効化する
    disableOperation();
    
    // 通信中レイヤーを作成する
    createConnectingLayer();
    
    // 通信中インジケータを作成する
    createConnectingIndicator();
}

// 通信中レイヤー作成
void AKOptionScene::createConnectingLayer()
{
    // 画面サイズを取得する
    Size winSize = Director::getInstance()->getVisibleSize();
    
    // 半透明、黒色のレイヤーを作成する
    m_connectingLayer = LayerColor::create(Color4B(0, 0, 0, 128),
                                           winSize.width,
                                           winSize.height);
    
    // シーンに追加する
    addChild(m_connectingLayer, ZPositionConnectionLayer);
}

// 通信中インジケータ作成
void AKOptionScene::createConnectingIndicator()
{
    // 通信中インジケータを作成する
    m_indicator = ActivityIndicator::create();
    
    // 画面サイズを取得する
    Size winSize = Director::getInstance()->getVisibleSize();
    
    // 画面中央に配置する
    m_indicator->setPosition(winSize.width / 2, winSize.height / 2);
    
    // シーンに追加する
    addChild(m_indicator, ZPositionIndicator);
}