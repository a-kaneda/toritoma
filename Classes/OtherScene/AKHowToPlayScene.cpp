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

using cocos2d::Sprite;
using cocos2d::SpriteFrameCache;
using cocos2d::Controller;

/// 遊び方説明画像テクスチャアトラス定義ファイル名
static const char *kAKHowToTextureAtlasDefFile = "HowTo.plist";
/// 遊び方説明画像テクスチャアトラスファイル名
static const char *kAKHowToTextureAtlasFile = "HowTo.png";
/// 表示画像ファイル名
static const char *kAKFileNameFormat = "HowTo_%d.png";

/// メッセージボックスの1行の文字数
static const int kAKHowToMsgLength = 20;
/// メッセージボックスの行数
static const int kAKHowToMsgLineCount = 3;
/// メッセージボックスの位置、下からの位置
static const float kAKHowToMsgPosBottomPoint = 85.0f;

/// イメージ表示位置、下からの位置
static const float kAKHowToImagePosBottomPoint = 200.0f;

/// ページ数
static const int kAKHowToPageCount = 6;

// コンビニエンスコンストラクタ
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
 @brief コンストラクタ
 
 初期化処理を行う。
 */
AKHowToPlayScene::AKHowToPlayScene() : PageScene(kAKHowToPageCount)
{
    
}

// 派生クラスの初期化処理
bool AKHowToPlayScene::initSub()
{
    // テクスチャアトラスを読み込む
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(kAKHowToTextureAtlasDefFile, kAKHowToTextureAtlasFile);

    // メッセージボックスを作成する
    _messageLabel = AKLabel::createLabel("", kAKHowToMsgLength, kAKHowToMsgLineCount, kAKLabelFrameMessage, AKLabel::MessageFont);
    
    // 配置位置を設定する
    float x = AKScreenSize::center().x;
    float y = AKScreenSize::positionFromBottomPoint(kAKHowToMsgPosBottomPoint);
    _messageLabel->setPosition(x, y);
    
    // シーンに配置する
    addChild(_messageLabel, ZPositionItem);

    return true;
}

// ページ表示内容更新
unsigned int AKHowToPlayScene::updatePageContents(int pageNo)
{
    // 表示文字列のキーを生成する
    char key[32] = "";
    
    // コントローラが接続されている場合はメッセージを切り替える
    if (Controller::getAllController().size() > 0) {
        snprintf(key, sizeof(key), "HowToPlay_Controller_%d", pageNo);
    }
    else {
        snprintf(key, sizeof(key), "HowToPlay_%d", pageNo);
    }
    
    // 表示文字列を変更する
    _messageLabel->setString(aklib::LocalizedResource::getInstance().getString(key));
    
    // 表示ファイル名を作成する
    char fileName[16] = "";
    snprintf(fileName, sizeof(fileName), kAKFileNameFormat, pageNo);
    
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
    m_image->setPosition(x, y);
    
    // シーンに追加する
    addChild(m_image, ZPositionItem);
    
    return 0;
}
