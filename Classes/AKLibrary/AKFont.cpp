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
 @file AKFont.m
 @brief フォント管理クラス
 
 フォント情報を管理するクラスを定義する。
 */

#include "AKFont.h"
#include "AKScreenSize.h"
#include "AKLogNoDef.h"

using cocos2d::__Dictionary;
using cocos2d::SpriteBatchNode;
using cocos2d::Integer;
using cocos2d::Rect;
using cocos2d::SpriteFrame;
using cocos2d::__String;
using cocos2d::Sprite;
using cocos2d::Director;

/// フォントサイズ
const int kAKFontSize = 16;

/// フォント画像のファイル名
static const char *kAKFontImageName = "Font.png";
/// フォント中の文字の位置情報のファイル名
static const char *kAKFontMapName = "Font.plist";
/// 色反転フォントの位置のキー
static const char *kAKReversePosKey = "Reverse";

/*!
 @brief シングルトンオブジェクト取得
 
 シングルトンオブジェクトを返す。初回呼び出し時はオブジェクトを作成して返す。
 @return シングルトンオブジェクト
 */
AKFont& AKFont::getInstance()
{
    // シングルトンオブジェクト
    static AKFont sharedInstance;
        
    // シングルトンオブジェクトを返す。
    return sharedInstance;
}

/*!
 @brief フォントサイズ取得
 
 フォントサイズを取得する。
 iPadの場合は倍のサイズを返す。
 @return フォントサイズ
 */
int AKFont::getFontSize()
{
    return AKScreenSize::deviceLength(kAKFontSize);
}

/*!
 @method コンストラクタ
 
 フォント画像、文字の位置情報を読み込む。
 */
AKFont::AKFont()
{
    // フォント画像を読み込む
    this->m_fontTexture = Director::getInstance()->getTextureCache()->addImage(kAKFontImageName);
    AKAssert(this->m_fontTexture, "フォント画像の読み込みに失敗。:%s", kAKFontImageName);
    this->m_fontTexture->retain();
    
    // 文字の位置情報のplistファイルを読み込む
    this->m_fontMap = __Dictionary::createWithContentsOfFile(kAKFontMapName);
    assert(this->m_fontMap);
    this->m_fontMap->retain();
}

/*!
 @brief デストラクタ
 
 インスタンス解放時にオブジェクトを解放する。
 */
AKFont::~AKFont()
{
    // メンバを解放する
    this->m_fontTexture->release();
    this->m_fontMap->release();
}

/*!
 @brief 文字表示用バッチノード作成
 
 文字表示用のバッチノードをフォントテクスチャから作成する。
 @param capacity 最大文字数
 @return 文字表示用バッチノード
 */
SpriteBatchNode* AKFont::createBatchNode(int capacity)
{
    return SpriteBatchNode::createWithTexture(m_fontTexture, capacity);
}

/*!
 @brief キーからテクスチャ内の位置を取得する
 
 キーからテクスチャ内の位置を取得する。
 @param key キー
 @return テクスチャ内の位置
 */
Rect AKFont::getRect(const std::string &key) const
{
    AKLog(kAKLogFont_1, "key=%s", key.c_str());
    
    // 文字の位置情報を文字全体の情報から検索する
    __Dictionary *charInfo = static_cast<__Dictionary*>(this->m_fontMap->objectForKey(key));
    
    // フォントサイズを取得する
    int fontSize = AKFont::getFontSize();
    
    // 見つからない場合は一番左上のダミー文字を返す
    if (charInfo == NULL) {
        
        AKAssert(false, "フォントが見つからない:%s", key.c_str());
        
        Rect rect(0, 0, fontSize, fontSize);
        return rect;
    }
    
    // 位置情報を取得する
    __String *xString = dynamic_cast<__String*>(charInfo->objectForKey("x"));
    __String *yString = dynamic_cast<__String*>(charInfo->objectForKey("y"));
    
    AKAssert(xString, "x取得に失敗");
    AKAssert(yString, "y取得に失敗");
    
    // オブジェクトから数値に変換する
    int xValue = xString != NULL ? xString->intValue() : 0;
    int yValue = yString != NULL ? yString->intValue() : 0;
    
    // 位置情報から矩形座標を作成する
    Rect rect(xValue * (fontSize + 2) + 1,
              yValue * (fontSize + 2) + 1,
              fontSize,
              fontSize);
    AKLog(kAKLogFont_1, "rect=(%f, %f, %f, %f)",
          rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
    
    return rect;
}

/*!
 @brief キーからスプライトフレームを作成する
 
 キーからスプライトフレームを作成する。
 @param key キー
 @param isReverse 色反転するかどうか
 @return キーのスプライトフレーム
 */
SpriteFrame* AKFont::createSpriteFrame(const std::string &key, bool isReverse) const
{
    // 文字の座標を取得する
    Rect charRect = this->getRect(key);
    
    // 色反転する場合は色反転の座標をプラスする
    if (isReverse) {
        Rect reverseRect = this->getRect(kAKReversePosKey);
        
        charRect.origin.x += reverseRect.origin.x;
        charRect.origin.y += reverseRect.origin.y;
    }

    // フォントのテクスチャからキーに対応する部分を切り出して返す
    return SpriteFrame::createWithTexture(this->m_fontTexture, charRect);
}

/*!
 @brief キーからスプライトを作成する
 
 キーからスプライトを作成する。
 @param key キー
 @param isReverse 色反転するかどうか
 @return キーのスプライト
 */
Sprite* AKFont::createSprite(const std::string &key, bool isReverse) const
{
    // 文字の座標を取得する
    Rect charRect = this->getRect(key);
    
    // 色反転する場合は色反転の座標をプラスする
    if (isReverse) {
        Rect reverseRect = this->getRect(kAKReversePosKey);
        
        charRect.origin.x += reverseRect.origin.x;
        charRect.origin.y += reverseRect.origin.y;
    }
    
    // フォントのテクスチャからキーに対応する部分を切り出して返す
    return Sprite::createWithTexture(this->m_fontTexture, charRect);
}