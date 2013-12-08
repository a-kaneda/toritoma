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
//#include "AKInAppPurchaseHelper.h"

using cocos2d::CCSpriteFrameCache;
using cocos2d::CCLayerColor;
using cocos2d::CCPoint;
using cocos2d::SEL_CallFunc;
using cocos2d::CCBlink;
using cocos2d::CCCallFunc;
using cocos2d::CCSequence;
using cocos2d::CCTransitionFade;
using cocos2d::CCDirector;
using CocosDenshion::SimpleAudioEngine;

// オプション画面シーンに配置するノードのタグ
enum {
    kAKOptionSceneBackColor = 0,    ///< 背景色のタグ
    kAKOptionSceneInterface         ///< インターフェースレイヤーのタグ
};

// メニューイベント番号
enum {
    kAKSelectLeaderboard = 0,   ///< Leaderboardボタン
    kAKSelectAchievements,      ///< Achievementsボタン
    kAKSelectPrevPage,          ///< 前ページボタン
    kAKSelectNextPage,          ///< 次ページボタン
    kAKSelectBack,              ///< 戻るボタン
    kAKSelectBuy,               ///< 購入ボタン
    kAKSelectRestore            ///< リストアボタン
};

/// Game Cetnerのページ
static const int kAKPageGameCenter = 1;
/// Storeのページ
static const int kAKPageStore = 2;
/// 全ページに表示する項目のタグ
static const unsigned int kAKMenuAll = 0x00;
/// Game Centerページの項目のタグ
static const unsigned int kAKMenuGameCenter = 0x01;
/// Storeページ(共通)の項目のタグ
static const unsigned int kAKMenuStore = 0x02;
/// Storeページ(購入前)の項目のタグ
static const unsigned int kAKMenuStoreBeforePurchase = 0x04;
/// Storeページ(購入後)の項目のタグ
static const unsigned int kAKMenuStoreAfterPurcase = 0x08;
/// Option画面ページ数
static const int kAKMenuPageCount = 2;

/// 前ページボタンの画像ファイル名
static const char *kAKPrevImage = "PrevButton.png";
/// 次ページボタンの画像ファイル名
static const char *kAKNextImage = "NextButton.png";
/// 戻るボタンの画像ファイル名
static const char *kAKBackImage = "BackButton.png";
/// 前ページボタンの位置、左からの位置
static const float kAKPrevPosLeftPoint = 40.0f;
/// 次ページボタンの位置、右からの位置
static const float kAKNextPosRightPoint = 40.0f;
/// 戻るボタンの位置、右からの位置
static const float kAKBackPosRightPoint = 26.0f;
/// 戻るボタンの位置、上からの位置
static const float kAKBackPosTopPoint = 26.0f;

/// Game Centerのキャプション
static const char *kAKGameCenterCaption = "GAME CENTER";
/// Leaderboardボタンのキャプション
static const char *kAKLeaderboardCaption = "LEADERBOARD";
/// Achievementsボタンのキャプション
static const char *kAKAchievementsCaption = "ACHIEVEMENTS";
/// Game Centerのキャプション位置、上からの比率
static const float kAKGameCenterCaptionPosTopRatio = 0.1f;
/// Leaderboardボタンの位置、上からの比率
static const float kAKLeaderboardPosTopRatio = 0.3f;
//: Achievementsボタンの位置、上からの比率
static const float kAKAchievemetnsPosTopRatio = 0.5f;

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
static const float kAKStoreCaptionPosTopRatio = 0.1f;
/// ボタンの位置、上からの比率
static const float kAKButtonPosTopRatio = 0.3f;
/// 購入ボタンの位置、左からの比率
static const float kAKBuyButtonPosLeftRatio = 0.3f;
/// リストアボタンの位置、左からの比率
static const float kAKRestoreButtonPosLeftRatio = 0.7f;
/// メッセージボックスの1行の文字数
static const int kAKMsgLength = 20;
/// メッセージボックスの行数
static const int kAKMsgLineCount = 3;
/// メッセージボックスの位置、下からの位置
static const float kAKMsgPosBottomPoint = 120.0f;
/// 購入済みのキャプションの位置、上からの比率
static const float kAKPurchasedCaptionPosTopRatio = 0.3f;

/*!
 @brief コンビニエンスコンストラクタ
 
 インスタンスを生成し、初期化処理を行い、autoreleaseを行う。
 @return 生成したインスタンス
 */
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

/*!
 @brief イベント実行
 
 イベントを実行する。
 @param item 選択されたメニュー項目
 */
void AKOptionScene::execEvent(const AKMenuItem *item)
{
    // 選択された項目に応じて処理を行う
    switch (item->getEventNo()) {
        case kAKSelectLeaderboard:
            selectLeaderboard();
            break;

        case kAKSelectAchievements:
            selectAchievements();
            break;

        case kAKSelectPrevPage:
            selectPrevPage();
            break;

        case kAKSelectNextPage:
            selectNextPage();
            break;

        case kAKSelectBack:
            selectBack();
            break;

        case kAKSelectBuy:
            selectBuy();
            break;

        case kAKSelectRestore:
            selectRestore();
            break;

        default:
            AKAssert(false, "不正なイベント番号:%d", item->getEventNo());
            break;
    }
}

/*!
 @brief インスタンス初期化処理
 
 インスタンスの初期化を行う。
 @return 初期化したインスタンス。失敗時はnilを返す。
 */
AKOptionScene::AKOptionScene()
{
    // テクスチャアトラスを読み込む
    CCSpriteFrameCache *spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    spriteFrameCache->addSpriteFramesWithFile(kAKControlTextureAtlasDefFile, kAKControlTextureAtlasFile);

    // 背景色レイヤーを作成する
    CCLayerColor *backColor = AKCreateBackColorLayer();
    
    // シーンへ配置する
    addChild(backColor, kAKOptionSceneBackColor, kAKOptionSceneBackColor);
    
    // インターフェースを作成する
    AKInterface *interface = AKInterface::create(this);
    
    // シーンへ配置する
    addChild(interface, kAKOptionSceneInterface, kAKOptionSceneInterface);
    
    // 通信中フラグはオフにする
    m_isConnecting = false;
    
    // TODO:アプリ課金が有効な場合は全ページを表示できるようにする
//    if ([[AKInAppPurchaseHelper sharedHelper] canMakePayments]) {
//        m_maxPage = kAKMenuPageCount;
//    }
    // アプリ課金が禁止されている場合はページをひとつ減らす
//    else {
        m_maxPage = kAKMenuPageCount - 1;
//    }
    
    // 全ページ共通の項目を作成する
    initCommonItem(interface);
    
    // Game Centerページの項目を作成する
    initGameCenterPage(interface);
    
    // Storeページの項目を作成する
    initStorePage(interface);

    // 初期ページを1ページ目とする
    setPageNo(1);
}

/*!
 @brief ページ共通の項目作成
 
 全ページ共通の項目を作成する。
 戻るボタン、次ページボタン、前ページボタンを作成する。
 @param interface インターフェースレイヤー
 */
void AKOptionScene::initCommonItem(AKInterface *interface)
{
    // 前ページボタンをインターフェースに配置する
    CCPoint prevPoint(AKScreenSize::positionFromLeftPoint(kAKPrevPosLeftPoint),
                      AKScreenSize::center().y);
    interface->addSpriteMenu(kAKPrevImage,
                             prevPoint,
                             0,
                             kAKSelectPrevPage,
                             kAKMenuAll,
                             kAKMenuTypeButton);
    
    // 次ページボタンをインターフェースに配置する
    CCPoint nextPoint(AKScreenSize::positionFromRightPoint(kAKNextPosRightPoint),
                      AKScreenSize::center().y);
    interface->addSpriteMenu(kAKNextImage,
                             nextPoint,
                             0,
                             kAKSelectNextPage,
                             kAKMenuAll,
                             kAKMenuTypeButton);
    
    // 戻るボタンをインターフェースに配置する
    CCPoint backPoint(AKScreenSize::positionFromRightPoint(kAKBackPosRightPoint),
                      AKScreenSize::positionFromTopPoint(kAKBackPosTopPoint));
    interface->addSpriteMenu(kAKBackImage,
                             backPoint,
                             0,
                             kAKSelectBack,
                             kAKMenuAll,
                             kAKMenuTypeButton);
}

/*! 
 @brief Game Centerページの項目作成
 
 Game Centerページの項目を作成する。
 Game Centerのラベル、Leaderboardボタン、Achievementsボタンを作成する。
 @param interface インターフェースレイヤー
 */
void AKOptionScene::initGameCenterPage(AKInterface *interface)
{
    // Game Centerのラベルを作成する
    AKLabel *gameCenterLabel = AKLabel::createLabel(kAKGameCenterCaption,
                                                    strlen(kAKGameCenterCaption),
                                                    1,
                                                    kAKLabelFrameNone);
    
    // Game Centerのラベルの位置を設定する
    float x = AKScreenSize::center().x;
    float y = AKScreenSize::positionFromTopRatio(kAKGameCenterCaptionPosTopRatio);
    gameCenterLabel->setPosition(CCPoint(x, y));
    
    // Game Centerのラベルを配置する
    addChild(gameCenterLabel, 0, kAKMenuGameCenter);
    
    // Leaderboardのメニューを作成する
    x = AKScreenSize::center().x;
    y = AKScreenSize::positionFromTopRatio(kAKLeaderboardPosTopRatio);
    m_leaderboardButton = interface->addLabelMenu(kAKLeaderboardCaption,
                                                  CCPoint(x, y),
                                                  0,
                                                  kAKSelectAchievements,
                                                  kAKMenuGameCenter,
                                                  true);
    
    // Achievementsのメニューを作成する
    x = AKScreenSize::center().x;
    y = AKScreenSize::positionFromTopRatio(kAKAchievemetnsPosTopRatio);
    m_achievementsButton = interface->addLabelMenu(kAKAchievementsCaption,
                                                  CCPoint(x, y),
                                                  0,
                                                  kAKSelectAchievements,
                                                  kAKMenuGameCenter,
                                                  true);
}

/*!
 @brief Storeページの項目作成
 
 Storeページの項目を作成する。
 Storeラベル、説明ラベル、購入ボタンを作成する。
 @param interface インターフェースレイヤー
 */
void AKOptionScene::initStorePage(AKInterface *interface)
{
    // Storeのラベルを作成する
    AKLabel *storeLabel = AKLabel::createLabel(kAKStoreCaption,
                                               strlen(kAKStoreCaption),
                                               1,
                                               kAKLabelFrameNone);
    
    // Storeのラベルの位置を設定する
    float x = AKScreenSize::center().x;
    float y = AKScreenSize::positionFromTopRatio(kAKStoreCaptionPosTopRatio);
    storeLabel->setPosition(CCPoint(x, y));
    
    // Storeのラベルを配置する
    addChild(storeLabel, 0, kAKMenuStore);
    
    // 購入ボタンを作成する
    x = AKScreenSize::positionFromLeftRatio(kAKBuyButtonPosLeftRatio);
    y = AKScreenSize::positionFromTopRatio(kAKButtonPosTopRatio);
    m_buyButton = interface->addLabelMenu(kAKBuyButtonCaption,
                                          CCPoint(x, y),
                                          0,
                                          kAKSelectBuy,
                                          kAKMenuStoreBeforePurchase,
                                          true);

    // リストアボタンを作成する
    x = AKScreenSize::positionFromLeftRatio(kAKRestoreButtonPosLeftRatio);
    y = AKScreenSize::positionFromTopRatio(kAKButtonPosTopRatio);
    m_restoreButton = interface->addLabelMenu(kAKRestoreButtonCaption,
                                              CCPoint(x, y),
                                              0,
                                              kAKSelectRestore,
                                              kAKMenuStoreBeforePurchase,
                                              true);

    // メッセージボックスを作成する
    AKLabel *message = AKLabel::createLabel("",
                                            kAKMsgLength,
                                            kAKMsgLineCount,
                                            kAKLabelFrameMessage);
    
    // 配置位置を設定する
    x = AKScreenSize::center().x;
    y = AKScreenSize::positionFromBottomPoint(kAKMsgPosBottomPoint);
    message->setPosition(CCPoint(x, y));
    
    // 表示文字列を設定する
    message->setString(CCLocalizedString(kAKStoreMessageKey, "Storeページの説明文"));
    
    // レイヤーに配置する
    interface->addChild(message, 0, kAKMenuStore);
    
    // 購入済みの文字列を取得する
    const char *purchasedMessage = CCLocalizedString(kAKStorePurchasedKey,
                                                     "購入済みのキャプション");
    
    // 購入済みのラベルを作成する
    AKLabel *purchasedLabel = AKLabel::createLabel(purchasedMessage,
                                                   strlen(purchasedMessage),
                                                   1,
                                                   kAKLabelFrameNone);
    
    // 購入済みのラベルの位置を設定する
    x = AKScreenSize::center().x;
    y = AKScreenSize::positionFromTopRatio(kAKPurchasedCaptionPosTopRatio);
    purchasedLabel->setPosition(CCPoint(x, y));
    
    // レイヤーに配置する
    interface->addChild(purchasedLabel, 0, kAKMenuStoreAfterPurcase);
}

/*!
 @brief ページ番号設定
 
 表示ページを変更する。
 範囲外の場合はページをループする。
 インターフェースの有効タグをページ番号にあわせて変更する。
 @param pageNo ページ番号
 */
void AKOptionScene::setPageNo(int pageNo)
{
    // 最小値未満の場合は最大ページへループする
    if (pageNo < 1) {
        m_pageNo = m_maxPage;
    }
    // 最大値超過の場合は最小ページヘループする
    else if (pageNo > m_maxPage) {
        m_pageNo = 1;
    }
    // 範囲内の場合はそのまま設定する
    else {
        m_pageNo = pageNo;
    }
    
    // インターフェースを取得する
    AKInterface *interface = static_cast<AKInterface*>(getChildByTag(kAKOptionSceneInterface));
    
    // ページ番号によってインターフェースのタグを変更する
    interface->setEnableTag(getInterfaceTag(m_pageNo));
}

/*!
 @brief Leaderboardボタン選択時の処理
 
 Leaderboardボタン選択時の処理。
 Leaderboardを表示する。
 */
void AKOptionScene::selectLeaderboard()
{
    // メニュー選択時の効果音を鳴らす
    SimpleAudioEngine::sharedEngine()->playEffect(kAKSelectSEFileName);
    
    // ボタンのブリンクアクションを作成する。
    // ブリンクアクション終了後にLeaderboardを表示する。
    // ブリンクアクションの途中でViewを表示させると、消えた状態でアニメーションが止まることがあるため。
    CCBlink *blink = CCBlink::create(0.2f, 2);
    CCCallFunc *callFunc = CCCallFunc::create(this,
                                              callfunc_selector(AKOptionScene::showLeaderboard));
    CCSequence *action = CCSequence::create(blink, callFunc, NULL);
    
    // ブリンクアクションを開始する
    m_leaderboardButton->runAction(action);
}

/*!
 @brief Achievementsボタン選択時の処理

 Achievementsボタン選択時の処理
 Achievementsを表示する。
 */
void AKOptionScene::selectAchievements()
{
    // メニュー選択時の効果音を鳴らす
    SimpleAudioEngine::sharedEngine()->playEffect(kAKSelectSEFileName);
    
    // ボタンのブリンクアクションを作成する
    // ブリンクアクション終了後にAchievementsを表示する。
    // ブリンクアクションの途中でViewを表示させると、消えた状態でアニメーションが止まることがあるため。
    CCBlink *blink = CCBlink::create(0.2f, 2);
    CCCallFunc *callFunc = CCCallFunc::create(this,
                                              callfunc_selector(AKOptionScene::showAchievements));
    CCSequence *action = CCSequence::create(blink, callFunc, NULL);
        
    // ブリンクアクションを開始する
    m_achievementsButton->runAction(action);
}

/*!
 @brief 前ページ表示
 
 前ページを表示する。
 ページ番号をデクリメントする。
 */
void AKOptionScene::selectPrevPage()
{
    // 通信中の場合は処理を行わない
    if (m_isConnecting) {
        return;
    }
    
    // メニュー選択時の効果音を鳴らす
    SimpleAudioEngine::sharedEngine()->playEffect(kAKSelectSEFileName);
    
    setPageNo(m_pageNo - 1);
}

/*!
 @brief 次ページ表示
 
 次ページを表示する。
 ページ番号をインクリメントする。
 */
void AKOptionScene::selectNextPage()
{
    // 通信中の場合は処理を行わない
    if (m_isConnecting) {
        return;
    }
    
    // TODO:メニュー選択時の効果音を鳴らす
//    [[SimpleAudioEngine sharedEngine] playEffect:kAKMenuSelectSE];
    
    setPageNo(m_pageNo + 1);
}

/*!
 @brief 戻るボタン選択時の処理
 
 戻るボタンを選択した時の処理を行う。
 効果音を鳴らし、タイトルシーンへと戻る。
 */
void AKOptionScene::selectBack()
{
    // 通信中の場合は処理を行わない
    if (m_isConnecting) {
        return;
    }
    
    // TODO:メニュー選択時の効果音を鳴らす
//    [[SimpleAudioEngine sharedEngine] playEffect:kAKMenuSelectSE];
    
    // タイトルシーンへの遷移を作成する
    CCTransitionFade *transition = CCTransitionFade::create(0.5f, AKTitleScene::create());
    
    // タイトルシーンへ遷移する
    CCDirector::sharedDirector()->replaceScene(transition);
}

/*!
 @brief Leaderboard表示
 
 Leaderboardを表示する。
 ボタンのブリンクアクション終了時に呼ばれるので、Leaderboard表示前にボタンのvisibleを表示に変更する。
 */
void AKOptionScene::showLeaderboard()
{
    // ブリンク終了直後はボタン非表示になっているため、表示を元に戻す
    m_leaderboardButton->setVisible(true);
    
    // TODO:Achievementsを表示する
//    AKGameCenterHelper::sharedHelper()->showLeaderboard();
}

/*! 
 @brief Achievements表示

 Achievementsを表示する。
 ボタンのブリンクアクション終了時に呼ばれるので、Achievements表示前にボタンのvisibleを表示に変更する。
 */
void AKOptionScene::showAchievements()
{
    // ブリンク終了直後はボタン非表示になっているため、表示を元に戻す
    m_achievementsButton->setVisible(true);
    
    // TODO:Achievementsを表示する
//    AKGameCenterHelper::sharedHelper()->showAchievements();
}

/*!
 @brief 購入ボタン選択時の処理
 
 購入ボタン選択時の処理を行う。
 ボタン選択時の効果音とエフェクトを発生させる。
 In App Purchaseの購入処理を行う。
 */
void AKOptionScene::selectBuy()
{
    // 通信中の場合は処理を行わない
    if (m_isConnecting) {
        return;
    }
    
    // TODO:メニュー選択時の効果音を鳴らす
//    [[SimpleAudioEngine sharedEngine] playEffect:kAKMenuSelectSE];
    
    // ボタンのブリンクアクションを作成する
    CCBlink *action = CCBlink::create(0.2f, 2);
    
    // ブリンクアクションを開始する
    m_buyButton->runAction(action);
    
    // 通信ビューを表示する
    startConnect();
    
    // TODO:購入処理を行う
//    [[AKInAppPurchaseHelper sharedHelper] buy];
}

/*!
 @brief リストアボタン選択時の処理
 
 リストアボタン選択時の処理を行う。
 ボタン選択時の効果音とエフェクトを発生させる。
 In App Purchaseのリストア処理を行う。
 */
void AKOptionScene::selectRestore()
{
    // 通信中の場合は処理を行わない
    if (m_isConnecting) {
        return;
    }
    
    // TODO:メニュー選択時の効果音を鳴らす
//    [[SimpleAudioEngine sharedEngine] playEffect:kAKMenuSelectSE];
    
    // ボタンのブリンクアクションを作成する
    CCBlink *action = CCBlink::create(0.2f, 2);
    
    // ブリンクアクションを開始する
    m_restoreButton->runAction(action);
    
    // 通信ビューを表示する
    startConnect();

    // TODO:リストア処理を行う
//    [[AKInAppPurchaseHelper sharedHelper] restore];
}

/*!
 @brief 通信開始
 
 通信開始時の処理を行う。
 通信中のビューを表示し、画面入力を無視するようにする。
 */
void AKOptionScene::startConnect()
{
    AKLog(kAKLogOptionScene_1, "start");
    
    /* TODO:通信中の処理を作成する
    // 画面入力を無効化する
    m_isConnecting = true;
    
    // ルートビューを取得する
    UIView *rootView = [UIApplication sharedApplication].keyWindow.rootViewController.view;
    
    // 通信中ビューを作成する
    self.connectingView = [[[UIView alloc] initWithFrame:rootView.bounds] autorelease];
    
    // 半透明の黒色とする
    self.connectingView.backgroundColor = [UIColor blackColor];
    self.connectingView.alpha = 0.5f;
    
    // インジケータを作成する
    UIActivityIndicatorView *indicator = [[[UIActivityIndicatorView alloc] init] autorelease];
    
    // 白色の大きいアイコンを表示する
    indicator.activityIndicatorViewStyle = UIActivityIndicatorViewStyleWhiteLarge;
    
    // 中心位置をルートビューと揃える
    // Landscapeのため、x座標とy座標を入れ替える
    indicator.center = ccp(rootView.center.y, rootView.center.x);
    
    // 通信中ビューにインジケータを配置する
    [self.connectingView addSubview:indicator];
    
    // ルートビューに通信中ビューを配置する
    [rootView addSubview:self.connectingView];
    
    // インジケータのアニメーションを開始する
    [indicator startAnimating];
     */
}

/*!
 @brief 通信終了
 
 通信終了時の処理を行う。
 通信中のビューを削除し、画面入力を有効にする。
 */
void AKOptionScene::endConnect()
{
    AKLog(kAKLogOptionScene_1, "start");
    
    /* TODO:通信中の処理を作成する
    // 通信中ビューを削除する
    [self.connectingView removeFromSuperview];
    self.connectingView = nil;

    // 画面入力を有効化する
    isConnecting_ = NO;
     */
}

/*!
 @brief インターフェース有効タグ取得
 
 インターフェースの有効タグをページ番号から作成する。
 @param page ページ番号
 @return インターフェース有効タグ
 */
unsigned int AKOptionScene::getInterfaceTag(int page)
{
    // ページによってインターフェースの有効タグを判定する
    switch (page) {
        case kAKPageGameCenter:     // Game Centerのページ
            return kAKMenuGameCenter;
            
        case kAKPageStore:          // Storeのページ
            
            // TODO:購入済みかどうかで有効化する項目を変える
            /*
            if ([AKInAppPurchaseHelper sharedHelper].isEnableContinue) {
                return kAKMenuStore | kAKMenuStoreAfterPurcase;
            }
            else {
                return kAKMenuStore | kAKMenuStoreBeforePurchase;
            }
             */
            return kAKMenuStore;
            
        default:                    // その他のページは存在しない
            AKAssert(false, "不正なページ番号:%d", page);
            return kAKMenuGameCenter;
    }
}
