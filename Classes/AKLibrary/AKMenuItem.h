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
 @file AKMenuItem.h
 @brief メニュー項目クラス
 
 画面入力管理クラスに登録するメニュー項目クラスを定義する。
 */

#ifndef AKMENUITEM_H
#define AKMENUITEM_H

#include "AKCommon.h"

/// メニューの種別
enum AKMenuType {
    kAKMenuTypeButton = 0,  ///< ボタン
    kAKMenuTypeMomentary,   ///< モーメンタリボタン
    kAKMenuTypeSlide        ///< スライド入力
};

/// タッチの状態
enum AKMenuTouchPhase {
    kAKMenuTouchNone = 0,   ///< タッチなし
    kAKMenuTouchBegan,      ///< タッチ開始
    kAKMenuTouchMove,       ///< タッチ移動
    kAKMenuTouchCancelled,  ///< タッチ取り消し
    kAKMenuTouchEnded       ///< タッチ終了
};

/*!
 @brief メニュー項目クラス
 
 画面入力管理クラスに登録するメニュー項目。
 メニューの位置、大きさ、処理を管理する。
 */
class AKMenuItem {
private:
    /// 種別
    const enum AKMenuType m_type;
    /// 位置
    const cocos2d::CCRect m_rect;
    /// イベント番号
    const int m_eventNo;
    /// タグ
    const unsigned int m_tag;
    /// 前回タッチ位置(スライド入力時に使用)
    cocos2d::CCPoint m_prevPoint;
    /// タッチ情報
    cocos2d::CCTouch *m_touch;
    /// タッチ状態
    enum AKMenuTouchPhase m_touchPhase;
    
private:
    // デフォルトコンストラクタは使用禁止にする
    AKMenuItem();
    
public:
    // 矩形指定のコンストラクタ
    AKMenuItem(const cocos2d::CCRect &rect,
               enum AKMenuType type,
               int eventNo,
               unsigned int tag);
    // 座標指定のコンストラクタ
    AKMenuItem(const cocos2d::CCPoint &position,
               int size,
               enum AKMenuType type,
               int eventNo,
               unsigned int tag);
    // 項目選択判定
    bool isSelect(const cocos2d::CCPoint &position) const;
    // イベント番号取得
    int getEventNo() const;
    // タグ取得
    unsigned int getTag() const;
    // 種別取得
    enum AKMenuType getType() const;
    // タッチ情報取得
    cocos2d::CCTouch *getTouch() const;
    // タッチ情報設定
    void setTouch(cocos2d::CCTouch *touch);
    // 前回タッチ位置取得
    const cocos2d::CCPoint* getPrevPoint() const;
    // 前回タッチ位置設定
    void setPrevPoint(cocos2d::CCPoint prevPoint);
    // タッチ状態取得
    enum AKMenuTouchPhase getTouchPhase() const;
    // タッチ状態設定
    void setTouchPhase(enum AKMenuTouchPhase touchPhase);
};

#endif
