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
 @file AKFont.h
 @brief フォント管理クラス
 
 フォント情報を管理するクラスを定義する。
 */

#ifndef AKFONT_H
#define AKFONT_H

#include "AKCommon.h"

// フォントサイズ
extern const int kAKFontSize;

/*!
 @brief フォント管理クラス
 
 フォントのテクスチャ情報を管理する。
 */
class AKFont {
public:
    // シングルトンオブジェクトの取得
    static AKFont& getInstance();
    // フォントサイズ取得
    static int getFontSize();
private:
    /// 文字のテクスチャ内の位置情報
    cocos2d::__Dictionary *m_fontMap;
    /// フォントテクスチャ
    cocos2d::Texture2D *m_fontTexture;
public:
    // コンストラクタ
    AKFont();
    // デストラクタ
    ~AKFont();
    // 文字表示用バッチノード作成
    cocos2d::SpriteBatchNode* createBatchNode(int capacity);
    // キーからテクスチャ内の位置を取得する
    cocos2d::Rect getRect(const std::string &key) const;
    // キーからスプライトフレームを作成する
    cocos2d::SpriteFrame* createSpriteFrame(const std::string &key, bool isReverse) const;
    // キーからスプライトを作成する
    cocos2d::Sprite* createSprite(const std::string &key, bool isReverse) const;
};

#endif