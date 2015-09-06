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
 @file AKLife.cpp
 @brief 残機表示クラス定義
 
 残機表示のクラスを定義する。
 */

#include "AKLife.h"

using cocos2d::Vec2;

/// 残機マークの画像名
static const char *kAKLifeMarkImageName = "Life.png";
/// 残機数のラベル書式
static const char *kAKLifeNumberFormat = ":%02d";
/// 残機数表示の最大値
static const int kAKLifeCountViewMax = 99;
/// 残機マークのサイズ
static const int kAKLifeMarkSize = 16;

/*!
 @brief コンビニエンスコンストラクタ
 
 インスタンスを生成し、初期化処理を行い、autoreleaseを行う。
 @return 生成したインスタンス
 */
AKLife* AKLife::create()
{
    AKLife *instance = new AKLife();
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
 
 残機マークの読み込みと残機ラベルの作成を行う。
 @return 初期化したオブジェクト。失敗時はnilを返す。
 */
AKLife::AKLife() :
m_lifeCount(0), m_numberLabel(NULL), m_mark(NULL)
{
    AKLog(kAKLogLife_1, "start");
    
    // 残機マークを読み込む
    m_mark = cocos2d::CCSprite::createWithSpriteFrameName(kAKLifeMarkImageName);
    
    // 残機数の設定と残機ラベルの作成を行う
    setLifeCount(0);
    
    // 残機マークを左側に配置し、残機数ラベルを右側に配置する。
    // 残機マークの幅をW1、残機数ラベルの幅をW2とすると、全体の幅をWとすると。
    // 残機マークの移動量X1は初めに全体の左端に移動するように左にW/2移動し、
    // 残機マークの左端が全体の左端と一致するように右にW1/2移動する。
    //   X1 = -W / 2 + W1 / 2
    //      = -(W1 + W2) / 2 + W1 / 2
    //      = -W2 / 2
    // 同様に残機数ラベルの移動量X2は以下のようになる。
    //   X2 = W1 / 2
    
    // 残機マークの座標を設定する
    m_mark->setPosition(Vec2(-m_numberLabel->getWidth() / 2, 0.0f));
    
    // 残機数ラベルの座標を設定する
    m_numberLabel->setPosition(Vec2(AKScreenSize::deviceLength(kAKLifeMarkSize) / 2, 0.0f));
    
    // 自ノードに残機マークを配置する
    addChild(m_mark);
    
    // 自ノードに残機数ラベルを配置する
    addChild(m_numberLabel);
    
    AKLog(kAKLogLife_1, "end");
}

/*!
 @brief オブジェクト解放処理
 
 メンバの解放を行う。
 */
AKLife::~AKLife()
{
    AKLog(kAKLogLife_1, "start");
    
    // メンバを解放する
    m_mark->removeAllChildrenWithCleanup(true);
    m_numberLabel->removeAllChildrenWithCleanup(true);

    AKLog(kAKLogLife_1, "end");
}

/*!
 @brief 残機数設定
 
 残機数を設定する。
 @param lifeCount 残機数
 */
void AKLife::setLifeCount(int lifeCount)
{
    AKLog(kAKLogLife_1, "start:lifeCount=%d", lifeCount);
    
    // メンバに設定する
    m_lifeCount = lifeCount;

    // 残機が範囲外の場合は補正する
    int lifeCountView;
    if (lifeCount < 0) {
        lifeCountView = 0;
    }
    else if (lifeCount > kAKLifeCountViewMax) {
        lifeCountView = kAKLifeCountViewMax;
    }
    else {
        lifeCountView = lifeCount;
    }
    
    // 残機文字列を作成する
    char labelStr[8] = "";
    snprintf(labelStr, sizeof(labelStr), kAKLifeNumberFormat, lifeCountView);
    
    // ラベルが作成されていない場合
    if (m_numberLabel == NULL) {
        
        AKLog(kAKLogLife_1, "ラベル作成:\"%s\"", labelStr);
        
        // 残機数ラベルを作成する
        m_numberLabel = AKLabel::createLabel(labelStr, (int)strlen(labelStr), 1, kAKLabelFrameNone, AKLabel::ControlFont);
    }
    // ラベルが作成されている場合
    else {

        AKLog(kAKLogLife_1, "ラベル変更:\"%s\"", labelStr);

        // 表示文字列を変更する
        m_numberLabel->setString(labelStr);
    }
    
    AKLog(kAKLogLife_1, "end");
}

/*!
 @brief 幅取得
 
 ノードの幅を取得する。
 残機マークと残機数ラベルの幅の合計を返す。
 @return 幅
 */
int AKLife::getWidth()
{
    return AKScreenSize::deviceLength(kAKLifeMarkSize) + m_numberLabel->getWidth();
}
