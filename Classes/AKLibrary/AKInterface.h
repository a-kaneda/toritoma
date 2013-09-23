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
 @file AKInterface.h
 @brief 画面入力管理クラス
 
 画面のタッチ入力を管理するクラスを定義する。
 */

#ifndef AKINTERFACE_H
#define AKINTERFACE_H

#include "AKCommon.h"
#include "AKMenuItem.h"
#include "AKMenuEventHandler.h"
#include "AKLabel.h"
#include "AKScreenSize.h"

/*!
 @brief 画面入力管理クラス
 
 画面のタッチ入力を管理する。
 */
class AKInterface : public cocos2d::CCLayer {
public:
    // コンビニエンスコンストラクタ
    static AKInterface* create(AKMenuEventHandler * const eventHandler);
    
private:
    /// メニュー項目
    std::vector<AKMenuItem*> m_menuItems;
    /// 有効タグ
    unsigned int m_enableTag;
    /// イベント処理を行うクラス
    AKMenuEventHandler * const m_eventHandler;

private:
    // デフォルトコンストラクタを使用禁止にする
    AKInterface();
    
public:
    // デストラクタ
    ~AKInterface();
    // 有効化タグ設定
    void setEnableTag(unsigned int enableTag);
    // レイヤー表示時処理
    virtual void onEnter();
    // タッチ開始処理
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    // タッチ中断処理
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    // タッチ終了処理
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    // タッチ移動処理
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    // メニュー項目追加
    void addMenuItem(AKMenuItem *menu);
    // スプライトフレームからメニュー項目作成
    cocos2d::CCSprite* addSpriteMenu(const std::string &spriteName, cocos2d::CCPoint position, int z, int event, unsigned int tag, enum AKMenuType type);
    // 文字列からメニュー項目作成
    AKLabel* addLabelMenu(const std::string menuString, cocos2d::CCPoint position, int z, int event, unsigned int tag, bool withFrame);
    // スライド入力作成
    void addSlideMenu(cocos2d::CCRect rect, int event, unsigned int tag);
    
protected:
    // イベントハンドラを指定したコンストラクタ
    AKInterface(AKMenuEventHandler * const eventHandler);
    
private:
    // メニュー項目表示非表示設定
    void updateVisible();
    // メニュー項目個別表示設定
    virtual void updateVisible(cocos2d::CCNode *item);
};

#endif
