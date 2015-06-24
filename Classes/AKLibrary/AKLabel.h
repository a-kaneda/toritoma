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
 @file AKLabel.h
 @brief ラベル表示クラス
 
 テキストラベルを表示するクラスを定義する。
 */

#ifndef AKLABEL_H
#define AKLABEL_H

#include "AKFont.h"

/// ラベルの枠のタイプ
enum AKLabelFrame {
    kAKLabelFrameNone = 0,  ///< 枠なし
    kAKLabelFrameMessage,   ///< メッセージボックス
    kAKLabelFrameButton     ///< ボタン
};

/*!
 @brief ラベル表示クラス
 
 テキストラベルを表示する。
 */
class AKLabel : public cocos2d::Node, cocos2d::LabelProtocol {
public:
    // コンビニエンスコンストラクタ
    static AKLabel* createLabel(const std::string &str, int length, int line, enum AKLabelFrame frame);

public:
    // 指定文字数の幅取得
    static int getWidth(int length, bool hasFrame);
    // 指定行数の高さ取得
    static int getHeight(int line, bool hasFrame);
    // 指定文字数、指定行数の指定位置の矩形範囲取得
    static cocos2d::Rect getRect(cocos2d::Vec2 position, int length, int line, bool hasFrame);
    
private:
    /// 表示文字列
    std::string m_labelString;
    /// １行の表示文字数
    int m_length;
    /// 表示行数
    int m_line;
    /// 枠のタイプ
    enum AKLabelFrame m_frame;
    /// 色反転するかどうか
    bool m_isReverse;
    
private:
    // デフォルトコンストラクタは使用禁止にする
    AKLabel();
    // 初期文字列を指定したコンストラクタ
    AKLabel(const std::string &str, int length, int line, enum AKLabelFrame frame);
    
public:
    // デストラクタ
    virtual ~AKLabel();
    // 色反転するかどうかの設定
    void setIsReverse(bool isReverse);
    // 文字列の取得
    virtual const std::string &getString() const;
    // 文字列の設定
    virtual void setString(const std::string &label);
    // ラベルの幅の取得
    int getWidth();
    // ラベルの高さの取得
    int getHeight();
    // ラベルの矩形領域の取得
    cocos2d::Rect getRect();
    
private:
    // 枠表示用バッチノード取得
    cocos2d::SpriteBatchNode* getFrameBatch();
    // 文字表示用バッチノード取得
    cocos2d::SpriteBatchNode* getLabelBatch();
    // 表示更新
    void updateLabel();
    // 枠の生成
    void createFrame();
};

#endif
