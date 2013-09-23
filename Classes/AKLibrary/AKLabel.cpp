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
 @file AKLabel.cpp
 @brief ラベル表示クラス
 
 テキストラベルを表示するクラスを定義する。
 */

#include "AKLabel.h"
#include "AKStringSplitter.h"

using cocos2d::CCSpriteBatchNode;
using cocos2d::CCRect;
using cocos2d::CCPoint;
using cocos2d::CCSpriteFrame;
using cocos2d::CCSprite;

/// 左上の枠のキー
static const char kAKTopLeft[] = "TopLeft";
/// 右上の枠のキー
static const char kAKTopRight[] = "TopRight";
/// 左下の枠のキー
static const char kAKBottomLeft[] = "BottomLeft";
/// 右下の枠のキー
static const char kAKBottomRight[] = "BottomRight";
/// 上の枠のキー
static const char kAKTopBar[] = "TopBar";
/// 左の枠のキー
static const char kAKLeftBar[] = "LeftBar";
/// 右の枠のキー
static const char kAKRightBar[] = "RightBar";
/// 下の枠のキー
static const char kAKBottomBar[] = "BottomBar";
/// ボタン左上の枠のキー
static const char kAKButtonTopLeft[] = "ButtonTopLeft";
/// ボタン右上の枠のキー
static const char kAKButtonTopRight[] = "ButtonTopRight";
/// ボタン左下の枠のキー
static const char kAKButtonBottomLeft[] = "ButtonBottomLeft";
/// ボタン右下の枠のキー
static const char kAKButtonBottomRight[] = "ButtonBottomRight";
/// ボタン上の枠のキー
static const char kAKButtonTopBar[] = "ButtonTopBar";
/// ボタン左の枠のキー
static const char kAKButtonLeftBar[] = "ButtonLeftBar";
/// ボタン右の枠のキー
static const char kAKButtonRightBar[] = "ButtonRightBar";
/// ボタン下の枠のキー
static const char kAKButtonBottomBar[] = "ButtonBottomBar";
/// ボタンブランクのキー
static const char kAKBlank[] = " ";

/// 1行の高さ(単位：文字)
static const float kAKLabelLineHeight = 1.5f;

// バッチノードのz座標(タグ兼用)
enum {
    kAKFrameBatchPosZ = 0,  ///< 枠表示用バッチノードのz座標
    kAKLabelBatchPosZ       ///< 文字表示用バッチノードのz座標
};

/*!
 @brief 指定文字数の幅取得
 
 指定された文字数のラベルの幅を取得する。
 文字数にフォントサイズをかけて返す。
 枠付きが指定された場合は枠のサイズ2文字分をプラスする。
 @param length 文字数
 @param hasFrame 枠付きかどうか
 @return ラベルの幅
 */
int AKLabel::getWidth(int length, bool hasFrame)
{
    // 枠がある場合は枠の領域を2文字分プラスして返す
    if (hasFrame) {
        return (length + 2) * AKFont::getFontSize();
    }
    // 枠がない場合は文字領域のサイズを返す
    else {
        return length * AKFont::getFontSize();
    }
}

/*!
 @brief 指定行数の高さ取得
 
 指定された行数のラベルの高さを取得する。
 行数にフォントサイズをかけた値と行間の高さを足した値を返す。
 枠付きが指定された場合は枠のサイズ2文字分をプラスする。
 @param line 行数
 @param hasFrame 枠付きかどうか
 @return ラベルの高さ
 */
int AKLabel::getHeight(int line, bool hasFrame)
{
    // 枠がある場合は枠の領域を2文字分プラスして返す
    if (hasFrame) {
        return ((int)(line * kAKLabelLineHeight) + 2) * AKFont::getFontSize();
    }
    // 枠がない場合は文字領域のサイズを返す
    else {
        return (int)(line * kAKLabelLineHeight) * AKFont::getFontSize();
    }
}

/*!
 @brief 指定文字数、指定行数の指定位置の矩形範囲取得
 
 指定された文字数、行数、指定位置のラベルの矩形範囲を取得する。
 @param x ラベルの中心座標x座標
 @param y ラベルの中心座標y座標
 @param length 1行の文字数
 @param line 行数
 @param hasFrame 枠付きかどうか
 @return ラベルの矩形範囲
 */
CCRect AKLabel::getRect(CCPoint position, int length, int line, bool hasFrame)
{
    return CCRectMake(position.x - AKLabel::getWidth(length, hasFrame) / 2,
                               position.y - AKLabel::getHeight(line, hasFrame) / 2,
                               AKLabel::getWidth(length, hasFrame),
                               AKLabel::getHeight(line, hasFrame));
}

/*!
 @brief 初期文字列を指定したコンストラクタ
 
 初期文字列を指定して初期化を行う。
 @param str 表示文字列
 @param length 1行の文字数
 @param line 表示行数
 @param frame 枠の種類
 */
AKLabel::AKLabel(const std::string &str, int length, int line, enum AKLabelFrame frame) :
m_length(length), m_line(line), m_frame(frame), m_isReverse(false)
{
    // 文字列が表示可能文字数を超えている場合はエラー
    AKAssert(str.length() <= length * line, "文字列が表示可能文字数を超えている:str.size()=%lu, length=%d, line=%d", str.size(), length, line);
    
    // 行数、文字数は最低でも1以上とする
    AKAssert(length > 0, "文字数が0以下:length=%d", length);
    AKAssert(line > 0, "行数が0以下:line=%d", line);
    
    // 文字表示用バッチノードを作成する
    CCSpriteBatchNode *batchNode = AKFont::getInstance().createBatchNode(m_length * m_line);
    this->addChild(batchNode, kAKLabelBatchPosZ, kAKLabelBatchPosZ);
    
    // 枠付きの場合は枠の作成を行う
    if (m_frame != kAKLabelFrameNone) {
        
        // 枠示用バッチノードを作成する
        CCSpriteBatchNode *frameBatchNode = AKFont::getInstance().createBatchNode((m_length + 2) * (m_line * 1.5 + 2));
        AKAssert(frameBatchNode != NULL, "枠表示用バッチノードの作成に失敗");
        
        this->addChild(frameBatchNode, kAKFrameBatchPosZ, kAKFrameBatchPosZ);

        // 枠を作成する
        createFrame();
    }
    
    // 各文字のスプライトを生成し、バッチノードへ登録する
    for (int y = 0; y < m_line; y++) {
        
        for (int x = 0; x < m_length; x++) {
                
            // フォントクラスからスプライトフレームを生成する
            CCSpriteFrame *charSpriteFrame = AKFont::getInstance().createSpriteFrame(" ", m_isReverse);
            
            // スプライトを生成する
            CCSprite *charSprite = CCSprite::createWithSpriteFrame(charSpriteFrame);
            
            // 表示位置を設定する。
            // テキスト領域の中央とバッチノードの中央を一致させるため、
            // 左に1行の長さの半分、上方向に行数の半分移動する。
            // 行間に0.5文字分の隙間を入れるため、高さは1.5倍する。
            CCPoint position((x - (m_length - 1) / 2.0f) * AKFont::getFontSize(),
                             (-y + (m_line - 1) / 2.0f) * AKFont::getFontSize() * kAKLabelLineHeight);
            charSprite->setPosition(position);
            
            // バッチノードに登録する
            getLabelBatch()->addChild(charSprite);
            
            // 先頭からの文字数をタグにする
            charSprite->setTag(x + y * m_length);
        }
    }
    
    // ラベル文字列を設定する
    setString(str.c_str());
}

/*!
 @brief デストラクタ
 
 無処理。
 */
AKLabel::~AKLabel()
{
    
}

/*!
 @brief コンビニエンスコンストラクタ

 インスタンスを生成し、初期化処理を行い、autoreleaseを行う。
 @param str 表示文字列
 @param length 1行の文字数
 @param line 表示行数
 @param frame 枠の種類
 @return 生成したインスタンス
 */
AKLabel* AKLabel::createLabel(const std::string &str, int length, int line, enum AKLabelFrame frame)
{
    AKLabel *instance = new AKLabel(str, length, line, frame);
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
 @brief 色反転するかどうかの設定
 
 色反転するかどうかを設定する。
 設定したあとに表示文字列の更新を行う。
 @param isReverse 色反転するかどうか
 */
void AKLabel::setIsReverse(bool isReverse)
{
    // 変更された場合のみ処理を行う
    if (m_isReverse != isReverse) {
        
        AKLog(kAKLogLabel_1, "色反転設定:%d", isReverse);

        // メンバに設定する
        m_isReverse = isReverse;
        
        // 枠を更新する
        createFrame();
        
        // 表示文字列を更新する
        updateLabel();
    }
}

/*!
 @brief 表示文字列の取得
 
 表示文字列を取得する
 @return 表示文字列
 */
const char* AKLabel::getString(void)
{
    return m_labelString.c_str();
}

/*!
 @brief 表示文字列の設定
 
 表示文字列を変更する。
 @param label 表示文字列
 */
void AKLabel::setString(const char *label)
{
    // 文字列が表示可能文字数を超えている場合はエラー
    AKAssert(strlen(label) <= m_length * m_line, "文字列長が表示可能数を超えている:label=%s, m_length=%d, m_line=%d", label, m_length, m_line);
    
    // パラメータをメンバに設定する
    if (m_labelString != label) {
        m_labelString = label;
    }
    
    // 表示を更新する
    updateLabel();
}

/*!
 @brief ラベルの幅の取得
 
 ラベルの幅を取得する。
 クラスメソッドの幅取得処理にメンバの値を渡して計算を行う。
 @return ラベルの幅
 */
int AKLabel::getWidth()
{
    return AKLabel::getWidth(m_length, (m_frame != kAKLabelFrameNone));
}

/*!
 @brief ラベルの高さの取得
 
 ラベルの高さを取得する。
 クラスメソッドの高さ取得処理にメンバの値を渡して計算を行う。
 @return ラベルの高さ
 */
int AKLabel::getHeight()
{
    return AKLabel::getHeight(m_line, (m_frame != kAKLabelFrameNone));
}

/*!
 @brief ラベルの矩形領域の取得
 
 ラベルの矩形領域を取得する。
 @return ラベルの矩形領域
 */
CCRect AKLabel::getRect()
{
    return CCRectMake(getPosition().x - getWidth() / 2,
                               getPosition().y - getHeight() / 2,
                               getWidth(),
                               getHeight());
}

/*!
 @brief 枠表示用バッチノード取得
 
 枠表示用バッチノードを取得する
 @return 枠表示用バッチノード
 */
CCSpriteBatchNode* AKLabel::getFrameBatch()
{
    CCSpriteBatchNode *frameBatch = static_cast<CCSpriteBatchNode*>(getChildByTag(kAKFrameBatchPosZ));
    
    AKAssert(frameBatch, "枠表示用バッチノードが作成されていない");
    
    return frameBatch;
}

/*!
 @brief 文字表示用バッチノード取得
 
 文字表示用バッチノードを取得する
 @return 文字表示用バッチノード
 */
CCSpriteBatchNode* AKLabel::getLabelBatch()
{
    CCSpriteBatchNode *labelBatch = static_cast<CCSpriteBatchNode*>(getChildByTag(kAKLabelBatchPosZ));
    
    AKAssert(labelBatch, "文字表示用バッチノードが作成されていない");

    return labelBatch;
}

/*!
 @brief 表示更新
 
 表示内容を更新する。
 */
void AKLabel::updateLabel()
{
    // 各文字のスプライトを変更する
    bool isNewLine = false;
    AKStringSplitter stringSplitter(m_labelString);
    for (int y = 0; y < m_line; y++) {
        
        // 改行フラグを落とす
        isNewLine = false;
        
        for (int x = 0; x < m_length; x++) {
            
            // バッチノードからスプライトを取り出す
            CCSprite *charSprite = dynamic_cast<CCSprite*>(getLabelBatch()->getChildByTag(x + y * m_length));
            if (charSprite == NULL) {
                AKAssert(false, "スプライトの取り出しに失敗:x=%d, y=%d, m_length=%d", x, y, m_length);
                continue;
            }
            
            // 出力用文字列を用意する
            char c[MB_LEN_MAX + 1] = " ";
            
            // 改行されていない場合は文字列を切り取る
            if (!isNewLine) {
                const char *splitChar = stringSplitter.split();
                
                // 文字が切り取れた場合は出力文字に設定する
                if (splitChar != NULL) {
                    
                    // 改行文字の場合は改行フラグを立て、出力文字は変更しない
                    if (splitChar[0] == '\n') {
                        
                        // 行頭の改行文字は無視する
                        if (x == 0) {
                            splitChar = stringSplitter.split();
                        }
                        
                        isNewLine = true;
                    }
                    else {
                        strncpy(c, splitChar, sizeof(c));
                    }
                }
            }
            
            AKLog(kAKLogLabel_1, "x=%d y=%d c=%s", x, y, c);
            
            // フォントクラスからスプライトフレームを生成する
            CCSpriteFrame *charSpriteFrame = AKFont::getInstance().createSpriteFrame(c, m_isReverse);
            AKAssert(charSpriteFrame, "スプライトフレームの作成に失敗:%s", c);
            
            // スプライトを差し替える
            if (charSpriteFrame != NULL) {
                charSprite->setDisplayFrame(charSpriteFrame);
            }
        }
        
        // 行末の改行文字は飛ばす
        if (stringSplitter.getCurrentByte() == '\n') {
            stringSplitter.split();
        }
    }    
}

/*!
 @brief 枠の生成
 
 枠を生成する。
 */
void AKLabel::createFrame()
{
    std::string keyTopLeft;
    std::string keyTopRight;
    std::string keyBottomLeft;
    std::string keyBottomRight;
    std::string keyTopBar;
    std::string keyBottomBar;
    std::string keyLeftBar;
    std::string keyRightBar;
    
    // 枠の種類に応じてキー文字列を切り替える
    switch (m_frame) {
            
        case kAKLabelFrameMessage:  // メッセージボックス
            keyTopLeft = kAKTopLeft;
            keyTopRight = kAKTopRight;
            keyBottomLeft = kAKBottomLeft;
            keyBottomRight = kAKBottomRight;
            keyTopBar = kAKTopBar;
            keyBottomBar = kAKBottomBar;
            keyLeftBar = kAKLeftBar;
            keyRightBar = kAKRightBar;
            
            break;
            
        case kAKLabelFrameButton:   // ボタン
            keyTopLeft = kAKButtonTopLeft;
            keyTopRight = kAKButtonTopRight;
            keyBottomLeft = kAKButtonBottomLeft;
            keyBottomRight = kAKButtonBottomRight;
            keyTopBar = kAKButtonTopBar;
            keyBottomBar = kAKButtonBottomBar;
            keyLeftBar = kAKButtonLeftBar;
            keyRightBar = kAKButtonRightBar;

            break;
            
        default:
            AKAssert(false, "枠の種類が異常:m_frame=%d", m_frame);
            return;
    }
    
    // 行間に0.5文字分の隙間を空けるため、行数の1.5倍の高さを用意する。
    // 枠を入れるため、上下+-1個分用意する。
    for (int y = -1; y < (int)(m_line * kAKLabelLineHeight) + 1; y++) {
        
        AKLog(kAKLogLabel_1, "y=%d pos=%f", y, (-y + m_line * kAKLabelLineHeight / 2.0f) * AKFont::getFontSize());
        
        // 枠を入れるため、左右+-1個分用意する。
        for (int x = -1; x < m_length + 1; x++) {
            
            AKLog(kAKLogLabel_1 && y == -1, "x=%d pos=%f", x, (x - m_length / 2.0f) * AKFont::getFontSize());
            
            // キー文字列
            std::string key;
            
            // 左上の場合
            if (y == -1 && x == -1) {
                AKLog(kAKLogLabel_1, "x=%d y=%d topleft", x, y);
                key = keyTopLeft;
            }
            // 右上の場合
            else if (y == -1 && x == m_length) {
                AKLog(kAKLogLabel_1, "x=%d y=%d topright", x, y);
                key = keyTopRight;
            }
            // 左下の場合
            else if (y == (int)(m_line * 1.5) && x == -1) {
                AKLog(kAKLogLabel_1, "x=%d y=%d bottomleft", x, y);
                key = keyBottomLeft;
            }
            // 右下の場合
            else if (y == (int)(m_line * 1.5) && x == m_length) {
                AKLog(kAKLogLabel_1, "x=%d y=%d bottomright", x, y);
                key = keyBottomRight;
            }
            // 上の場合
            else if (y == -1) {
                AKLog(kAKLogLabel_1, "x=%d y=%d topbar", x, y);
                key = keyTopBar;
            }
            // 左の場合
            else if (x == -1) {
                AKLog(kAKLogLabel_1, "x=%d y=%d leftbar", x, y);
                key = keyLeftBar;
            }
            // 右の場合
            else if (x == m_length) {
                AKLog(kAKLogLabel_1, "x=%d y=%d rightbar", x, y);
                key = keyRightBar;
            }
            // 下の場合
            else if (y == (int)(m_line * 1.5)) {
                AKLog(kAKLogLabel_1, "x=%d y=%d bottombar", x, y);
                key = keyBottomBar;
            }
            // 文字の部分の場合
            else {
                key = kAKBlank;
            }
            
            // 先頭からの文字数をタグとする
            // xとyはそれぞれ-1からスタートしているため、+1して0からスタートするように補正する。
            int tag = (x + 1) + (y + 1) * (m_length + 1 + 1);
            AKLog(kAKLogLabel_1, "tag=%d", tag);
            
            // フォントクラスからスプライトフレームを生成する
            CCSpriteFrame *charSpriteFrame = AKFont::getInstance().createSpriteFrame(key, m_isReverse);
            
            // バッチノードからスプライトを取り出す
            CCSprite *charSprite = static_cast<CCSprite*>(getFrameBatch()->getChildByTag(tag));
            
            // 取得できない場合はスプライトを生成する
            if (charSprite == NULL) {
                
                //　スプライトを生成する
                charSprite = CCSprite::createWithSpriteFrame(charSpriteFrame);
                
                // 表示位置を設定する。
                // テキスト領域の中央とバッチノードの中央を一致させるため、
                // 左に1行の長さの半分、上方向に行数の半分移動する。
                charSprite->setPosition(CCPointMake((x - (m_length - 1) / 2.0f) * AKFont::getFontSize(),
                                                             (-y + (m_line - 1) * kAKLabelLineHeight / 2.0f) * AKFont::getFontSize()));
                
                // バッチノードに登録する
                getFrameBatch()->addChild(charSprite, 0, tag);
            }
            else {
            
                // スプライトを差し替える
                charSprite->setDisplayFrame(charSpriteFrame);
            }
        }
    }
}
