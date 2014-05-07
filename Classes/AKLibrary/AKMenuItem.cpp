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
 @file AKMenuItem.m
 @brief メニュー項目クラス
 
 画面入力管理クラスに登録するメニュー項目クラスを定義する。
 */

#include "AKMenuItem.h"
#include "AKLogNoDef.h"

using cocos2d::Rect;
using cocos2d::Vector2;
using cocos2d::Touch;

/*!
 @brief 矩形指定のコンストラクタ
 
 矩形を指定してメニュー項目の生成を行う。
 @param rect 位置と大きさ
 @param type メニュー種別
 @param eventNo イベント番号
 @param tag タグ情報(任意に使用)
 */
AKMenuItem::AKMenuItem(const Rect &rect,
                       enum AKMenuType type,
                       int eventNo,
                       unsigned int tag) :
m_rect(rect), m_type(type), m_eventNo(eventNo), m_tag(tag),
m_prevPoint(0.0f, 0.0f), m_touch(NULL)
{
}

/*!
 @brief 座標指定のメニュー項目生成
 
 中心座標と矩形の1辺のサイズを指定してメニュー項目の生成を行う。
 @param point 中心座標
 @param size 矩形の1辺のサイズ
 @param type メニュー種別
 @param eventNo イベント番号
 @param tag タグ情報(任意に使用)
 @return 生成したオブジェクト。失敗時はnilを返す。
 */
AKMenuItem::AKMenuItem(const Vector2 &position,
                       int size,
                       enum AKMenuType type,
                       int eventNo,
                       unsigned int tag) :
m_rect(AKMakeRectFromCenter(position, size)), m_type(type), m_eventNo(eventNo),
m_tag(tag), m_prevPoint(0.0f, 0.0f), m_touch(NULL)
{
}

/*!
 @brief 項目選択判定
 
 座標がメニュー項目の範囲内かどうかを判定する。
 @param pos 選択位置
 @return メニュー項目の範囲内かどうかを
 */
bool AKMenuItem::isSelect(const Vector2 &position) const
{
    // 座標がメニュー項目の範囲内の場合は処理を行う
    return AKIsInside(position, m_rect);
}

/*!
 @brief イベント番号取得
 
 イベント番号を取得する。
 @return イベント番号
 */
int AKMenuItem::getEventNo() const
{
    return m_eventNo;
}

/*!
 @brief タグ取得
 
 タグを取得する。
 @return タグ
 */
unsigned int AKMenuItem::getTag() const
{
    return m_tag;
}

/*!
 @brief 種別取得
 */
enum AKMenuType AKMenuItem::getType() const
{
    return m_type;
}

/*!
 @brief タッチ情報取得
 
 タッチ情報を取得する。
 @return タッチ情報
 */
Touch *AKMenuItem::getTouch() const
{
    return m_touch;
}

/*!
 @brief タッチ情報設定
 
 タッチ情報を設定する。
 @param touch タッチ情報
 */
void AKMenuItem::setTouch(Touch *touch)
{
    m_touch = touch;
}

/*!
 @brief 前回タッチ位置取得
 
 前回のタッチ位置を取得する。
 @return 前回タッチ位置
 */
const Vector2* AKMenuItem::getPrevPoint() const
{
    return &m_prevPoint;
}

/*! 
 @brief 前回タッチ位置設定
 
 前回のタッチ位置を設定する。
 @param 前回タッチ位置
 */
void AKMenuItem::setPrevPoint(Vector2 prevPoint)
{
    m_prevPoint = prevPoint;
}

/*!
 @brief タッチ状態取得
 
 タッチ状態を取得する。
 @return タッチ状態
 */
enum AKMenuTouchPhase AKMenuItem::getTouchPhase() const
{
    return m_touchPhase;
}

/*!
 @brief タッチ状態設定
 
 タッチ状態を設定する。
 @param touchPhase タッチ状態
 */
void AKMenuItem::setTouchPhase(enum AKMenuTouchPhase touchPhase)
{
    m_touchPhase = touchPhase;
}
