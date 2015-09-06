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
 @file AKHowToPlayScene.cpp
 @brief プレイ方法画面シーンクラスの定義
 
 プレイ方法画面シーンクラスを定義する。
 */

#include "AKHowToPlayScene.h"
#include "AKTitleScene.h"
#include "base/CCEventListenerController.h"

using cocos2d::Vec2;
using cocos2d::Node;
using cocos2d::Sprite;
using cocos2d::SpriteFrameCache;
using cocos2d::LayerColor;
using cocos2d::TransitionFade;
using cocos2d::Director;
using cocos2d::EventListenerController;
using cocos2d::Controller;
using cocos2d::Event;
using CocosDenshion::SimpleAudioEngine;

// シーンに配置するノードのz座標
enum {
    kAKHowToBackPosZ = 0,   ///< 背景のz座標
    kAKHowToItemPosZ,       ///< 背景以外のz座標
    kAKHowToInterface       ///< インターフェースのz座標
};

// シーンに配置するノードのタグ
enum {
    kAKHowToIntarfeceTag = 0,   ///< インターフェースのタグ
    kAKHowToImageTag,           ///< 説明画像のタグ
    kAKHowToMessageTag,         ///< メッセージのタグ
    kAKHowToPageTag             ///< ページ番号のタグ
};

// イベント番号
enum {
    kAKEventTouchPrevButton = 0,    ///< 前ページボタン
    kAKEventTouchNextButton,        ///< 次ページボタン
    kAKEventTouchBackButton         ///< 戻るボタン
};

/// 遊び方説明画像テクスチャアトラス定義ファイル名
static const char *kAKHowToTextureAtlasDefFile = "HowTo.plist";
/// 遊び方説明画像テクスチャアトラスファイル名
static const char *kAKHowToTextureAtlasFile = "HowTo.png";
/// 表示画像ファイル名
static const char *kAKFileNameFormat = "HowTo_%d.png";
/// 前ページボタンの画像ファイル名
static const char *kAKHowToPrevImage = "PrevButton.png";
/// 次ページボタンの画像ファイル名
static const char *kAKHowToNextImage = "NextButton.png";
/// 戻るボタンの画像ファイル名
static const char *kAKHowToBackImage = "BackButton.png";
/// ページ数表示のフォーマット
static const char *kAKHowToPageFormat = "%d / %d";

/// 前ページボタンの位置、左からの位置
static const float kAKHowToPrevPosLeftPoint = 40.0f;
/// 次ページボタンの位置、右からの位置
static const float kAKHowToNextPosRightPoint = 40.0f;
/// ページ番号の位置、上からの位置
static const float kAKHowToPagePosTopPoint = 60.0f;
/// 戻るボタンの位置、右からの位置
static const float kAKHowToBackPosRightPoint = 26.0f;
/// 戻るボタンの位置、上からの位置
static const float kAKHowToBackPosTopPoint = 26.0f;
/// Lボタンの位置、左からの位置
static const float LButtonPosLeftPoint = kAKHowToPrevPosLeftPoint;
/// Lボタンの位置、中心からの縦方向の位置
static const float LButtonPosVerticalCenterPoint = 28.0f;
/// Rボタンの位置、右からの位置
static const float RButtonPosRightPoint = kAKHowToNextPosRightPoint;
/// Rボタンの位置、中心からの縦方向の位置
static const float RButtonPosVerticalCenterPoint = 28.0f;
/// Bボタンの位置、右からの位置
static const float BButtonPosRightPoint =kAKHowToBackPosRightPoint;
/// Bボタンの位置、上からの位置
static const float BButtonPosTopPoint = kAKHowToBackPosTopPoint + 32.0f;

/// メッセージボックスの1行の文字数
static const int kAKHowToMsgLength = 20;
/// メッセージボックスの行数
static const int kAKHowToMsgLineCount = 3;
/// メッセージボックスの位置、下からの位置
static const float kAKHowToMsgPosBottomPoint = 65.0f;

/// イメージ表示位置、下からの位置
static const float kAKHowToImagePosBottomPoint = 180.0f;

/// ページ数
static const int kAKHowToPageCount = 6;

/// 前ページボタンのタグ
static const unsigned int kAKHowToPrevTag = 0x01;
/// 次ページボタンのタグ
static const unsigned int kAKHowToNextTag = 0x02;
/// 戻るボタンのタグ
static const unsigned int kAKHowToBackTag = 0x03;

/*!
 @brief コンビニエンスコンストラクタ

 インスタンスを生成し、初期化処理を行い、autoreleaseを行う。
 @return 生成したインスタンス
 */
AKHowToPlayScene* AKHowToPlayScene::create()
{
    AKHowToPlayScene *instance = new AKHowToPlayScene();
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
 
 画像を解放する。
 未使用のスプライトフレームを解放する。
 */
AKHowToPlayScene::~AKHowToPlayScene()
{
    AKLog(kAKLogHowToPlayScene_1, "start");
    
    // 画像を解放する
    if (m_image != NULL) {
        removeChild(m_image, true);
    }
    if (m_bButton != NULL) {
        removeChild(m_bButton, true);
    }
    if (m_lButton != NULL) {
        removeChild(m_lButton, true);
    }
    if (m_rButton != NULL) {
        removeChild(m_rButton, true);
    }
    
    // 未使用のスプライトフレームを解放する
    SpriteFrameCache *spriteFrameCache = SpriteFrameCache::getInstance();
    spriteFrameCache->removeUnusedSpriteFrames();
    
    AKLog(kAKLogHowToPlayScene_1, "end");
}

/*!
 @brief 初期化処理
 
 初期化処理を行う。
 @retval true 初期化成功
 @retval false 初期化失敗
 */
bool AKHowToPlayScene::init()
{
    // スーパークラスの初期化処理を行う。
    if (!Scene::init()) {
        return false;
    }
    
    // テクスチャアトラスを読み込む
    SpriteFrameCache *spriteFrameCache = SpriteFrameCache::getInstance();
    spriteFrameCache->addSpriteFramesWithFile(kAKControlTextureAtlasDefFile, kAKControlTextureAtlasFile);
    spriteFrameCache->addSpriteFramesWithFile(kAKHowToTextureAtlasDefFile, kAKHowToTextureAtlasFile);

    // 背景色レイヤーを作成する
    LayerColor *backColor = AKCreateBackColorLayer();
    
    // シーンに配置する
    addChild(backColor, kAKHowToBackPosZ);
    
    // メッセージボックスを作成する
    AKLabel *message = AKLabel::createLabel("", kAKHowToMsgLength, kAKHowToMsgLineCount, kAKLabelFrameMessage, AKLabel::MessageFont);
    
    // 配置位置を設定する
    message->setPosition(Vec2(AKScreenSize::center().x,
                                 AKScreenSize::positionFromBottomPoint(kAKHowToMsgPosBottomPoint)));
    
    // シーンに配置する
    addChild(message, kAKHowToItemPosZ, kAKHowToMessageTag);
    
    // ページ番号の初期文字列を作成する
    char pageString[16] = "";
    snprintf(pageString, sizeof(pageString), kAKHowToPageFormat, 1, kAKHowToPageCount);
    
    // ページ番号のラベルを作成する
    AKLabel *pageLabel = AKLabel::createLabel(pageString, (int)strlen(pageString), 1, kAKLabelFrameNone, AKLabel::ControlFont);
    
    // ページ番号の位置を設定する
    float x = AKScreenSize::center().x;
    float y = AKScreenSize::positionFromTopPoint(kAKHowToPagePosTopPoint);
    pageLabel->setPosition(Vec2(x, y));
    
    // ページ番号のラベルをシーンに配置する
    addChild(pageLabel, kAKHowToItemPosZ, kAKHowToPageTag);
    
    // インターフェースを作成する
    AKInterface *interface = AKInterface::create(this);
    
    // シーンへ配置する
    addChild(interface, 0, kAKHowToIntarfeceTag);
    
    // 前ページボタンをインターフェースに配置する
    x = AKScreenSize::positionFromLeftPoint(kAKHowToPrevPosLeftPoint);
    y = AKScreenSize::center().y;
    interface->addSpriteMenu(kAKHowToPrevImage,
                             Vec2(x, y),
                             kAKHowToItemPosZ,
                             kAKEventTouchPrevButton,
                             kAKHowToPrevTag,
                             kAKMenuTypeButton);

    // 次ページボタンをインターフェースに配置する
    x = AKScreenSize::positionFromRightPoint(kAKHowToNextPosRightPoint);
    y = AKScreenSize::center().y;
    interface->addSpriteMenu(kAKHowToNextImage,
                             Vec2(x, y),
                             kAKHowToItemPosZ,
                             kAKEventTouchNextButton,
                             kAKHowToNextTag,
                             kAKMenuTypeButton);
    
    // 戻るボタンをインターフェースに配置する
    x = AKScreenSize::positionFromRightPoint(kAKHowToBackPosRightPoint);
    y = AKScreenSize::positionFromTopPoint(kAKHowToBackPosTopPoint);
    interface->addSpriteMenu(kAKHowToBackImage,
                             Vec2(x, y),
                             kAKHowToItemPosZ,
                             kAKEventTouchBackButton,
                             kAKHowToBackTag,
                             kAKMenuTypeButton);
                             
    // ゲームコントローラ関連イベントハンドラを登録する。
    EventListenerController* controllerListener = EventListenerController::create();
    
    controllerListener->onConnected = CC_CALLBACK_2(AKHowToPlayScene::onConnectedController, this );
    controllerListener->onDisconnected = CC_CALLBACK_2(AKHowToPlayScene::onDisconnectedController, this );
    controllerListener->onKeyDown = CC_CALLBACK_3(AKHowToPlayScene::onKeyDown, this);
    controllerListener->onKeyUp = CC_CALLBACK_3(AKHowToPlayScene::onKeyUp, this);
    controllerListener->onAxisEvent = CC_CALLBACK_3(AKHowToPlayScene::onAxisEvent, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(controllerListener, this);
    
    // コントローラの検出を開始する。
    Controller::startDiscoveryController();
    
    // Bボタン画像を読み込む
    m_bButton = Sprite::createWithSpriteFrameName(BButtonImageFileName);
    
    // Bボタン画像の位置を設定する
    x = AKScreenSize::positionFromRightPoint(BButtonPosRightPoint);
    y = AKScreenSize::positionFromTopPoint(BButtonPosTopPoint);
    m_bButton->setPosition(x, y);
    
    // Bボタン画像を配置する
    this->addChild(m_bButton, kAKHowToItemPosZ);
    
    // Lボタン画像を読み込む
    m_lButton = Sprite::createWithSpriteFrameName(LButtonImageFileName);
    
    // Lボタン画像の位置を設定する
    x = AKScreenSize::positionFromLeftPoint(LButtonPosLeftPoint);
    y = AKScreenSize::positionFromVerticalCenterPoint(LButtonPosVerticalCenterPoint);
    m_lButton->setPosition(x, y);
    
    // Lボタン画像を配置する
    this->addChild(m_lButton, kAKHowToItemPosZ);

    // Rボタン画像を読み込む
    m_rButton = Sprite::createWithSpriteFrameName(RButtonImageFileName);
    
    // Rボタン画像の位置を設定する
    x = AKScreenSize::positionFromRightPoint(RButtonPosRightPoint);
    y = AKScreenSize::positionFromVerticalCenterPoint(RButtonPosVerticalCenterPoint);
    m_rButton->setPosition(x, y);
    
    // Rボタン画像を配置する
    this->addChild(m_rButton, kAKHowToItemPosZ);
    
    // 初期ページ番号を設定する
    setPageNo(1);
    
    return true;
}

/*!
 @brief イベント実行
 
 イベントを実行する。
 @param item 選択されたメニュー項目
 */
void AKHowToPlayScene::execEvent(const AKMenuItem *item)
{
    // 選択された項目に応じて処理を行う
    switch (item->getEventNo()) {
        case kAKEventTouchPrevButton:   // 前ページボタン
            goPrevPage();
            break;

        case kAKEventTouchNextButton:   // 次ページボタン
            goNextPage();
            break;

        case kAKEventTouchBackButton:   // 戻るボタン
            backToTitle();
            break;
            
        default:
            AKAssert(false, "不正なイベント番号:%d", item->getEventNo());
            break;
    }
}

/*!
 @brief コントローラー接続時処理
 
 コントローラーが接続された時の処理を行う。
 @param controller コントローラー
 @param event イベント
 */
void AKHowToPlayScene::onConnectedController(Controller* controller, Event* event)
{
    // ボタン表示の更新を行う
    updatePageButton();
}

/*!
 @brief コントローラー切断時処理
 
 コントローラーが切断された時の処理を行う。
 @param controller コントローラー
 @param event イベント
 */
void AKHowToPlayScene::onDisconnectedController(Controller* controller, Event* event)
{
    // コントローラのボタンを非表示にする
    m_bButton->setVisible(false);
    m_lButton->setVisible(false);
    m_rButton->setVisible(false);
}

/*!
 @brief コントローラーのボタンを押した時の処理
 
 コントローラーがボタンを押した時の処理を行う。
 @param controller コントローラー
 @param keyCode キーの種類
 @param event イベント
 */
void AKHowToPlayScene::onKeyDown(Controller* controller, int keyCode, Event* event)
{
    // キーの種類に応じて処理を分岐する
    switch (keyCode) {
        case Controller::BUTTON_B:
            
            // タイトルへ戻る
            backToTitle();
            break;
            
        case Controller::BUTTON_LEFT_SHOULDER:
            
            // 前ページへ移動する
            if (m_pageNo > 1) {
                goPrevPage();
            }
            break;
            
        case Controller::BUTTON_RIGHT_SHOULDER:
            
            // 次ページへ移動する
            if (m_pageNo < kAKHowToPageCount) {
                goNextPage();
            }
            break;
            
        default:
            break;
    }
}

/*!
 @brief コントローラーのボタンを離した時の処理
 
 コントローラーがボタンを離した時の処理を行う。
 @param controller コントローラー
 @param keyCode キーの種類
 @param event イベント
 */
void AKHowToPlayScene::onKeyUp(Controller* controller, int keyCode, Event* event)
{
}

/*!
 @brief コントローラーの方向キー入力処理
 
 コントローラーが方向キーを入力した時の処理を行う。
 @param controller コントローラー
 @param keyCode キーの種類
 @param event イベント
 */
void AKHowToPlayScene::onAxisEvent(Controller* controller, int keyCode, Event* event)
{
}

/*!
 @brief インターフェース取得
 
 インターフェースを取得する
 @return インターフェース
 */
AKInterface* AKHowToPlayScene::getInterface()
{
    AKInterface *interface = static_cast<AKInterface*>(getChildByTag(kAKHowToIntarfeceTag));

    AKAssert(interface != NULL, "インターフェースが作成されていない");

    return interface;
}

/*!
 @brief 前ページボタン取得
 
 前ページボタンを取得する
 @return 前ページボタン
 */
Node* AKHowToPlayScene::getPrevButton()
{
    Node *prevButton = getInterface()->getChildByTag(kAKHowToPrevTag);

    AKAssert(prevButton != NULL, "前ページボタンが作成されていない");

    return prevButton;
}

/*!
 @brief 次ページボタン取得
 
 次ページボタンを取得する
 @return 次ページボタン
 */
Node* AKHowToPlayScene::getNextButton()
{
    Node *nextButton = getInterface()->getChildByTag(kAKHowToNextTag);

    AKAssert(nextButton != NULL, "次ページボタンが作成されていない");

    return nextButton;
}

/*!
 @brief ページ番号ラベル取得
 
 ページ番号ラベルを取得する
 @return ページ番号ラベル
 */
AKLabel* AKHowToPlayScene::getPageLabel()
{
    AKLabel *pageLabel = static_cast<AKLabel*>(getChildByTag(kAKHowToPageTag));

    AKAssert(pageLabel != NULL, "ページ番号が作成されていない");

    return pageLabel;
}

/*!
 @brief メッセージラベル取得
 
 メッセージラベルを取得する
 @return メッセージラベル
 */
AKLabel* AKHowToPlayScene::getMessageLabel()
{
    AKLabel *messageLabel = static_cast<AKLabel*>(getChildByTag(kAKHowToMessageTag));

    AKAssert(messageLabel != NULL, "メッセージラベルが作成されていない");

    return messageLabel;
}

/*! 
 @brief ページ番号設定
 
 ページ番号を設定する。
 @param pageNo ページ番号
 */
void AKHowToPlayScene::setPageNo(int pageNo)
{
    AKAssert(pageNo > 0 || pageNo <= kAKHowToPageCount, "ページ番号が範囲外:pageNo=%d", pageNo);
    
    // ページ番号を変更する
    m_pageNo = pageNo;
    
    // 前ページ次ページボタンの表示を更新する
    updatePageButton();
    
    // ページ番号のラベルを更新する
    updatePageLabel();
    
    // 表示文字列のキーを生成する
    char key[32] = "";
    
    // コントローラが接続されている場合はメッセージを切り替える
    if (Controller::getAllController().size() > 0) {
        snprintf(key, sizeof(key), "HowToPlay_Controller_%d", m_pageNo);
    }
    else {
        snprintf(key, sizeof(key), "HowToPlay_%d", m_pageNo);
    }
    
    // 表示文字列を変更する
    getMessageLabel()->setString(aklib::LocalizedResource::getInstance().getString(key));
    
    // 表示ファイル名を作成する
    char fileName[16] = "";
    snprintf(fileName, sizeof(fileName), kAKFileNameFormat, m_pageNo);
    
    AKLog(kAKLogHowToPlayScene_1, "画像ファイル名:%s", fileName);
    
    // すでに画像を読み込んでいるときは画像を解放する
    if (m_image != NULL) {
        removeChild(m_image, true);
    }
    
    // ファイルからスプライトを作成する
    m_image = Sprite::createWithSpriteFrameName(fileName);
    
    // 表示位置を設定する
    float x = AKScreenSize::center().x;
    float y = AKScreenSize::positionFromBottomPoint(kAKHowToImagePosBottomPoint);
    m_image->setPosition(Vec2(x, y));
    
    // シーンに追加する
    addChild(m_image, kAKHowToItemPosZ, kAKHowToImageTag);
}

/*!
 @brief 前ページ次ページボタン表示非表示更新
 
 前ページ、次ページボタンの表示非表示を現在のページ番号に応じて更新する。
 最初のページの場合は前ページボタンを無効にする。
 最後のページの場合は次ページボタンを無効にする。
 */
void AKHowToPlayScene::updatePageButton()
{
    // 有効タグの初期値を0(すべて無効)とする
    unsigned int enableTag = 0;
    
    // 最初のページ以外の場合は前ページボタンを有効にする
    if (m_pageNo > 1) {

        // 有効タグに前ページボタンを追加する
        enableTag |= kAKHowToPrevTag;
    }
    
    // 最後のページ以外の場合は次ページボタンを有効にする
    if (m_pageNo < kAKHowToPageCount) {
        
        // 有効タグに次ページボタンを追加する
        enableTag |= kAKHowToNextTag;
    }
    
    // 有効タグをインターフェースに反映する
    getInterface()->setEnableTag(enableTag);
    
    // コントローラが接続されている場合はコントローラボタンガイドを表示する
    if (Controller::getAllController().size() > 0) {
        
        // Bボタンを表示する
        m_bButton->setVisible(true);
        
        // 最初のページ以外の場合はLボタンを表示する
        if (m_pageNo > 1) {
            m_lButton->setVisible(true);
        }
        else {
            m_lButton->setVisible(false);
        }
        
        // 最後のページ以外の場合はRボタンを表示する
        if (m_pageNo < kAKHowToPageCount) {
            m_rButton->setVisible(true);
        }
        else {
            m_rButton->setVisible(false);
        }
    }
    else {
        m_bButton->setVisible(false);
        m_lButton->setVisible(false);
        m_rButton->setVisible(false);
    }
}

/*!
 @brief ページ番号表示更新
 
 ページ番号のラベルを更新する。
 */
void AKHowToPlayScene::updatePageLabel()
{
    // ページ番号の文字列を作成する
    char pageString[16] = "";
    snprintf(pageString, sizeof(pageString), kAKHowToPageFormat, m_pageNo, kAKHowToPageCount);
    
    // ページ番号のラベルを更新する
    getPageLabel()->setString(pageString);
}

/*!
 @brief 前ページ表示
 
 前ページを表示する。ページ番号を一つ減らす。
 */
void AKHowToPlayScene::goPrevPage()
{
    AKLog(kAKLogHowToPlayScene_1, "goPrevPage開始");
    
    // メニュー選択時の効果音を鳴らす
    SimpleAudioEngine::getInstance()->playEffect(kAKSelectSEFileName);

    setPageNo(m_pageNo - 1);
}

/*!
 @brief 次ページ表示
 
 次ページを表示する。ページ番号を一つ増やす。
 */
void AKHowToPlayScene::goNextPage()
{
    AKLog(kAKLogHowToPlayScene_1, "goNextPage開始");
    
    // メニュー選択時の効果音を鳴らす
    SimpleAudioEngine::getInstance()->playEffect(kAKSelectSEFileName);

    setPageNo(m_pageNo + 1);
}

/*!
 @brief タイトルへ戻る
 
 タイトル画面シーンへ遷移する。
 */
void AKHowToPlayScene::backToTitle()
{
    AKLog(kAKLogHowToPlayScene_1, "backToTitle開始");

    // メニュー選択時の効果音を鳴らす
    SimpleAudioEngine::getInstance()->playEffect(kAKSelectSEFileName);

    // タイトルシーンへの遷移を作成する
    TransitionFade *transition = TransitionFade::create(0.5f,
                                                        AKTitleScene::create());
    
    // タイトルシーンへ遷移する
    Director *director = Director::getInstance();
    director->replaceScene(transition);
}
