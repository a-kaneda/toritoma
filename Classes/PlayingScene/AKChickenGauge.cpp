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
 @file AKChickenGauge.cpp
 @brief チキンゲージクラス定義
 
 チキンゲージのクラスを定義する。
 */

#include "AKChickenGauge.h"

/// 空ゲージの画像名
static const char *kAKEmptyImageName = "ChickenGauge_01.png";
/// 満ゲージの画像名
static const char *kAKFullImageName = "ChickenGauge_02.png";
/// 画像の幅
static const int kAKImageWidth = 256;
/// 画像の高さ
static const int kAKImageHeight = 16;

/*!
 @brief コンビニエンスコンストラクタ

 インスタンスを生成し、初期化処理を行い、autoreleaseを行う。
 @return 生成したインスタンス
 */
AKChickenGauge* AKChickenGauge::create()
{
    AKChickenGauge *instance = new AKChickenGauge();
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
AKChickenGauge::AKChickenGauge() :
m_percent(0.0f)
{
    // 空画像を読み込む
    m_emptyImage = cocos2d::CCSprite::create(kAKEmptyImageName);
    
    // 自ノードに配置する
    addChild(m_emptyImage, 0);
    
    // 満画像を読み込む
    m_fullImage = cocos2d::CCSprite::create(kAKFullImageName);
    
    // 自ノードに配置する
    addChild(m_fullImage, 1);
    
    // ゲージがたまるのを満ゲージの幅変更によって表現する。
    // 幅が変わってもx座標を変えなくても良いように左端にアンカーを設定する。
    m_fullImage->setAnchorPoint(cocos2d::CCPointMake(0.0f, 0.5f));
    m_fullImage->setPosition(cocos2d::CCPointMake(AKScreenSize::deviceLength(-kAKImageWidth / 2.0f), 0.0f));
}

/*!
 @brief デストラクタ
 
 画像とテクスチャを解放する。
 */
AKChickenGauge::~AKChickenGauge()
{
    // 画像を解放する
    m_emptyImage->removeFromParentAndCleanup(true);
    m_fullImage->removeFromParentAndCleanup(true);
    
    // テクスチャを解放する
    cocos2d::CCTextureCache::sharedTextureCache()->removeTextureForKey(kAKEmptyImageName);
    cocos2d::CCTextureCache::sharedTextureCache()->removeTextureForKey(kAKFullImageName);
}

/*!
 @brief ゲージの溜まっている比率設定
 
 ゲージの溜まっている比率を設定する。
 満ゲージの幅を更新する。
 @param parcent ゲージの溜まっている比率
 */
void AKChickenGauge::setPercent(float percent)
{
    // メンバに設定する。
    m_percent = percent;
    
    // 満ゲージの幅を更新する
    cocos2d::CCRect rect = AKScreenSize::deviceRect(0.0f, 0.0f, kAKImageWidth * percent / 100.0f, kAKImageHeight);
    m_fullImage->setTextureRect(rect);
}
