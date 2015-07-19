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
 @file CreditScene.cpp
 @brief クレジットシーンクラスの定義
 
 クレジットシーンを表示するクラスを定義する。
 */

#include "CreditScene.h"
#include "AKTitleScene.h"
#include "OpenUrl.h"
#include "base/CCEventListenerController.h"

using cocos2d::Vec2;
using cocos2d::LayerColor;
using cocos2d::TransitionFade;
using cocos2d::Director;
using cocos2d::SpriteFrameCache;
using cocos2d::EventListenerController;
using cocos2d::Controller;
using cocos2d::Event;
using cocos2d::Sprite;
using CocosDenshion::SimpleAudioEngine;
using aklib::OpenUrl;

/// 前ページボタンの位置、左からの位置
const float CreditScene::PrevPosLeftPoint = 40.0f;
/// 次ページボタンの位置、右からの位置
const float CreditScene::NextPosRightPoint = 40.0f;
/// ページ番号の位置、上からの位置
const float CreditScene::PagePosTopPoint = 60.0f;
/// 戻るボタンの位置、右からの位置
const float CreditScene::BackPosRightPoint = 26.0f;
/// 戻るボタンの位置、上からの位置
const float CreditScene::BackPosTopPoint = 26.0f;
/// Lボタンの位置、左からの位置
const float CreditScene::LButtonPosLeftPoint = PrevPosLeftPoint;
/// Lボタンの位置、中心からの縦方向の位置
const float CreditScene::LButtonPosVerticalCenterPoint = 28.0f;
/// Rボタンの位置、右からの位置
const float CreditScene::RButtonPosRightPoint = NextPosRightPoint;
/// Rボタンの位置、中心からの縦方向の位置
const float CreditScene::RButtonPosVerticalCenterPoint = 28.0f;
/// Bボタンの位置、右からの位置
const float CreditScene::BButtonPosRightPoint = BackPosRightPoint;
/// Bボタンの位置、上からの位置
const float CreditScene::BButtonPosTopPoint = BackPosTopPoint + 32.0f;
/// カーソル画像の位置のボタンとの重なりの幅
const float CreditScene::CursorPosOverlap = 8.0f;
/// 前ページボタンの画像ファイル名
const char *CreditScene::PrevImage = "PrevButton.png";
/// 次ページボタンの画像ファイル名
const char *CreditScene::NextImage = "NextButton.png";
/// 戻るボタンの画像ファイル名
const char *CreditScene::BackImage = "BackButton.png";
/// ページ数表示のフォーマット
const char *CreditScene::PageFormat = "%d / %d";
/// リンクの個数
const int CreditScene::LinkNum = 5;
/// クレジットラベルの位置
const float CreditScene::CreditPosLeftRatio = 0.35f;
/// クレジットラベルの行数
const int CreditScene::CreditLineNum = 3;
/// クレジットラベルの1行の文字数
const int CreditScene::CreditLineLength = 14;
/// クレジットラベルテキストのキー
const char *CreditScene::CreditTextKey = "CreditName_%d";
/// クレジットラベルテキストのキー
const char *CreditScene::UrlTextKey = "CreditLink_%d";
/// 最大ページ番号
const int CreditScene::MaxPageNum = (LinkNum - 1) / LinkNumOfPage + 1;
/// リンクボタンのキャプション
const char *CreditScene::LinkCaption = "WEB SITE";
/// リンクボタンの位置、左からの比率
const float CreditScene::LinkPosLeftRatio = 0.7f;
/// リンクボタンの位置、上からの比率
const float CreditScene::LinkPosTopRatio[] = {0.35f, 0.6f, 0.85f};
/// 前ページボタンのタグ
const unsigned int CreditScene::PrevTag = 0x01;
/// 次ページボタンのタグ
const unsigned int CreditScene::NextTag = 0x02;
/// 戻るボタンのタグ
const unsigned int CreditScene::BackTag = 0x03;
/// リンクボタンのタグ
const unsigned int CreditScene::LinkTag[] = {0x04, 0x08, 0x10};

/*!
 @brief コンビニエンスコンストラクタ
 
 インスタンスを生成し、初期化処理を行い、autoreleaseを行う。
 @return 生成したインスタンス
 */
CreditScene* CreditScene::create()
{
    CreditScene *instance = new CreditScene();
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
 
 メンバを解放する。
 */
CreditScene::~CreditScene()
{
    // メンバを解放する
    if (m_interface != NULL) {
        m_interface->release();
    }
    
    // 画像を解放する
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
}

/*!
 @brief 初期化処理
 
 初期化処理を行う。
 @retval true 初期化成功
 @retval false 初期化失敗
 */
bool CreditScene::init()
{
    // スーパークラスの初期化処理を行う。
    if (!Scene::init()) {
        return false;
    }
    
    // テクスチャアトラスを読み込む
    SpriteFrameCache *spriteFrameCache = SpriteFrameCache::getInstance();
    spriteFrameCache->addSpriteFramesWithFile(kAKControlTextureAtlasDefFile, kAKControlTextureAtlasFile);

    // 背景色レイヤーを作成する
    LayerColor *backColor = AKCreateBackColorLayer();
    
    // シーンに配置する
    addChild(backColor, PosZBackGround);
    
    // ページ番号の初期文字列を作成する
    char pageString[16] = "";
    snprintf(pageString, sizeof(pageString), PageFormat, 1, MaxPageNum);
    
    // ページ番号のラベルを作成する
    m_pageLabel = AKLabel::createLabel(pageString, (int)strlen(pageString), 1, kAKLabelFrameNone);
    
    // ページ番号の位置を設定する
    float x = AKScreenSize::center().x;
    float y = AKScreenSize::positionFromTopPoint(PagePosTopPoint);
    m_pageLabel->setPosition(Vec2(x, y));

    // ページ番号のラベルをシーンに配置する
    addChild(m_pageLabel, PosZItems);

    // インターフェースを作成する
    m_interface = AKInterface::create(this);
    m_interface->retain();
    
    // インターフェースをシーンに配置する
    addChild(m_interface, PosZInterface);
    
    // 前ページボタンをインターフェースに配置する
    x = AKScreenSize::positionFromLeftPoint(PrevPosLeftPoint);
    y = AKScreenSize::center().y;
    m_interface->addSpriteMenu(PrevImage,
                               Vec2(x, y),
                               PosZItems,
                               MenuPrevPage,
                               PrevTag,
                               kAKMenuTypeButton);
    
    // 次ページボタンをインターフェースに配置する
    x = AKScreenSize::positionFromRightPoint(NextPosRightPoint);
    y = AKScreenSize::center().y;
    m_interface->addSpriteMenu(NextImage,
                               Vec2(x, y),
                               PosZItems,
                               MenuNextPage,
                               NextTag,
                               kAKMenuTypeButton);

    // 戻るボタンをインターフェースに配置する
    x = AKScreenSize::positionFromRightPoint(BackPosRightPoint);
    y = AKScreenSize::positionFromTopPoint(BackPosTopPoint);
    m_interface->addSpriteMenu(BackImage,
                               Vec2(x, y),
                               PosZItems,
                               MenuBackToTitle,
                               BackTag,
                               kAKMenuTypeButton);
    
    // リンクボタンをインターフェースに配置する
    for (int i = 0; i < LinkNumOfPage; i++) {
        
        x = AKScreenSize::positionFromLeftRatio(LinkPosLeftRatio);
        y = AKScreenSize::positionFromTopRatio(LinkPosTopRatio[i]);
        m_link[i] = m_interface->addLabelMenu(LinkCaption,
                                              Vec2(x, y),
                                              PosZItems,
                                              MenuLink1 + i,
                                              LinkTag[i],
                                              true);
    }
    
    // クレジットラベルをインターフェースに配置する
    for (int i = 0; i < LinkNumOfPage; i++) {
        
        m_creditLabel[i] = AKLabel::createLabel("", CreditLineLength, CreditLineNum, kAKLabelFrameNone);
        
        x = AKScreenSize::positionFromLeftRatio(CreditPosLeftRatio);
        y = AKScreenSize::positionFromTopRatio(LinkPosTopRatio[i]);
        m_creditLabel[i]->setPosition(Vec2(x, y));
        
        addChild(m_creditLabel[i], PosZItems);
    }
    
    // ゲームコントローラ関連イベントハンドラを登録する。
    EventListenerController* controllerListener = EventListenerController::create();
    
    controllerListener->onConnected = CC_CALLBACK_2(CreditScene::onConnectedController, this );
    controllerListener->onDisconnected = CC_CALLBACK_2(CreditScene::onDisconnectedController, this );
    controllerListener->onKeyDown = CC_CALLBACK_3(CreditScene::onKeyDown, this);
    controllerListener->onKeyUp = CC_CALLBACK_3(CreditScene::onKeyUp, this);
    controllerListener->onAxisEvent = CC_CALLBACK_3(CreditScene::onAxisEvent, this);
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
    this->addChild(m_bButton, PosZItems);
    
    // Lボタン画像を読み込む
    m_lButton = Sprite::createWithSpriteFrameName(LButtonImageFileName);
    
    // Lボタン画像の位置を設定する
    x = AKScreenSize::positionFromLeftPoint(LButtonPosLeftPoint);
    y = AKScreenSize::positionFromVerticalCenterPoint(LButtonPosVerticalCenterPoint);
    m_lButton->setPosition(x, y);
    
    // Lボタン画像を配置する
    this->addChild(m_lButton, PosZItems);
    
    // Rボタン画像を読み込む
    m_rButton = Sprite::createWithSpriteFrameName(RButtonImageFileName);
    
    // Rボタン画像の位置を設定する
    x = AKScreenSize::positionFromRightPoint(RButtonPosRightPoint);
    y = AKScreenSize::positionFromVerticalCenterPoint(RButtonPosVerticalCenterPoint);
    m_rButton->setPosition(x, y);
    
    // Rボタン画像を配置する
    this->addChild(m_rButton, PosZItems);
    
    // カーソル画像を読み込む。
    m_cursor = Sprite::createWithSpriteFrameName(CursorImageFileName);
    
    // カーソル画像をシーンに配置する
    this->addChild(m_cursor, PosZCursor);
    
    // 初期ページ番号を設定する
    setPageNo(1);
    
    // 1個目のリンクを選択する
    selectMenuItem(0);
    
    return true;
}

/*!
 @brief イベント実行
 
 イベントを実行する。
 @param item 選択されたメニュー項目
 */
void CreditScene::execEvent(const AKMenuItem *item)
{
    // 選択された項目に応じて処理を行う
    switch (item->getEventNo()) {
        case MenuBackToTitle:   // タイトルへ戻るボタン
            backToTitle();
            break;
            
        case MenuPrevPage:      // 前ページボタン
            goPrevPage();
            break;
            
        case MenuNextPage:      // 次ページボタン
            goNextPage();
            break;
            
        case MenuLink1:         // リンクボタン1
            openLink(1);
            break;
            
        case MenuLink2:         // リンクボタン2
            openLink(2);
            break;
            
        case MenuLink3:         // リンクボタン3
            openLink(3);
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
void CreditScene::onConnectedController(Controller* controller, Event* event)
{
    // ボタン表示の更新を行う
    updateButton();
}

/*!
 @brief コントローラー切断時処理
 
 コントローラーが切断された時の処理を行う。
 @param controller コントローラー
 @param event イベント
 */
void CreditScene::onDisconnectedController(Controller* controller, Event* event)
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
void CreditScene::onKeyDown(Controller* controller, int keyCode, Event* event)
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
            if (m_pageNo < MaxPageNum) {
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
void CreditScene::onKeyUp(Controller* controller, int keyCode, Event* event)
{
}

/*!
 @brief コントローラーの方向キー入力処理
 
 コントローラーが方向キーを入力した時の処理を行う。
 @param controller コントローラー
 @param keyCode キーの種類
 @param event イベント
 */
void CreditScene::onAxisEvent(Controller* controller, int keyCode, Event* event)
{
    const auto& keyStatus = controller->getKeyStatus(keyCode);
    
    // 連続入力禁止用に前回入力内容を持つ
    static int prevInput = 0;
    
    // y軸方向の操作の場合は処理を行う
    if (keyCode == Controller::JOYSTICK_LEFT_Y) {
        
        const int MaxMenu = MIN(LinkNum - (m_pageNo - 1) * LinkNumOfPage, LinkNumOfPage);
        
        // しきい値よりも小さい場合は上方向に選択項目を移動する
        if (keyStatus.value < -kAKControllerAxisThreshold) {
            
            // 前回入力値が上方向以外の場合は処理する
            if (prevInput != -1) {
                
                // メニュー項目をデクリメントする
                m_selectMenu--;
                
                // 下限以下になった場合は末尾へ移動する
                if (m_selectMenu < 0) {
                    m_selectMenu = MaxMenu - 1;
                }
                
                // カーソル移動時の効果音を鳴らす
                SimpleAudioEngine::getInstance()->playEffect(kAKCursorSEFileName);
                
                // メニュー項目を選択する
                selectMenuItem(m_selectMenu);
                
                // 連続入力を防止するために今回入力内容を記憶する
                prevInput = -1;
            }
        }
        // しきい値よりも大きい場合は下方向に選択項目を移動する
        else if (keyStatus.value > kAKControllerAxisThreshold) {
            
            // 前回入力値が下方向以外の場合は処理する
            if (prevInput != 1) {
                
                // メニュー項目をインクリメントする
                m_selectMenu++;
                
                // 上限以上になった場合は末尾へ移動する
                if (m_selectMenu >= MaxMenu) {
                    m_selectMenu = 0;
                }
                
                // カーソル移動時の効果音を鳴らす
                SimpleAudioEngine::getInstance()->playEffect(kAKCursorSEFileName);
                
                // メニュー項目を選択する
                selectMenuItem(m_selectMenu);
                
                prevInput = 1;
            }
        }
        // しきい値以内の場合は無処理
        else {
            prevInput = 0;
        }
    }
}

/*!
 @brief タイトルへ戻る
 
 タイトル画面シーンへ遷移する。
 */
void CreditScene::backToTitle()
{    
    // メニュー選択時の効果音を鳴らす
    SimpleAudioEngine::getInstance()->playEffect(kAKSelectSEFileName);
    
    // タイトルシーンへの遷移を作成する
    TransitionFade *transition = TransitionFade::create(0.5f,
                                                        AKTitleScene::create());
    
    // タイトルシーンへ遷移する
    Director *director = Director::getInstance();
    director->replaceScene(transition);
}

/*!
 @brief 前ページ表示
 
 前ページを表示する。ページ番号を一つ減らす。
 */
void CreditScene::goPrevPage()
{
    // メニュー選択時の効果音を鳴らす
    SimpleAudioEngine::getInstance()->playEffect(kAKSelectSEFileName);
    
    setPageNo(m_pageNo - 1);
}

/*!
 @brief 次ページ表示
 
 次ページを表示する。ページ番号を一つ増やす。
 */
void CreditScene::goNextPage()
{
    // メニュー選択時の効果音を鳴らす
    SimpleAudioEngine::getInstance()->playEffect(kAKSelectSEFileName);
    
    setPageNo(m_pageNo + 1);
}

/*!
 @brief リンクボタン押下
 
 選択したリンクを開く。
 @param リンクボタンの番号
 */
void CreditScene::openLink(int linkNumber)
{
    // メニュー選択時の効果音を鳴らす
    SimpleAudioEngine::getInstance()->playEffect(kAKSelectSEFileName);
    
    // URL文字列のキーを生成する
    char key[16] = "";
    snprintf(key, sizeof(key), UrlTextKey, linkNumber + (m_pageNo - 1) * LinkNumOfPage);
    
    // URLを開く
    OpenUrl::open(aklib::LocalizedResource::getInstance().getString(key).c_str());
}

/*!
 @brief ページ番号設定
 
 ページ番号を設定する。
 @param pageNo ページ番号
 */
void CreditScene::setPageNo(int pageNo)
{
    AKAssert(pageNo > 0 || pageNo <= MaxPageNum, "ページ番号が範囲外:pageNo=%d", pageNo);
    
    // ページ番号を変更する
    m_pageNo = pageNo;
    
    // ボタンの表示を更新する
    updateButton();
    
    // ページ番号のラベルを更新する
    updatePageLabel();
    
    // クレジットラベルを更新する
    updateCreditLabel();
}

/*!
 @brief ボタン表示非表示更新
 
 各ボタンの表示非表示を現在のページ番号に応じて更新する。
 最初のページの場合は前ページボタンを無効にする。
 最後のページの場合は次ページボタンを無効にする。
 リンクボタンは最後のページ以外はすべて表示する。
 最後のページの場合は
 */
void CreditScene::updateButton()
{
    // 有効タグの初期値を0(すべて無効)とする
    unsigned int enableTag = 0;
    
    // 最初のページ以外の場合は前ページボタンを有効にする
    if (m_pageNo > 1) {
        
        // 有効タグに前ページボタンを追加する
        enableTag |= PrevTag;
    }
    
    // 最後のページ以外の場合は次ページボタンを有効にする
    if (m_pageNo < MaxPageNum) {
        
        // 有効タグに次ページボタンを追加する
        enableTag |= NextTag;
    }
    
    // 最後のページ以外の場合は全リンクボタンを有効にする
    // 全リンクボタンが1ページあたりのリンクボタンの個数で割り切れる場合は常に全リンクボタンを有効にする
    if (m_pageNo < MaxPageNum || (LinkNum % LinkNumOfPage == 0)) {
        for (int i = 0; i < LinkNumOfPage; i++) {
            enableTag |= LinkTag[i];
        }
    }
    // 最後のページの場合は1ページあたりのリンクボタンの個数で割った余りの個数のリンクボタンを有効にする
    else {
        for (int i = 0; i < LinkNum % LinkNumOfPage; i++) {
            enableTag |= LinkTag[i];
        }
    }
    
    // 有効タグをインターフェースに反映する
    m_interface->setEnableTag(enableTag);
    
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
        if (m_pageNo < MaxPageNum) {
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
void CreditScene::updatePageLabel()
{
    // ページ番号の文字列を作成する
    char pageString[16] = "";
    snprintf(pageString, sizeof(pageString), PageFormat, m_pageNo, MaxPageNum);
    
    // ページ番号のラベルを更新する
    m_pageLabel->setString(pageString);
}

/*!
 @brief クレジットラベル表示更新
 
 表示しているページに合わせてクレジットラベルの表示内容を更新する。
 */
void CreditScene::updateCreditLabel()
{
    // 1ページの各項目のラベルを更新する
    for (int i = 0; i < LinkNumOfPage; i++) {

        // 表示個数の範囲内ならば、リソースからテキストを取得してラベルに設定する
        if (i + (m_pageNo - 1) * LinkNumOfPage + 1 <= LinkNum) {
            
            // 表示文字列のキーを生成する
            char key[16] = "";
            snprintf(key, sizeof(key), CreditTextKey, i + (m_pageNo - 1) * LinkNumOfPage + 1);
            
            // 表示文字列を変更する
            m_creditLabel[i]->setString(aklib::LocalizedResource::getInstance().getString(key));
        }
        else {
            
            // 表示文字列を変更する
            m_creditLabel[i]->setString("");
        }
    }
}

/*!
 @brief メニュー項目選択
 
 メニュー項目を選択する。
 @param item 選択項目
 */
void CreditScene::selectMenuItem(int item)
{
    // 選択項目が範囲外の場合はカーソル非表示とする
    if (item < 0 || item >= LinkNumOfPage) {
        m_cursor->setVisible(false);
        return;
    }
 
    // x座標を決める
    float x = m_link[item]->getPosition().x - m_link[item]->getWidth() / 2 - m_cursor->getContentSize().width / 2 + CursorPosOverlap;
    
    // 選択項目に応じてカーソルのy座標を決定する
    float y = m_link[item]->getPosition().y;
    
    // カーソルの位置を設定する
    m_cursor->setPosition(x, y);
    
    // カーソルを表示する
    m_cursor->setVisible(true);
    
    // 選択中の項目を変更する
    m_selectMenu = item;
}

