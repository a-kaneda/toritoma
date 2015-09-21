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

#ifndef ActivityIndicator_h
#define ActivityIndicator_h

#include "cocos2d.h"

class ActivityIndicator : public cocos2d::Node
{
public:
    
    /*!
     @brief コンビニエンスコンストラクタ
     
     インスタンスを生成し、初期化処理を行い、autoreleaseを行う。
     @return 生成したインスタンス
     */
    static ActivityIndicator* create();

    /*!
     @brief 初期化処理
     
     インジケータ画像を読み込む。
     */
    virtual bool init();

    /*!
     @brief 更新処理
     
     インジケータの画像を回転させる。
     @param 更新間隔時間
     */
    virtual void update(float delta);
    
private:
    
    /// インジケータ画像ファイル名
    static const char *IndicatorFileName;
    /// インジケータの回転速度
    static const float RotateSpeed;
    
    /// インジケータ画像
    cocos2d::Sprite *_indicator;
};

#endif /* ActivityIndicator_h */
