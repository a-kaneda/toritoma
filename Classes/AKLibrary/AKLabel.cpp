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

using cocos2d::SpriteBatchNode;
using cocos2d::Rect;
using cocos2d::Vec2;
using cocos2d::SpriteFrame;
using cocos2d::Sprite;
using cocos2d::Label;
using cocos2d::TTFConfig;
using cocos2d::Color3B;
using cocos2d::SpriteFrameCache;

/// ラベル枠テクスチャアトラス設定ファイル名
static const char LabelFrameTextureDef[] = "LabelFrame.plist";
/// ラベル枠テクスチャアトラス画像ファイル名
static const char LabelFrameTextureImage[] = "LabelFrame.png";
/// 左上の枠の画像ファイル名
static const char TopLeft[] = "TopLeft.png";
/// 右上の枠の画像ファイル名
static const char TopRight[] = "TopRight.png";
/// 左下の枠の画像ファイル名
static const char BottomLeft[] = "BottomLeft.png";
/// 右下の枠の画像ファイル名
static const char BottomRight[] = "BottomRight.png";
/// 上の枠の画像ファイル名
static const char TopBar[] = "TopBar.png";
/// 左の枠の画像ファイル名
static const char LeftBar[] = "LeftBar.png";
/// 右の枠の画像ファイル名
static const char RightBar[] = "RightBar.png";
/// 下の枠の画像ファイル名
static const char BottomBar[] = "BottomBar.png";
/// ボタン左上の枠の画像ファイル名
static const char ButtonTopLeft[] = "ButtonTopLeft.png";
/// ボタン右上の枠の画像ファイル名
static const char ButtonTopRight[] = "ButtonTopRight.png";
/// ボタン左下の枠の画像ファイル名
static const char ButtonBottomLeft[] = "ButtonBottomLeft.png";
/// ボタン右下の枠の画像ファイル名
static const char ButtonBottomRight[] = "ButtonBottomRight.png";
/// ボタン上の枠の画像ファイル名
static const char ButtonTopBar[] = "ButtonTopBar.png";
/// ボタン左の枠の画像ファイル名
static const char ButtonLeftBar[] = "ButtonLeftBar.png";
/// ボタン右の枠の画像ファイル名
static const char ButtonRightBar[] = "ButtonRightBar.png";
/// ボタン下の枠の画像ファイル名
static const char ButtonBottomBar[] = "ButtonBottomBar.png";
/// ボタンブランクの画像ファイル名
static const char Blank[] = "Blank.png";
/// 反転時の画像プレフィックス
static const char Revesese[] = "Reverse";
/// フォントサイズ
static const int FontSize = 16;
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
        return (length + 2) * FontSize;
    }
    // 枠がない場合は文字領域のサイズを返す
    else {
        return length * FontSize;
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
        return ((int)(line * kAKLabelLineHeight) + 2) * FontSize;
    }
    // 枠がない場合は文字領域のサイズを返す
    else {
        return (int)(line * kAKLabelLineHeight) * FontSize;
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
Rect AKLabel::getRect(Vec2 position, int length, int line, bool hasFrame)
{
    return Rect(position.x - AKLabel::getWidth(length, hasFrame) / 2,
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
    AKAssert(AKStringSplitter::getStringLength(str.c_str()) <= length * line, "文字列が表示可能文字数を超えている:str.size()=%d, length=%d, line=%d", AKStringSplitter::getStringLength(str.c_str()), length, line);
    
    // 行数、文字数は最低でも1以上とする
    AKAssert(length > 0, "文字数が0以下:length=%d", length);
    AKAssert(line > 0, "行数が0以下:line=%d", line);
    
    // メンバ変数を初期化する
    m_text = NULL;
    
    // テクスチャアトラスを読み込む
    SpriteFrameCache *spriteFrameCache = SpriteFrameCache::getInstance();
    spriteFrameCache->addSpriteFramesWithFile(LabelFrameTextureDef,
                                              LabelFrameTextureImage);
    
    // 枠示用バッチノードを作成する
    SpriteBatchNode *frameBatchNode = SpriteBatchNode::create(LabelFrameTextureImage, (m_length + 2) * (m_line * 1.5 + 2));
    AKAssert(frameBatchNode != NULL, "枠表示用バッチノードの作成に失敗");
    
    // バッチノードを配置する
    this->addChild(frameBatchNode, kAKFrameBatchPosZ, kAKFrameBatchPosZ);

    // 枠を作成する
    createFrame();
    
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
const std::string& AKLabel::getString() const
{
    return m_labelString;
}

/*!
 @brief 表示文字列の設定
 
 表示文字列を変更する。
 @param label 表示文字列
 */
void AKLabel::setString(const std::string &label)
{
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
Rect AKLabel::getRect()
{
    return Rect(getPosition().x - getWidth() / 2,
                getPosition().y - getHeight() / 2,
                getWidth(),
                getHeight());
}

/*!
 @brief 枠表示用バッチノード取得
 
 枠表示用バッチノードを取得する
 @return 枠表示用バッチノード
 */
SpriteBatchNode* AKLabel::getFrameBatch()
{
    SpriteBatchNode *frameBatch = static_cast<SpriteBatchNode*>(getChildByTag(kAKFrameBatchPosZ));
    
    AKAssert(frameBatch, "枠表示用バッチノードが作成されていない");
    
    return frameBatch;
}

/*!
 @brief 表示更新
 
 表示内容を更新する。
 */
void AKLabel::updateLabel()
{
    // ラベルを削除する
    if (m_text != NULL) {
        removeChild(m_text);
        m_text = NULL;
    }
    
    // フォント設定を作成する
    TTFConfig config(MISAKI_FONT, 16);
    
    // ラベルを作成する
    m_text = Label::createWithTTF(config, convertHalfCharacter(m_labelString.c_str()));
    
    // ラベル高さを設定する
    m_text->setHeight(getHeight(m_line, false));
    
    // ラベル幅を設定する
    m_text->setWidth(getWidth(m_length, false));
    
    // 色を設定する
    m_text->setColor(cocos2d::Color3B(kAKColor[kAKColorDark]));
    
    // タグを設定する
    m_text->setTag(1);
    
    // ラベルを配置する
    addChild(m_text, kAKLabelBatchPosZ);
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
    
    // 反転時は反転画像プレフィックスを先頭につける
    if (m_isReverse) {
        keyTopLeft = Revesese;
        keyTopRight = Revesese;
        keyBottomLeft = Revesese;
        keyBottomRight = Revesese;
        keyTopBar = Revesese;
        keyBottomBar = Revesese;
        keyLeftBar = Revesese;
        keyRightBar = Revesese;
    }
    
    // 枠の幅
    int framewidth = 0;
    
    // 枠の種類に応じてキー文字列を切り替える
    switch (m_frame) {
            
        case kAKLabelFrameMessage:  // メッセージボックス
            
            keyTopLeft += TopLeft;
            keyTopRight += TopRight;
            keyBottomLeft += BottomLeft;
            keyBottomRight += BottomRight;
            keyTopBar += TopBar;
            keyBottomBar += BottomBar;
            keyLeftBar += LeftBar;
            keyRightBar += RightBar;
            framewidth = 1;
            
            break;
            
        case kAKLabelFrameButton:   // ボタン
            keyTopLeft += ButtonTopLeft;
            keyTopRight += ButtonTopRight;
            keyBottomLeft += ButtonBottomLeft;
            keyBottomRight += ButtonBottomRight;
            keyTopBar += ButtonTopBar;
            keyBottomBar += ButtonBottomBar;
            keyLeftBar += ButtonLeftBar;
            keyRightBar += ButtonRightBar;
            framewidth = 1;

            break;
            
        default:
            break;
    }
    
    // スプライトフレームキャッシュを取得する
    SpriteFrameCache *spriteFrameCache = SpriteFrameCache::getInstance();
    
    // 行間に0.5文字分の隙間を空けるため、行数の1.5倍の高さを用意する。
    // 枠を入れるため、上下+-1個分用意する。
    for (int y = -framewidth; y < (int)(m_line * kAKLabelLineHeight) + framewidth; y++) {
        
        AKLog(kAKLogLabel_1, "y=%d pos=%f", y, (-y + m_line * kAKLabelLineHeight / 2.0f) * FontSize);
        
        // 枠を入れるため、左右+-1個分用意する。
        for (int x = -framewidth; x < m_length + framewidth; x++) {
            
            AKLog(kAKLogLabel_1 && y == -1, "x=%d pos=%f", x, (x - m_length / 2.0f) * FontSize);
            
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
                
                key = "";

                // 反転時はプレフィックスを付ける
                if (m_isReverse) {
                    key = Revesese;
                }
                
                key += Blank;
            }
            
            // 先頭からの文字数をタグとする
            // xとyはそれぞれ-1からスタートしているため、+1して0からスタートするように補正する。
            int tag = (x + 1) + (y + 1) * (m_length + 1 + 1);
            AKLog(kAKLogLabel_1, "tag=%d", tag);
            
            // スプライトフレームを生成する
            SpriteFrame *charSpriteFrame = spriteFrameCache->getSpriteFrameByName(key);
            
            // バッチノードからスプライトを取り出す
            Sprite *charSprite = static_cast<Sprite*>(getFrameBatch()->getChildByTag(tag));
            
            // 取得できない場合はスプライトを生成する
            if (charSprite == NULL) {
                
                //　スプライトを生成する
                charSprite = Sprite::createWithSpriteFrame(charSpriteFrame);
                
                // 表示位置を設定する。
                // テキスト領域の中央とバッチノードの中央を一致させるため、
                // 左に1行の長さの半分、上方向に行数の半分移動する。
                charSprite->setPosition(Vec2((x - (m_length - 1) / 2.0f) * FontSize,
                                                (-y + (m_line - 1) * kAKLabelLineHeight / 2.0f) * FontSize));
                
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

/*!
 @brief 半角文字全角文字変換
 
 半角英数を全角英数に変換する。
 \nを改行コードに変換する。
 @param org 元になる文字列
 @return 返還後の文字列
 */
std::string AKLabel::convertHalfCharacter(const char *org)
{
    int i = 0;
    std::string ret;
    
    // 文字列長を取得する
    size_t len = strlen(org);
    
    while (org[i] != '\0') {
    
        // 処理対象の文字のバイト数を取得する
        int byte = AKStringSplitter::getByteOfCharacter(org[i]);
        
        // マルチバイト文字はそのままコピーする
        if (byte > 1) {
            
            for (int j = i; j < i + byte; j++) {
                ret += org[j];
            }
            i += byte;
            continue;
        }
        
        // "\n"は改行コードに変換する
        if (i + 1 < len && org[i] == '\\' && org[i + 1] == 'n') {
            ret += '\n';
            i += 2;
            continue;
        }
        
        // 半角英数を全角英数に変換する
        switch (org[i]) {
            case '0':
                ret += "０";
                break;

            case '1':
                ret += "１";
                break;
                
            case '2':
                ret += "２";
                break;
                
            case '3':
                ret += "３";
                break;
                
            case '4':
                ret += "４";
                break;
                
            case '5':
                ret += "５";
                break;
                
            case '6':
                ret += "６";
                break;
                
            case '7':
                ret += "７";
                break;
                
            case '8':
                ret += "８";
                break;
                
            case '9':
                ret += "９";
                break;
                
            case 'A':
                ret += "Ａ";
                break;
                
            case 'B':
                ret += "Ｂ";
                break;
                
            case 'C':
                ret += "Ｃ";
                break;
                
            case 'D':
                ret += "Ｄ";
                break;

            case 'E':
                ret += "Ｅ";
                break;

            case 'F':
                ret += "Ｆ";
                break;
                
            case 'G':
                ret += "Ｇ";
                break;
                
            case 'H':
                ret += "Ｈ";
                break;
                
            case 'I':
                ret += "Ｉ";
                break;
                
            case 'J':
                ret += "Ｊ";
                break;
                
            case 'K':
                ret += "Ｋ";
                break;
                
            case 'L':
                ret += "Ｌ";
                break;
                
            case 'M':
                ret += "Ｍ";
                break;
                
            case 'N':
                ret += "Ｎ";
                break;
                
            case 'O':
                ret += "Ｏ";
                break;
                
            case 'P':
                ret += "Ｐ";
                break;
                
            case 'Q':
                ret += "Ｑ";
                break;
                
            case 'R':
                ret += "Ｒ";
                break;
                
            case 'S':
                ret += "Ｓ";
                break;
                
            case 'T':
                ret += "Ｔ";
                break;
                
            case 'U':
                ret += "Ｕ";
                break;
                
            case 'V':
                ret += "Ｖ";
                break;
                
            case 'W':
                ret += "Ｗ";
                break;
                
            case 'X':
                ret += "Ｘ";
                break;
                
            case 'Y':
                ret += "Ｙ";
                break;
                
            case 'Z':
                ret += "Ｚ";
                break;
                
            case '.':
                ret += "．";
                break;
                
            case ' ':
                ret += "　";
                break;
                
            case '-':
                ret += "−";
                break;
                
            case '?':
                ret += "？";
                break;
                
            default:
                ret += org[i];
                break;
        }
        
        i++;
    }
    
    return ret;
}
