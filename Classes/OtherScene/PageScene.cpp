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
 @file PageScene.cpp
 @brief ページを持ったシーン
 
 ページを持ったシーンを表示するクラス。
 */

#include "base/CCEventListenerController.h"
#include "PageScene.h"
#include "AKTitleScene.h"

using cocos2d::Controller;
using cocos2d::Director;
using cocos2d::Event;
using cocos2d::EventListenerController;
using cocos2d::LayerColor;
using cocos2d::Sprite;
using cocos2d::SpriteFrameCache;
using cocos2d::TransitionFade;
using cocos2d::Vec2;
using CocosDenshion::SimpleAudioEngine;

// ページ表示のフォーマット
const char *PageScene::PageFormat = "%d / %d";
// ページ表示位置、下からの位置
const float PageScene::PagePositionBottomPoint = 20.0f;
// 前ページボタンの位置、左からの位置
const float PageScene::PrevPositionLeftPoint = 40.0f;
// 次ページボタンの位置、右からの位置
const float PageScene::NextPositionRightPoint = 40.0f;
// 戻るボタンの位置、右からの位置
const float PageScene::BackPositionRightPoint = 26.0f;
// 戻るボタンの位置、上からの位置
const float PageScene::BackPositionTopPoint = 26.0f;
// 前ページボタンの画像ファイル名
const char *PageScene::PrevImage = "PrevButton.png";
// 次ページボタンの画像ファイル名
const char *PageScene::NextImage = "NextButton.png";
// 戻るボタンの画像ファイル名
const char *PageScene::BackImage = "BackButton.png";;
// 前ページボタンのタグ
const unsigned int PageScene::MenuTagPrev = 0x01;
// 次ページボタンのタグ
const unsigned int PageScene::MenuTagNext = 0x02;
// 戻るボタンのタグ
const unsigned int PageScene::MenuTagBack = 0x03;
// Lボタンの位置、左からの位置
const float PageScene::LButtonPosLeftPoint = PrevPositionLeftPoint;
// Lボタンの位置、中心からの縦方向の位置
const float PageScene::LButtonPosVerticalCenterPoint = 28.0f;
// Rボタンの位置、右からの位置
const float PageScene::RButtonPosRightPoint = NextPositionRightPoint;
// Rボタンの位置、中心からの縦方向の位置
const float PageScene::RButtonPosVerticalCenterPoint = 28.0f;
// Bボタンの位置、右からの位置
const float PageScene::BButtonPosRightPoint = BackPositionRightPoint;
// Bボタンの位置、上からの位置
const float PageScene::BButtonPosTopPoint = BackPositionTopPoint + 32.0f;
// カーソル画像の位置のボタンとの重なりの幅
const float PageScene::CursorPosOverlap = 8.0f;

// デストラクタ
PageScene::~PageScene()
{
    // 未使用のスプライトフレームを解放する
    SpriteFrameCache *spriteFrameCache = SpriteFrameCache::getInstance();
    spriteFrameCache->removeUnusedSpriteFrames();
}

// 初期化処理
bool PageScene::init()
{
    // スーパークラスの初期化処理を行う
    if (!Scene::init()) {
        return false;
    }
    
    // テクスチャアトラスを読み込む
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(kAKControlTextureAtlasDefFile, kAKControlTextureAtlasFile);
    
    // 背景を作成する
    createBackground();
    
    // ページ番号のラベルを作成する
    createPageLabel();
    
    // インターフェースを作成する
    createInterface();
    
    // ゲームコントローラ初期処理を行う
    initController();
    
    // Bボタン画像を作成する
    createBButtonImage();
    
    // Lボタン画像を作成する
    createLButtonImage();
    
    // Rボタン画像を作成する
    createRButtonImage();
    
    // カーソル画像を作成する
    createCursorImage();
    
    // 派生クラスの初期化処理を行う
    if (!initSub()) {
        return false;
    }
    
    // 初期ページ番号を設定する
    setPageNo(1);
    
    return true;
}

// イベント処理
void PageScene::execEvent(const AKMenuItem *item)
{
    // 操作が無効化されている場合は処理をしない
    if (_isDisabled) {
        return;
    }
    
    // 選択された項目に応じて処理を行う
    switch (item->getEventNo()) {
        case EventTouchPrevButton:  // 前ページボタン
            goPrevPage();
            break;
            
        case EventTouchNextButton:  // 次ページボタン
            goNextPage();
            break;
            
        case EventTouchBackButton:  // 戻るボタン
            backToTitle();
            break;
            
        default:                    // その他の項目は派生クラスで処理する
            execSubEvent(_pageNo, item);
            break;
    }
}

// コントローラー接続時処理
void PageScene::onConnectedController(Controller* controller, Event* event)
{
    // カーソル位置を初期化する
    setCursorPosition(0);
    
    // コントローラ用画像の表示を更新する
    updateControllerImage();
}

// コントローラー切断時処理
void PageScene::onDisconnectedController(Controller* controller, Event* event)
{
    // コントローラのボタンを非表示にする
    _bButton->setVisible(false);
    _lButton->setVisible(false);
    _rButton->setVisible(false);
    
    // カーソルを非表示にする
    _cursorImage->setVisible(false);
}

// コントローラーのボタンを押した時の処理
void PageScene::onKeyDown(Controller* controller, int keyCode, Event* event)
{
    // 操作が無効化されている場合は処理をしない
    if (_isDisabled) {
        return;
    }

    // キーの種類に応じて処理を分岐する
    switch (keyCode) {
        case Controller::BUTTON_B:
            
            // タイトルへ戻る
            backToTitle();
            
            break;
            
        case Controller::BUTTON_LEFT_SHOULDER:
            
            // 前ページへ移動する
            if (_pageNo > 1) {
                goPrevPage();
            }
            
            break;
            
        case Controller::BUTTON_RIGHT_SHOULDER:
            
            // 次ページへ移動する
            if (_pageNo < _maxPage) {
                goNextPage();
            }
            
            break;
            
        case cocos2d::Controller::BUTTON_A:
            
            // Aボタンを押した時の処理を行う
            onKeyDownAButton(_pageNo, _cursorPosition);
            
            break;
            
        default:
            break;
    }
}

// コントローラーのボタンを離した時の処理
void PageScene::onKeyUp(cocos2d::Controller *controller, int keyCode, cocos2d::Event *event)
{
    
}

// コントローラーの方向キー入力処理
void PageScene::onAxisEvent(cocos2d::Controller *controller, int keyCode, cocos2d::Event *event)
{
    // 操作が無効化されている場合は処理をしない
    if (_isDisabled) {
        return;
    }

    const auto& keyStatus = controller->getKeyStatus(keyCode);
    
    // x軸方向とy軸方向の操作で処理を分ける
    if (keyCode == Controller::JOYSTICK_LEFT_Y) {
        
        // しきい値よりも小さい場合は上方向の入力処理を行う
        if (keyStatus.value < -kAKControllerAxisThreshold) {
            
            // 連続で上方向に入力されていない場合は処理を行う
            if (!_isInputUp) {
                
                // 上方向入力時の処理を行う
                setCursorPosition(onLStickUp(_pageNo, _cursorPosition));
            }
            
            // 上方向入力中を記憶する
            _isInputUp = true;
            
            // 下方向の入力はなしとする
            _isInputDown = false;
        }
        // しきい値よりも大きい場合は下方向入力処理を行う
        else if (keyStatus.value > kAKControllerAxisThreshold) {
            
            // 連続で下方向に入力されていない場合は処理を行う
            if (!_isInputDown) {
                
                // 下方向入力時の処理を行う
                setCursorPosition(onLStickDown(_pageNo, _cursorPosition));
            }

            // 上方向の入力はなしとする
            _isInputUp = false;
            
            // 下方向入力中を記憶する
            _isInputDown = true;
        }
        // しきい値以内の場合は無処理
        else {
            
            // 上下方向の入力はなしとする
            _isInputUp = false;
            _isInputDown = false;
        }
    }
    else if (keyCode == Controller::JOYSTICK_LEFT_X) {
            
        // しきい値よりも小さい場合は左方向の入力処理を行う
        if (keyStatus.value < -kAKControllerAxisThreshold) {
            
            // 連続で左方向に入力されていない場合は処理を行う
            if (!_isInputLeft) {
                
                // 左方向入力時の処理を行う
                setCursorPosition(onLStickLeft(_pageNo, _cursorPosition));
            }
            
            // 左方向入力中を記憶する
            _isInputLeft = true;
            
            // 右方向の入力はなしとする
            _isInputRight = false;
        }
        // しきい値よりも大きい場合は右方向入力処理を行う
        else if (keyStatus.value > kAKControllerAxisThreshold) {
            
            // 連続で右方向に入力されていない場合は処理を行う
            if (!_isInputRight) {
                
                // 右方向入力時の処理を行う
                setCursorPosition(onLStickRight(_pageNo, _cursorPosition));
            }
            
            // 左方向の入力はなしとする
            _isInputLeft = false;
            
            // 右方向入力中を記憶する
            _isInputRight = true;
        }
        // しきい値以内の場合は無処理
        else {
            
            // 左右方向の入力はなしとする
            _isInputLeft = false;
            _isInputRight = false;
        }
    }
    else {
        // その他の場合は処理をしない
    }
}

// コンストラクタ
PageScene::PageScene(int maxPage)
{
    // 最大ページ数を設定する
    _maxPage = maxPage;
    
    // Lスティック入力有無を初期化する
    _isInputUp = false;
    _isInputDown = false;
    _isInputLeft = false;
    _isInputRight = false;
    
    // カーソル位置を初期化する
    _cursorPosition = 0;
    
    // 操作が無効かどうかを初期化する
    _isDisabled = false;
}

// 派生クラスの初期化処理
bool PageScene::initSub()
{
    // 継承先で処理を実装する

    return true;
}

// 派生クラスのイベント処理
void PageScene::execSubEvent(int pageNo, const AKMenuItem *item)
{
    // 継承先で処理を実装する
}

// コントローラのAボタンを押した時の処理
void PageScene::onKeyDownAButton(int pageNo, int cursorPosition)
{
    // 継承先で処理を実装する
}

// コントローラのLスティックを上に倒した時の処理
int PageScene::onLStickUp(int pageNo, int cursorPosition)
{
    // 継承先で処理を実装する
    
    return cursorPosition;
}

// コントローラのLスティックを下に倒した時の処理
int PageScene::onLStickDown(int pageNo, int cursorPosition)
{
    // 継承先で処理を実装する
    
    return cursorPosition;
}

// コントローラのLスティックを左に倒した時の処理
int PageScene::onLStickLeft(int pageNo, int cursorPosition)
{
    // 継承先で処理を実装する
    
    return cursorPosition;
}

// コントローラのLスティックを右に倒した時の処理
int PageScene::onLStickRight(int pageNo, int cursorPosition)
{
    // 継承先で処理を実装する
    
    return cursorPosition;
}

// ページ表示内容更新
unsigned int PageScene::updatePageContents(int pageNo)
{
    // 継承先で処理を実装する
    
    return 0;
}

// カーソル表示有無取得
bool PageScene::isVisibleCursor(int pageNo)
{
    // 継承先で処理を実装する

    return false;
}

// カーソル位置取得
Vec2 PageScene::getCursorPosition(int pageNo, int positionID)
{
    // 継承先で処理を実装する
    
    return Vec2(0, 0);
}

// インタフェース取得
AKInterface* PageScene::getInterface()
{
    return _interface;
}

// カーソル画像の位置のマージン取得
float PageScene::getCursorPositionMargin()
{
    return CursorPosOverlap - _cursorImage->getContentSize().width / 2;
}

// ページ数設定
void PageScene::setMaxPage(int maxPage)
{
    _maxPage = maxPage;
}

// 操作を無効化する
void PageScene::disableOperation()
{
    _isDisabled = true;
}

// 操作を有効化する
void PageScene::enableOperation()
{
    _isDisabled = false;
}

// 表示内容更新
void PageScene::updateView()
{
    // 現在のページを表示し直す
    setPageNo(_pageNo);    
}

// デフォルトコンストラクタ
PageScene::PageScene()
{
    AKAssert(false, "Default constructor is called.");
}

// 背景作成
void PageScene::createBackground()
{
    // 背景色レイヤーを作成する
    LayerColor *backColor = AKCreateBackColorLayer();
    
    // シーンに追加する
    addChild(backColor, ZPositionBack);
}

// ページラベル作成
void PageScene::createPageLabel()
{
    // ページ番号の初期文字列を作成する
    char pageString[16] = "";
    snprintf(pageString, sizeof(pageString), PageFormat, 1, _maxPage);
    
    // ページ番号のラベルを作成する
    _pageLabel = AKLabel::createLabel(pageString, (int)strlen(pageString), 1, kAKLabelFrameNone, AKLabel::ControlFont);
    
    // ページ番号の位置を設定する
    float x = AKScreenSize::center().x;
    float y = AKScreenSize::positionFromBottomPoint(PagePositionBottomPoint);
    _pageLabel->setPosition(x, y);
    
    // シーンに追加する
    addChild(_pageLabel, ZPositionItem);
}

// インターフェース作成
void PageScene::createInterface()
{
    // インターフェースを作成する
    _interface = AKInterface::create(this);
    
    // シーンへ配置する
    addChild(_interface, ZPositionInterface);
    
    // 前ページボタンを作成する
    createPrevPageButton();
    
    // 次ページボタンを作成する
    createNextPageButton();
    
    // 戻ろボタンを作成する
    createBackButton();
}

// 前ページボタン作成
void PageScene::createPrevPageButton()
{
    AKAssert(_interface != NULL, "interface_ is null.");
    
    // ボタンの位置を設定する
    float x = AKScreenSize::positionFromLeftPoint(PrevPositionLeftPoint);
    float y = AKScreenSize::center().y;
    
    // 前ページボタンをインターフェースに配置する
    _interface->addSpriteMenu(PrevImage,
                              Vec2(x, y),
                              0,
                              EventTouchPrevButton,
                              MenuTagPrev,
                              kAKMenuTypeButton);    
}

// 次ページボタン作成
void PageScene::createNextPageButton()
{
    AKAssert(_interface != NULL, "interface_ is null.");
    
    // ボタンの位置を設定する
    float x = AKScreenSize::positionFromRightPoint(NextPositionRightPoint);
    float y = AKScreenSize::center().y;

    // 次ページボタンをインターフェースに配置する
    _interface->addSpriteMenu(NextImage,
                              Vec2(x, y),
                              0,
                              EventTouchNextButton,
                              MenuTagNext,
                              kAKMenuTypeButton);
}

// 戻るボタン作成
void PageScene::createBackButton()
{
    AKAssert(_interface != NULL, "interface_ is null.");
    
    // ボタンの位置を設定する
    float x = AKScreenSize::positionFromRightPoint(BackPositionRightPoint);
    float y = AKScreenSize::positionFromTopPoint(BackPositionTopPoint);
    
    // 戻るボタンをインターフェースに配置する
    _interface->addSpriteMenu(BackImage,
                              Vec2(x, y),
                              0,
                              EventTouchBackButton,
                              MenuTagBack,
                              kAKMenuTypeButton);
}

// ゲームコントローラ初期処理
void PageScene::initController()
{
    // ゲームコントローラ関連イベントハンドラを登録する。
    EventListenerController* controllerListener = EventListenerController::create();
    
    controllerListener->onConnected = CC_CALLBACK_2(PageScene::onConnectedController, this );
    controllerListener->onDisconnected = CC_CALLBACK_2(PageScene::onDisconnectedController, this );
    controllerListener->onKeyDown = CC_CALLBACK_3(PageScene::onKeyDown, this);
    controllerListener->onKeyUp = CC_CALLBACK_3(PageScene::onKeyUp, this);
    controllerListener->onAxisEvent = CC_CALLBACK_3(PageScene::onAxisEvent, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(controllerListener, this);
    
    // コントローラの検出を開始する。
    Controller::startDiscoveryController();
}

// Bボタン画像作成
void PageScene::createBButtonImage()
{
    // Bボタン画像を読み込む
    _bButton = Sprite::createWithSpriteFrameName(BButtonImageFileName);
    
    // Bボタン画像の位置を設定する
    float x = AKScreenSize::positionFromRightPoint(BButtonPosRightPoint);
    float y = AKScreenSize::positionFromTopPoint(BButtonPosTopPoint);
    _bButton->setPosition(x, y);
    
    // Bボタン画像を配置する
    addChild(_bButton, ZPositionItem);
}

// Lボタン画像作成
void PageScene::createLButtonImage()
{
    // Lボタン画像を読み込む
    _lButton = Sprite::createWithSpriteFrameName(LButtonImageFileName);
    
    // Lボタン画像の位置を設定する
    float x = AKScreenSize::positionFromLeftPoint(LButtonPosLeftPoint);
    float y = AKScreenSize::positionFromVerticalCenterPoint(LButtonPosVerticalCenterPoint);
    _lButton->setPosition(x, y);
    
    // Lボタン画像を配置する
    addChild(_lButton, ZPositionItem);
}

// Rボタン画像作成
void PageScene::createRButtonImage()
{
    // Rボタン画像を読み込む
    _rButton = Sprite::createWithSpriteFrameName(RButtonImageFileName);
    
    // Rボタン画像の位置を設定する
    float x = AKScreenSize::positionFromRightPoint(RButtonPosRightPoint);
    float y = AKScreenSize::positionFromVerticalCenterPoint(RButtonPosVerticalCenterPoint);
    _rButton->setPosition(x, y);
    
    // Rボタン画像を配置する
    addChild(_rButton, ZPositionItem);
}

// カーソル画像作成
void PageScene::createCursorImage()
{
    // カーソル画像を読み込む。
    _cursorImage = Sprite::createWithSpriteFrameName(CursorImageFileName);
    
    // カーソル画像をシーンに配置する
    this->addChild(_cursorImage, ZPositionItem);
}

// ページ番号設定
void PageScene::setPageNo(int pageNo)
{
    AKAssert(pageNo > 0 || pageNo <= _maxPage, "ページ番号が範囲外:pageNo=%d", pageNo);
    
    // ページ番号を変更する
    _pageNo = pageNo;
    
    // カーソル位置を初期化する
    setCursorPosition(0);
    
    // コントローラ用画像の表示を更新する
    updateControllerImage();

    // ページ番号のラベルを更新する
    updatePageLabel();
    
    // 有効タグの初期値を0(すべて無効)とする
    unsigned int enableTag = 0;

    // 前ページ次ページボタンの表示を更新する
    enableTag |= updatePageButton();
    
    // ページ表示内容を更新する
    enableTag |= updatePageContents(_pageNo);
    
    // 有効タグをインターフェースに反映する
    _interface->setEnableTag(enableTag);
}

// カーソル位置設定
void PageScene::setCursorPosition(int cursorPosition)
{
    // カーソル位置を変更する
    _cursorPosition = cursorPosition;

    // カーソル画像の位置を設定する
    if (isVisibleCursor(_pageNo)) {
        _cursorImage->setPosition(getCursorPosition(_pageNo, _cursorPosition));
    }
}

// 前ページ次ページボタン表示非表示更新
unsigned int PageScene::updatePageButton()
{
    // 有効タグの初期値を0(すべて無効)とする
    unsigned int enableTag = 0;
    
    // 最初のページ以外の場合は前ページボタンを有効にする
    if (_pageNo > 1) {
        
        // 有効タグに前ページボタンを追加する
        enableTag |= MenuTagPrev;
    }
    
    // 最後のページ以外の場合は次ページボタンを有効にする
    if (_pageNo < _maxPage) {
        
        // 有効タグに次ページボタンを追加する
        enableTag |= MenuTagNext;
    }
    
    return enableTag;
}

/*!
 @brief ページ番号表示更新
 
 ページ番号のラベルを更新する。
 */
void PageScene::updatePageLabel()
{
    // ページ番号の文字列を作成する
    char pageString[16] = "";
    snprintf(pageString, sizeof(pageString), PageFormat, _pageNo, _maxPage);
    
    // ページ番号のラベルを更新する
    _pageLabel->setString(pageString);
}

// コントローラ用画像表示更新
void PageScene::updateControllerImage()
{
    // コントローラが接続されている場合はコントローラボタンガイドを表示する
    if (Controller::getAllController().size() > 0) {
        
        // Bボタンを表示する
        _bButton->setVisible(true);
        
        // 最初のページ以外の場合はLボタンを表示する
        if (_pageNo > 1) {
            _lButton->setVisible(true);
        }
        else {
            _lButton->setVisible(false);
        }
        
        // 最後のページ以外の場合はRボタンを表示する
        if (_pageNo < _maxPage) {
            _rButton->setVisible(true);
        }
        else {
            _rButton->setVisible(false);
        }
        
        // カーソルを表示する
        _cursorImage->setVisible(isVisibleCursor(_pageNo));
    }
    else {
        _bButton->setVisible(false);
        _lButton->setVisible(false);
        _rButton->setVisible(false);
        _cursorImage->setVisible(false);
    }
}

// 前ページ表示
void PageScene::goPrevPage()
{
    // ページ番号が2ページ目以降の場合は処理を行う
    if (_pageNo > 1) {

        // メニュー選択時の効果音を鳴らす
        SimpleAudioEngine::getInstance()->playEffect(kAKSelectSEFileName);
        
        // 1減らしたページ番号を設定する
        setPageNo(_pageNo - 1);
    }
}

// 次ページ表示
void PageScene::goNextPage()
{
    // ページ番号が最後のページでない場合は処理を行う
    if (_pageNo < _maxPage) {
        
        // メニュー選択時の効果音を鳴らす
        SimpleAudioEngine::getInstance()->playEffect(kAKSelectSEFileName);
        
        // 1増やしたページ番号を設定する
        setPageNo(_pageNo + 1);
    }
}

// タイトルへ戻る
void PageScene::backToTitle()
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
