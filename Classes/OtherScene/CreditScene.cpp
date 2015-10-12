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

/// カーソル画像の位置のボタンとの重なりの幅
const float CreditScene::CursorPosOverlap = 8.0f;
/// リンクの個数
const int CreditScene::LinkNum = 6;
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
const float CreditScene::LinkPosTopRatio[] = {0.30f, 0.55f, 0.8f};
/// リンクボタンのタグ
const unsigned int CreditScene::LinkTag[] = {0x04, 0x08, 0x10};

// コンビニエンスコンストラクタ
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

// コンストラクタ
CreditScene::CreditScene() : PageScene(MaxPageNum)
{
    
}

// 派生クラスの初期化処理
bool CreditScene::initSub()
{
    // リンクボタンをインターフェースに配置する
    for (int i = 0; i < LinkNumOfPage; i++) {
        
        float x = AKScreenSize::positionFromLeftRatio(LinkPosLeftRatio);
        float y = AKScreenSize::positionFromTopRatio(LinkPosTopRatio[i]);
        m_link[i] = getInterface()->addLabelMenu(LinkCaption,
                                                 Vec2(x, y),
                                                 0,
                                                 MenuLink1 + i,
                                                 LinkTag[i],
                                                 true);
    }
    
    // クレジットラベルをシーンに配置する
    for (int i = 0; i < LinkNumOfPage; i++) {
        
        m_creditLabel[i] = AKLabel::createLabel("", CreditLineLength, CreditLineNum, kAKLabelFrameNone, AKLabel::MessageFont);
        
        float x = AKScreenSize::positionFromLeftRatio(CreditPosLeftRatio);
        float y = AKScreenSize::positionFromTopRatio(LinkPosTopRatio[i]);
        m_creditLabel[i]->setPosition(Vec2(x, y));
        
        addChild(m_creditLabel[i], ZPositionBack);
    }

    return true;
}

// 派生クラスのイベント処理
void CreditScene::execSubEvent(int pageNo, const AKMenuItem *item)
{
    // 選択された項目に応じて処理を行う
    switch (item->getEventNo()) {
        case MenuLink1:         // リンクボタン1
            openLink(pageNo, 1);
            break;
            
        case MenuLink2:         // リンクボタン2
            openLink(pageNo, 2);
            break;
            
        case MenuLink3:         // リンクボタン3
            openLink(pageNo, 3);
            break;
            
        default:
            AKAssert(false, "不正なイベント番号:%d", item->getEventNo());
            break;
    }
}

// コントローラのAボタンを押した時の処理
void CreditScene::onKeyDownAButton(int pageNo, int cursorPosition)
{
    // リンクを開く
    openLink(pageNo, cursorPosition + 1);
}

// ページ表示内容更新
unsigned int CreditScene::updatePageContents(int pageNo)
{
    unsigned int enableTag = 0;
    
    // リンクボタンの表示を更新する
    enableTag = updateLinkButton(pageNo);

    // クレジットラベルを更新する
    updateCreditLabel(pageNo);
    
    return enableTag;
}

// カーソル表示有無取得
bool CreditScene::isVisibleCursor(int pageNo)
{
    return true;
}

// カーソル位置取得
Vec2 CreditScene::getCursorPosition(int pageNo, int positionID)
{
    AKAssert(positionID >= 0 && positionID <= LinkNumOfPage, "positionID is out of range.");
    
    // x座標を決める
    float x = m_link[positionID]->getPosition().x - m_link[positionID]->getWidth() / 2 + getCursorPositionMargin();
    
    // 選択項目に応じてカーソルのy座標を決定する
    float y = m_link[positionID]->getPosition().y;
    
    return Vec2(x, y);
}

// コントローラのLスティックを上に倒した時の処理
int CreditScene::onLStickUp(int pageNo, int cursorPosition)
{
    // カーソル移動時の効果音を鳴らす
    SimpleAudioEngine::getInstance()->playEffect(kAKCursorSEFileName);
    
    // 選択位置をひとつ減らす
    // 下限を超える場合は末尾へ移動する
    if (cursorPosition - 1 < 0) {
        return getLinkNum(pageNo) - 1;
    }
    else {
        return cursorPosition - 1;
    }
}

// コントローラのLスティックを下に倒した時の処理
int CreditScene::onLStickDown(int pageNo, int cursorPosition)
{
    // カーソル移動時の効果音を鳴らす
    SimpleAudioEngine::getInstance()->playEffect(kAKCursorSEFileName);
    
    // 選択位置をひとつ増やす
    // 上限を超える場合は先頭へ移動する
    if (cursorPosition + 1 >= getLinkNum(pageNo)) {
        return 0;
    }
    else {
        return cursorPosition + 1;
    }
}

// リンクボタン押下
void CreditScene::openLink(int pageNo, int linkNumber)
{
    // メニュー選択時の効果音を鳴らす
    SimpleAudioEngine::getInstance()->playEffect(kAKSelectSEFileName);
    
    // URL文字列のキーを生成する
    char key[16] = "";
    snprintf(key, sizeof(key), UrlTextKey, linkNumber + (pageNo - 1) * LinkNumOfPage);
    
    // URLを開く
    OpenUrl::open(aklib::LocalizedResource::getInstance().getString(key).c_str());
}

// リンクボタン表示更新
unsigned int CreditScene::updateLinkButton(int pageNo)
{
    unsigned int enableTag = 0;
    
    // 最後のページ以外の場合は全リンクボタンを有効にする
    // 全リンクボタンが1ページあたりのリンクボタンの個数で割り切れる場合は常に全リンクボタンを有効にする
    if (pageNo < MaxPageNum || (LinkNum % LinkNumOfPage == 0)) {
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
    
    return enableTag;
}

// クレジットラベル表示更新
void CreditScene::updateCreditLabel(int pageNo)
{
    // 1ページの各項目のラベルを更新する
    for (int i = 0; i < LinkNumOfPage; i++) {

        // 表示個数の範囲内ならば、リソースからテキストを取得してラベルに設定する
        if (i + (pageNo - 1) * LinkNumOfPage + 1 <= LinkNum) {
            
            // 表示文字列のキーを生成する
            char key[16] = "";
            snprintf(key, sizeof(key), CreditTextKey, i + (pageNo - 1) * LinkNumOfPage + 1);
            
            // 表示文字列を変更する
            m_creditLabel[i]->setString(aklib::LocalizedResource::getInstance().getString(key));
        }
        else {
            
            // 表示文字列を変更する
            m_creditLabel[i]->setString("");
        }
    }
}

// ページ内項目数計算
int CreditScene::getLinkNum(int pageNo)
{
    return MIN(LinkNum - (pageNo - 1) * LinkNumOfPage, LinkNumOfPage);
}

