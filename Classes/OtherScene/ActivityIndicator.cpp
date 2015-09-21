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
 @file ActivityIndicator.cpp
 @brief 通信中インジケータ
 
 通信中を表すインジケータ。
 */

#include "ActivityIndicator.h"

using cocos2d::Node;
using cocos2d::Sprite;

// インジケータ画像ファイル名
const char *ActivityIndicator::IndicatorFileName = "Indicator.png";
/// インジケータの回転速度
const float ActivityIndicator::RotateSpeed = 360.0f / 12.0f;

// コンビニエンスコンストラクタ
ActivityIndicator* ActivityIndicator::create()
{
    ActivityIndicator *instance = new ActivityIndicator();
    if (instance->init()) {
        instance->autorelease();
        return instance;
    }
    else {
        CC_SAFE_DELETE(instance);
        return NULL;
    }
}

// 初期化処理
bool ActivityIndicator::init()
{
    // スーパークラスの初期化処理を行う
    if (!Node::init()) {
        return false;
    }
    
    // インジケータ画像を読み込む
    _indicator = Sprite::create(IndicatorFileName);
    assert(_indicator != NULL);
    
    // ノードに追加する
    addChild(_indicator);
    
    // インジケータ回転アニメーションのため、スケジュール登録する
    schedule(CC_SCHEDULE_SELECTOR(ActivityIndicator::update), 0.1f);
    
    return true;
}

// 更新処理
void ActivityIndicator::update(float delta)
{
    // インジケータ画像を回転させる
    _indicator->setRotation(_indicator->getRotation() + RotateSpeed);
}