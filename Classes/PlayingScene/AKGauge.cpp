/*
 * Copyright (c) 2014 Akihiro Kaneda.
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
 @file AKGauge.cpp
 @brief ゲージクラス定義
 
 ゲージのクラスを定義する。
 */

#include "AKGauge.h"

using cocos2d::Vector2;
using cocos2d::Director;
using cocos2d::TextureCache;
using cocos2d::Sprite;
using cocos2d::Rect;

/*!
 @brief コンビニエンスコンストラクタ
 
 インスタンスを生成し、初期化処理を行い、autoreleaseを行う。
 @param emptyFileName 空ゲージの画像ファイル名
 @param fullFileName 満ゲージの画像ファイル名
 @param width 画像の幅
 @param height 画像の高さ
 @param isHorizontal 水平方向かどうか
 @return 生成したインスタンス
 */
AKGauge* AKGauge::create(const char *emptyFileName,
                         const char *fullFileName,
                         int width,
                         int height,
                         bool isHorizontal)
{
    AKGauge *instance = new AKGauge(emptyFileName,
                                    fullFileName,
                                    width,
                                    height,
                                    isHorizontal);
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
 
 画像の読み込みを行う。
 */
AKGauge::AKGauge(const char *emptyFileName,
                 const char *fullFileName,
                 int width,
                 int height,
                 bool isHorizontal) :
m_emptyFileName(emptyFileName),
m_fullFileName(fullFileName),
m_imageWidth(width),
m_imageHeight(height),
m_isHorizontal(isHorizontal),
m_percent(0.0f)
{
    // 空画像を読み込む
    m_emptyImage = Sprite::create(m_emptyFileName);
    
    // 自ノードに配置する
    addChild(m_emptyImage, 0);
    
    // 満画像を読み込む
    m_fullImage = Sprite::create(m_fullFileName);
    
    // 自ノードに配置する
    addChild(m_fullImage, 1);
    
    // 水平方向かどうかで画像のアンカーを置く場所を変更する
    if (m_isHorizontal) {
        
        // 水平方向の場合、ゲージがたまるのを満ゲージの幅変更によって表現する。
        // 幅が変わってもx座標を変えなくても良いように左端にアンカーを設定する。
        m_fullImage->setAnchorPoint(Vector2(0.0f, 0.5f));
        m_fullImage->setPosition(Vector2(AKScreenSize::deviceLength(-m_imageWidth / 2.0f), 0.0f));
    }
    else {
        
        // 垂直方向の場合、ゲージがたまるのを満ゲージの高さ変更によって表現する。
        // 幅が変わってもx座標を変えなくても良いように下端にアンカーを設定する。
        m_fullImage->setAnchorPoint(Vector2(0.5f, 0.0f));
        m_fullImage->setPosition(Vector2(0.0f, AKScreenSize::deviceLength(-m_imageHeight / 2.0f)));
    }
}

/*!
 @brief デストラクタ
 
 画像とテクスチャを解放する。
 */
AKGauge::~AKGauge()
{
    // 画像を解放する
    m_emptyImage->removeFromParentAndCleanup(true);
    m_fullImage->removeFromParentAndCleanup(true);
    
    // テクスチャを解放する
    TextureCache *cache = Director::getInstance()->getTextureCache();
    cache->removeTextureForKey(m_emptyFileName);
    cache->removeTextureForKey(m_fullFileName);
}

/*!
 @brief ゲージの溜まっている比率設定
 
 ゲージの溜まっている比率を設定する。
 満ゲージの幅を更新する。
 @param parcent ゲージの溜まっている比率
 */
void AKGauge::setPercent(float percent)
{
    // メンバに設定する。
    m_percent = percent;
    
    // 水平方向の場合、満ゲージの幅を更新する。
    // 垂直方向の場合、満ゲージの高さを更新する。
    Rect rect;
    if (m_isHorizontal) {
        rect = AKScreenSize::deviceRect(0.0f,
                                        0.0f,
                                        m_imageWidth * percent / 100.0f,
                                        m_imageHeight);
    }
    else {
        rect = AKScreenSize::deviceRect(0.0f,
                                        m_imageHeight * (100.0f - percent) / 100.0f,
                                        m_imageWidth,
                                        m_imageHeight * percent / 100.0f);
        AKLog(kAKLogGauge_1, "percent=%f y=%f h=%f", percent, rect.origin.y, rect.size.height);
    }
    m_fullImage->setTextureRect(rect);
}

/*!
 @brief 表示非表示切替
 
 表示非表示を切り替える。
 @param 表示非表示
 */
void AKGauge::setVisible(bool visible)
{
    m_emptyImage->setVisible(visible);
    m_fullImage->setVisible(visible);
}