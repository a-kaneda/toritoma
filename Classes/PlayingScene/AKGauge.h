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
 @file AKGauge.h
 @brief ゲージクラス定義
 
 ゲージのクラスを定義する。
 */

#ifndef __toritoma__AKGauge__
#define __toritoma__AKGauge__

#include "AKToritoma.h"

/*!
 @brifef ゲージクラス
 
 ゲージの表示を行うクラス。
 */
class AKGauge : public cocos2d::Node {
public:
    // コンビニエンスコンストラクタ
    static AKGauge* create(const char *emptyFileName,
                           const char *fullFileName,
                           int width,
                           int height,
                           bool isHorizontal);
    
private:
    /// 空ゲージの画像ファイル名
    std::string m_emptyFileName;
    /// 満ゲージの画像ファイル名
    std::string m_fullFileName;
    /// 空ゲージの画像
    cocos2d::Sprite *m_emptyImage;
    /// 満ゲージの画像
    cocos2d::Sprite *m_fullImage;
    /// 画像の幅
    int m_imageWidth;
    /// 画像の高さ
    int m_imageHeight;
    /// ゲージの溜まっている比率
    float m_percent;
    /// 水平方向かどうか
    bool m_isHorizontal;
    // デフォルトコンストラクタは使用不可とする
    AKGauge();
    
public:
    // コンストラクタ
    AKGauge(const char *emptyFileName,
            const char *fullFileName,
            int width,
            int height,
            bool isHorizontal);
    // デストラクタ
    ~AKGauge();
    // ゲージの溜まっている比率取得
    float getPercent() const;
    // ゲージの溜まっている比率設定
    void setPercent(float percent);
    // 表示非表示切替
    void setVisible(bool visible);
};

#endif /* defined(__toritoma__AKGauge__) */