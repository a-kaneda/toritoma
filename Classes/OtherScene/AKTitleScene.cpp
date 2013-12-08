/*
 * Copyright (c) 2012-2013 Akihiro Kaneda.
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
 @file AKTitleScene.cpp
 @brief タイトルシーンクラスの定義
 
 タイトルシーンを管理するクラスを定義する。
 */

#include "AKTitleScene.h"
#include "AKPlayingScene.h"
#include "AKHowToPlayScene.h"
#include "AKOptionScene.h"

using cocos2d::CCPoint;
using cocos2d::CCTransitionFade;
using cocos2d::CCDirector;
using cocos2d::CCBlink;
using cocos2d::CCNode;
using CocosDenshion::SimpleAudioEngine;

// メニュー項目のタグ
enum {
    kAKTitleMenuGame = 1,   ///< ゲーム開始ボタン
    kAKTitleMenuHowTo,      ///< 遊び方ボタン
    kAKTitleMenuOption,     ///< オプションボタン
    kAKTitleMenuCredit      ///< クレジットボタン
};

// シーンに配置するレイヤーのタグ
enum {
    kAKTitleBackground = 0,     ///< 背景レイヤー
    kAKTitleInterface           ///< インターフェースレイヤー
};

// メニューイベント番号
enum {
    kAKEventTouchGameStartButton = 0,   ///< ゲーム開始ボタン
    kAKEventTouchHowToButton,           ///< 遊び方ボタン
    kAKEventTouchOptionButton,          ///< オプションボタン
    kAKEventTouchCreditBUtton           ///< クレジットボタン
};

// TODO:タイトル画像のファイル名
//static const char *kAKTitleImage = @"Title.png";

/// ゲーム開始メニューのキャプション
static const char *kAKGameStartCaption  = "GAME START ";
/// 遊び方画面メニューのキャプション
static const char *kAKHowToPlayCaption  = "HOW TO PLAY";
/// オプション画面メニューのキャプション
static const char *kAKOptionCaption     = "OPTION     ";
/// クレジット画面メニューのキャプション
static const char *kAKCreditCaption     = "CREDIT     ";

/// タイトルの位置、横方向の中心からの位置
static const float kAKTitlePosFromHorizontalCenterPoint = -100.0f;
/// タイトルの位置、上からの比率
static const float kAKTitlePosFromTopRatio = 0.45f;
/// メニュー項目の数
static const int kAKMenuItemCount = 3;
/// メニュー項目の位置、右からの位置
static const float kAKMenuPosRightPoint = 120.0f;
/// ゲーム開始メニューのキャプションの表示位置、上からの比率
static const float kAKGameStartMenuPosTopRatio = 0.25f;
/// 遊び方画面メニューのキャプションの表示位置、上からの比率
static const float kAKHowToPlayMenuPosTopRatio = 0.45f;
/// オプション画面メニューのキャプションの表示位置、上からの位置
static const float kAKOptionMenuPosTopRatio = 0.65f;
/// クレジット画面メニューのキャプションの表示位置、上からの比率
static const float kAKCreditMenuPosTopRatio = 0.85f;

/// 各ノードのz座標
enum {
    kAKTitleBackPosZ = 0,   ///< 背景のz座標
    kAKTitleLogoPosZ,       ///< タイトルロゴのz座標
    kAKTitleMenuPosZ        ///< メニュー項目のz座標
};

/*!
 @brief コンビニエンスコンストラクタ

 インスタンスを生成し、初期化処理を行い、autoreleaseを行う。
 @return 生成したインスタンス
 */
AKTitleScene* AKTitleScene::create()
{
    AKTitleScene *instance = new AKTitleScene();
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
AKTitleScene::~AKTitleScene()
{
    // メンバを解放する
    m_interface->release();
}

/*!
 @brief イベント実行
 
 イベントを実行する。
 @param item 選択されたメニュー項目
 */
void AKTitleScene::execEvent(const AKMenuItem *item)
{
    // 選択された項目に応じて処理を行う
    switch (item->getEventNo()) {
        case kAKEventTouchGameStartButton:  // ゲーム開始ボタン
            touchGameStartButton();
            break;

        case kAKEventTouchHowToButton:      // 遊び方ボタン
            touchHowToButton();
            break;

        case kAKEventTouchOptionButton:     // オプションボタン
            touchOptionButton();
            break;

        case kAKEventTouchCreditBUtton:     // クレジットボタン
            touchCreditButton();
            break;

        default:
            AKAssert(false, "不正なイベント番号:%d", item->getEventNo());
            break;
    }
}

/*!
 @brief コンストラクタ

 シーンに配置する項目を作成する。
 */
AKTitleScene::AKTitleScene()
{
    AKLog(kAKLogTitleScene_1, "start");
    
    // 背景レイヤーを配置する
    addChild(AKCreateBackColorLayer(), kAKTitleBackground, kAKTitleBackground);
    
    // インターフェースを作成する
    m_interface = AKInterface::create(this);
    m_interface->retain();
    
    // インターフェースをシーンに配置する
    addChild(m_interface, kAKTitleInterface, kAKTitleInterface);

    // TODO:タイトル画像を読み込む
//    CCSprite *image = [CCSprite spriteWithFile:kAKTitleImage];
//    NSAssert(image != nil, @"can not open title image : %@", kAKTitleImage);
    
    // TODO:配置位置を設定する
//    image.position = ccp([AKScreenSize positionFromHorizontalCenterPoint:kAKTitlePosFromHorizontalCenterPoint],
//                         [AKScreenSize positionFromTopRatio:kAKTitlePosFromTopRatio]);
    
    // TODO:タイトル画像をシーンに配置する
//    [self addChild:image z:kAKTitleLogoPosZ];
    
    // ゲームスタートのメニューの位置を決める
    float x = AKScreenSize::positionFromRightPoint(kAKMenuPosRightPoint);
    float y = AKScreenSize::positionFromTopRatio(kAKGameStartMenuPosTopRatio);
    AKLog(kAKLogTitleScene_1, "ゲームスタートのメニュー位置:x=%f, y=%f", x, y);

    // ゲームスタートのメニューを作成する
    m_interface->addLabelMenu(kAKGameStartCaption,
                              CCPoint(x, y),
                              0,
                              kAKEventTouchGameStartButton,
                              kAKTitleMenuGame,
                              true);
    
    // 遊び方のメニューの位置を決める
    x = AKScreenSize::positionFromRightPoint(kAKMenuPosRightPoint);
    y = AKScreenSize::positionFromTopRatio(kAKHowToPlayMenuPosTopRatio);
    AKLog(kAKLogTitleScene_1, "遊び方のメニュー位置:x=%f, y=%f", x, y);

    // 遊び方のメニューを作成する
    m_interface->addLabelMenu(kAKHowToPlayCaption,
                              CCPoint(x, y),
                              0,
                              kAKEventTouchHowToButton,
                              kAKTitleMenuHowTo,
                              true);
    
    // オプションのメニューの位置を決める
    x = AKScreenSize::positionFromRightPoint(kAKMenuPosRightPoint);
    y = AKScreenSize::positionFromTopRatio(kAKOptionMenuPosTopRatio);
    AKLog(kAKLogTitleScene_1, "オプションのメニュー位置:x=%f, y=%f", x, y);

    // オプションのメニューを作成する
    m_interface->addLabelMenu(kAKOptionCaption,
                              CCPoint(x, y),
                              0,
                              kAKEventTouchOptionButton,
                              kAKTitleMenuOption,
                              true);
    
    // クレジットのメニューの位置を決める
    x = AKScreenSize::positionFromRightPoint(kAKMenuPosRightPoint);
    y = AKScreenSize::positionFromTopRatio(kAKCreditMenuPosTopRatio);
    AKLog(kAKLogTitleScene_1, "クレジットのメニュー位置:x=%f, y=%f", x, y);

    // クレジットのメニューを作成する
    m_interface->addLabelMenu(kAKCreditCaption,
                              CCPoint(x, y),
                              0,
                              kAKEventTouchCreditBUtton,
                              kAKTitleMenuCredit,
                              true);
    
    // すべてのメニュー項目を有効とする
    m_interface->setEnableTag(0xFFFFFFFFUL);
    
    AKLog(kAKLogTitleScene_1, "end");
}

/*!
 @brief ゲーム開始ボタンタッチ
 
 ゲームを開始する。プレイシーンへと遷移する。
 */
void AKTitleScene::touchGameStartButton()
{
    AKLog(kAKLogTitleScene_1, "start");
    
    // ボタン選択エフェクトを発生させる
    selectButton(kAKTitleMenuGame);
    
    // ゲームシーンへの遷移を作成する
    CCTransitionFade *transition = CCTransitionFade::create(0.5f, AKPlayingScene::create());
    
    // ゲームシーンへ遷移する
    CCDirector::sharedDirector()->replaceScene(transition);
}

/*!
 @brief 遊び方ボタンタッチ
 
 遊び方画面へ遷移する。
 */
void AKTitleScene::touchHowToButton()
{
    AKLog(kAKLogTitleScene_1, "start");
    
    // ボタン選択エフェクトを発生させる
    selectButton(kAKTitleMenuHowTo);
    
    // 遊び方シーンへの遷移を作成する
    CCTransitionFade *transition = CCTransitionFade::create(0.5f, AKHowToPlayScene::create());
    
    // 遊び方シーンへ遷移する
    CCDirector::sharedDirector()->replaceScene(transition);
}

/*!
 @brief オプションボタンタッチ
 
 オプション画面へ遷移する。
 */
void AKTitleScene::touchOptionButton()
{
    AKLog(kAKLogTitleScene_1, "start");

    // ボタン選択エフェクトを発生させる
    selectButton(kAKTitleMenuOption);
    
    // オプションシーンへの遷移を作成する
    CCTransitionFade *transition = CCTransitionFade::create(0.5f, AKOptionScene::create());
    
    // オプションシーンへ遷移する
    CCDirector::sharedDirector()->replaceScene(transition);
}

/*!
 @brief クレジット画面の開始
 
 クレジット画面を開始する。クレジットシーンへと遷移する。
 */
void AKTitleScene::touchCreditButton()
{
    AKLog(kAKLogTitleScene_1, "start");

    // ボタン選択エフェクトを発生させる
    selectButton(kAKTitleMenuCredit);
    
    // TODO:クレジット画面シーンへの遷移を作成する
//    CCTransitionFade *transition = CCTransitionFade::create(0.5f, AKCreditScene::create());
    
    // TODO:クレジット画面シーンへ遷移する
//    CCDirector::sharedDirector()->replaceScene(transition);
}

/*!
 @brief ボタン選択エフェクト
 
 ボタン選択時のエフェクトを表示する。
 効果音を鳴らし、ボタンをブリンクする。
 @param tag ボタンのタグ
 */
void AKTitleScene::selectButton(int tag)
{
    // メニュー選択時の効果音を鳴らす
    SimpleAudioEngine::sharedEngine()->playEffect(kAKSelectSEFileName);
    
    // ボタンのブリンクアクションを作成する
    CCBlink *action = CCBlink::create(0.2f, 2);
    
    // ボタンを取得する
    CCNode *button = m_interface->getChildByTag(tag);
    
    // ブリンクアクションを開始する
    button->runAction(action);
}
