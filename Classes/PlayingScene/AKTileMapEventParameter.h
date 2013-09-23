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
 @file AKTileMapEventParameter.h
 @brief タイルマップイベントパラメータクラス
 
 タイルマップイベント処理で使用するパラメータ。
 */

#ifndef AKTILEMAPEVENTPARAMETER_H
#define AKTILEMAPEVENTPARAMETER_H

#include "AKPlayDataInterface.h"

/*!
 @brief タイルマップイベントパラメータクラス
 
 タイルマップイベント処理で使用するパラメータ。
 */
class AKTileMapEventParameter {
private:
    /// タイルの座標
    cocos2d::CCPoint m_position;
    /// タイルのプロパティ
    cocos2d::CCDictionary *m_properties;
    
private:
    // デフォルトコンストラクタは使用禁止にする
    AKTileMapEventParameter();
    
public:
    // パラメータを指定したコンストラクタ
    AKTileMapEventParameter(const cocos2d::CCPoint &position, cocos2d::CCDictionary *properties);
    // デストラクタ
    ~AKTileMapEventParameter();
    // 座標取得
    const cocos2d::CCPoint* getPosition() const;
    // プロパティ取得
    cocos2d::CCDictionary* getProperties() const;
};

#endif