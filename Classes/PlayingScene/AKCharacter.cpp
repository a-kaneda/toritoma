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
 @file AKCharacter.cpp
 @brief キャラクタークラス定義
 
 当たり判定を持つオブジェクトの基本クラスを定義する。
 */

#include "AKCharacter.h"
#include "AKBlock.h"

using cocos2d::CCSprite;
using cocos2d::CCPoint;
using cocos2d::CCSize;
using cocos2d::CCSpriteFrame;
using cocos2d::CCSpriteFrameCache;

/// デフォルトアニメーション間隔
static const int kAKDefaultAnimationInterval = 12;
/// 画像ファイル名のフォーマット
static const char *kAKImageFileFormat = "%s_%02d.png";
/// 画像ファイル名の最大文字数
static const unsigned int kAKMaxImageFileName = 64;

/*!
 @brief コンストラクタ

 メンバ変数に初期値を設定する。
 */
AKCharacter::AKCharacter() :
m_image(NULL), m_size(0.0f, 0.0f), m_position(0.0f, 0.0f), m_prevPosition(0.0f, 0.0f),
m_speedX(0.0f), m_speedY(0.0f), m_hitPoint(0), m_power(1), m_defence(0), m_isStaged(false),
m_animationPattern(1), m_animationInterval(kAKDefaultAnimationInterval), m_animationFrame(0),
m_animationRepeat(0), m_animationInitPattern(1), m_imageName(""), m_scrollSpeed(0.0f),
m_blockHitAction(kAKBlockHitNone), m_blockHitSide(0), m_offset(0.0f, 0.0f)
{
}

/*!
 @brief デストラクタ

 スプライトを解放する。
 */
AKCharacter::~AKCharacter()
{
    // スプライトを解放する
    m_image->removeFromParentAndCleanup(true);
}

/*!
 @brief 位置取得

 位置を取得する。
 @return 位置
 */
const CCPoint* AKCharacter::getPosition()
{
    return &m_position;
}

/*!
 @brief 位置設定

 位置を設定する。
 @param position 位置
 */
void AKCharacter::setPosition(const CCPoint &position)
{
    m_position = position;
}

/*!
 @brief 前回位置取得

 前回位置を取得する。
 @return 前回位置
 */
const CCPoint* AKCharacter::getPrevPosition()
{
    return &m_prevPosition;
}

/*!
 @brief サイズ取得

 サイズを取得する。
 @return サイズ
 */
const CCSize* AKCharacter::getSize()
{
    return &m_size;
}

/*!
 @brief 障害物と衝突した時の動作の取得

 障害物と衝突した時の動作を取得する。
 @return 障害物と衝突した時の動作
 */
enum AKBlockHitAction AKCharacter::getBlockHitAction()
{
    return m_blockHitAction;
}

/*!
 @brief x方向の速度取得
 
 x方向の速度を取得する。
 @return x方向の速度
 */
float AKCharacter::getSpeedX()
{
    return m_speedX;
}

/*!
 @brief y方向の速度取得
 
 y方向の速度を取得する。
 @return y方向の速度
 */
float AKCharacter::getSpeedY()
{
    return m_speedY;
}

/*!
 @brief 画像名の取得

 画像名を取得する。
 @return 画像名
 */
const std::string* AKCharacter::getImageName()
{
    return &m_imageName;
}

/*!
 @brief アニメーションパターン数取得
 
 アニメーションパターン数を取得する。
 @return アニメーションパターン数
 */
int AKCharacter::getAnimationPattern()
{
    return m_animationPattern;
}

/*!
 @brief アニメーション間隔取得
 
 アニメーション間隔を取得する。
 @return アニメーション間隔
 */
int AKCharacter::getAnimationInterval()
{
    return m_animationInterval;
}

/*!
 @brief ステージ上に存在しているかどうか取得

 ステージ上に存在しているかどうかを取得する。
 @return ステージ上に存在しているかどうか
 */
bool AKCharacter::isStaged()
{
    return m_isStaged;
}

/*!
 @brief ヒットポイント取得

 ヒットポイントを取得する。
 @return ヒットポイント
 */
int AKCharacter::getHitPoint()
{
    return m_hitPoint;
}

/*!
 @brief ヒットポイント設定
 
 ヒットポイントを設定する。
 @param hitPoint ヒットポイント
 */
void AKCharacter::setHitPoint(int hitPoint)
{
    m_hitPoint = hitPoint;
}

/*!
 @brief 攻撃力取得

 攻撃力を取得する。
 @return 攻撃力
 */
int AKCharacter::getPower()
{
    return m_power;
}

/*!
 @brief 防御力取得
 
 防御力を取得する。
 @return 防御力
 */
int AKCharacter::getDefence()
{
    return m_defence;
}

/*!
 @brief 現在の位置を記憶する

 現在の位置を移動前の位置に保存する。
 */
void AKCharacter::savePosition()
{
    m_prevPosition = m_position;
}

/*!
 @brief 記憶した位置を復元する

 移動前の位置を現在の位置に設定する。
 */
void AKCharacter::restorePosition()
{
    m_position = m_prevPosition;
}

/*!
 @brief 画像の取得
 
 画像を取得する。
 @return 画像
 */
CCSprite* AKCharacter::getImage()
{
    AKAssert(m_image, "画像が作成されていない");
    return m_image;
}

/*!
 @brief 画像有無チェック

 画像を読み込んでいるかどうかをチェックする。
 @retval true 画像を読み込んでいる
 @retval false 画像を読み込んでいない
 */
bool AKCharacter::hasImage()
{
    return (m_image != NULL);
}

/*!
 @brief アニメーションフレーム設定
 
 アニメーションフレームを設定する。
 @param frame アニメーションフレーム
 */
void AKCharacter::setAnimationFrame(int frame)
{
    m_animationFrame = frame;
}

/*!
 @brief 画像名の設定
 
 画像名を設定し、スプライトの作成または切り替えを行う。
 @param imageName 画像名
 */
void AKCharacter::setImageName(const std::string &imageName)
{
    AKLog(kAKLogCharacter_1, "imageName=%s", imageName.c_str());

    // スプライト名を設定する
    m_imageName = imageName;
    
    // スプライト名が設定された場合はスプライト作成を行う
    if (m_imageName.length() > 0) {
        
        // 画像ファイル名を決定する
        char imageFileName[32] = "";
        snprintf(imageFileName,
                 sizeof(imageFileName),
                 kAKImageFileFormat,
                 m_imageName.c_str(),
                 m_animationInitPattern);
        
        // スプライト作成前の場合はスプライトを作成する
        if (m_image == NULL) {

            AKLog(kAKLogCharacter_1, "スプライトの作成");

            m_image = CCSprite::createWithSpriteFrameName(imageFileName);
            AKAssert(m_image, "スプライト作成に失敗:%s", imageFileName);
        }
        // すでにスプライトを作成している場合は画像の切り替えを行う
        else {

            AKLog(kAKLogCharacter_1, "画像の切り替え");

            CCSpriteFrameCache *spriteFrameCache =
                CCSpriteFrameCache::sharedSpriteFrameCache();

            CCSpriteFrame *spriteFrame = spriteFrameCache->spriteFrameByName(imageFileName);
            AKAssert(spriteFrame, "スプライト作成に失敗:%s", imageFileName);

            m_image->setDisplayFrame(spriteFrame);
        }
    }
}

/*!
 @brief アニメーション初期パターンの設定
 
 アニメーション初期パターンを設定する。
 すでにスプライトを作成している場合はスプライトの切り替えを行う。
 @param animationInitPattern アニメーション初期パターン
 */
void AKCharacter::setAnimationInitPattern(int animationInitPattern)
{
    // メンバに設定する
    m_animationInitPattern = animationInitPattern;
    
    // すでにスプライトを作成している場合は画像の切り替えを行う
    if (m_image != NULL) {
        
        // アニメーションフレーム数を初期化する
        m_animationFrame = 0;
        
        // アニメーションパターンに応じて画像ファイル名を作成する
        char imageFileName[32] = "";
        snprintf(imageFileName,
                 sizeof(imageFileName),
                 kAKImageFileFormat,
                 m_imageName.c_str(),
                 animationInitPattern);
        
        // 表示スプライトを変更する
        CCSpriteFrameCache *spriteFrameCache =
            CCSpriteFrameCache::sharedSpriteFrameCache();
        m_image->setDisplayFrame(spriteFrameCache->spriteFrameByName(imageFileName));
    }
}

/*!
 @brief 移動処理

 速度によって位置を移動する。
 アニメーションを行う。
 @param data ゲームデータ
 */
void AKCharacter::move(AKPlayDataInterface *data)
{
    // 画面に配置されていない場合は無処理
    if (!m_isStaged) {
        return;
    }
        
    // HPが0になった場合は破壊処理を行う
    if (m_hitPoint <= 0) {
        AKLog(kAKLogCharacter_1, "HP0により消滅");
        destroy(data);
        return;
    }
    
    // 画面外に出た場合は削除する
    if (isOutOfStage(data)) {
        AKLog(kAKLogCharacter_1, "画面外移動により消滅");

        // ステージ配置フラグを落とす
        m_isStaged = false;
        
        // 画面から取り除く
        m_image->removeFromParentAndCleanup(true);
        m_image = NULL;
        
        return;
    }
    
    // 移動前の座標を記憶する
    savePosition();
    
    // 座標の移動
    // 画面スクロールの影響を受ける場合は画面スクロール分も移動する
    m_position.x += m_speedX - (data->getScrollSpeedX() * m_scrollSpeed);
    m_position.y += m_speedY - (data->getScrollSpeedY() * m_scrollSpeed);
        
    // 障害物との衝突判定を行う
    switch (m_blockHitAction) {
        case kAKBlockHitNone:       // 無処理
        case kAKBlockHitPlayer:     // 自機
            break;
            
        case kAKBlockHitMove:       // 移動
            checkHit(*data->getBlocks(), data, &AKCharacter::moveOfBlockHit);
            break;
            
        case kAKBlockHitDisappear:  // 消滅
            checkHit(*data->getBlocks(), data, &AKCharacter::disappearOfBlockHit);
            break;
            
        default:
            break;
    }
    
    // 画像表示位置の更新を行う
    updateImagePosition();
    
    // アニメーションフレーム数をカウントする
    m_animationFrame++;
    
    // 表示するパターンを決定する
    int pattern = m_animationInitPattern;
    
    // アニメーションパターンが複数存在する場合はパターン切り替えを行う
    if (m_animationPattern >= 2) {
        
        // 経過時間からパターンを決定する
        pattern = m_animationFrame / m_animationInterval + m_animationInitPattern;
        
        AKLog(kAKLogCharacter_2, "frame=%d interval=%d init=%d pattern=%d",
              m_animationFrame, m_animationInterval, m_animationInitPattern, pattern);
    
        // パターンがパターン数を超えた場合はアニメーション時間をリセットし、パターンを最初のものに戻す。
        if (pattern - (m_animationInitPattern - 1) > m_animationPattern) {
            m_animationFrame = 0;
            pattern = m_animationInitPattern;
            
            // 繰り返し回数が設定されている場合
            if (m_animationRepeat > 0) {
                
                // 繰り返し回数を減らす
                m_animationRepeat--;
                
                // 繰り返し回数が0になった場合は画面から取り除く
                if (m_animationRepeat <= 0) {
                    
                    AKLog(kAKLogCharacter_1, "繰り返し回数0により消滅");

                    // ステージ配置フラグを落とす
                    m_isStaged = false;
                    
                    // 画面から取り除く
                    m_image->removeFromParentAndCleanup(true);
                    m_image = NULL;
                    
                    return;
                }
            }
        }
        
        // アニメーションパターンに応じて画像ファイル名を作成する
        char imageFileName[32] = "";
        snprintf(imageFileName,
                 sizeof(imageFileName),
                 kAKImageFileFormat,
                 m_imageName.c_str(),
                 pattern);
        
        AKLog(kAKLogCharacter_2, "imageFileName=%s", imageFileName);
        
        // 表示スプライトを変更する
        CCSpriteFrameCache *spriteFrameCache =
            CCSpriteFrameCache::sharedSpriteFrameCache();
        m_image->setDisplayFrame(spriteFrameCache->spriteFrameByName(imageFileName));
    }
    
    // キャラクター固有の動作を行う
    action(data);
}

/*!
 @brief キャラクター固有の動作

 キャラクター種別ごとの動作を行う。
 @param data ゲームデータ
 */
void AKCharacter::action(AKPlayDataInterface *data)
{
    // 派生クラスで動作を定義する
}

/*!
 @brief 破壊処理

 HPが0になったときの処理
 @param data ゲームデータ
 */
void AKCharacter::destroy(AKPlayDataInterface *data)
{
    AKLog(kAKLogCharacter_1, "start destroy");

    // ステージ配置フラグを落とす
    m_isStaged = false;
    
    // 画面から取り除く
    m_image->removeFromParentAndCleanup(true);
    m_image = NULL;
}


/*!
 @brief 衝突処理
 
 衝突した時の処理、自分と相手のHPを減らす。
 @param character 衝突した相手
 @param data ゲームデータ
 */
void AKCharacter::hit(AKCharacter *character, AKPlayDataInterface *data)
{
    // 自分と相手のHPを衝突した相手の攻撃力と自分の防御力の差の分減らす
    if (character->getPower() - m_defence > 0) {
        m_hitPoint -= (character->getPower() - m_defence);
    }

    if (m_power - character->getDefence() > 0) {
        int newHitPoint = character->getHitPoint() - (m_power - character->getDefence());
        character->setHitPoint(newHitPoint);
    }
}

/*!
 @brief 障害物との衝突による移動
 
 障害物との衝突時に行う移動処理。
 進行方向と反対方向へ移動して障害物の境界まで戻る。
 @param character 衝突した相手
 @param data ゲームデータ
 */
void AKCharacter::moveOfBlockHit(AKCharacter *character, AKPlayDataInterface *data)
{
    // 衝突による移動先の座標を現在の座標で初期化する
    CCPoint newPoint(m_position.x, m_position.y);
    
    // 判定後の接触フラグを現在の値で初期化する
    unsigned int newHitSide = m_blockHitSide;
    
    // 障害物の横方向の端に合わせて移動した場合は
    // 縦方向の移動は不要になるため、フラグを立てて後で使用する。
    bool isXMoved = false;
    
    // x方向右に進んでいる時に衝突した場合
    if (m_position.x > m_prevPosition.x &&
        m_position.x > character->getPosition()->x - (m_size.width + character->getSize()->width) / 2) {
        
        // 前回の右端が障害物の前回の左端よりも右側ならば
        // 障害物内部に入り込んでいるものとみなし、前回値に戻す
        if (m_prevPosition.x > character->getPrevPosition()->x - (m_size.width + character->getSize()->width) / 2) {

            AKLog(kAKLogCharacter_4, "右側が障害物内部に入り込み");
            newPoint.x = m_prevPosition.x;
        }
        // そうでない場合は右端を障害物の左端に合わせる
        else {
            
            AKLog(kAKLogCharacter_4, "右側が接触");
            newPoint.x = character->getPosition()->x - (m_size.width + character->getSize()->width) / 2;
            
            // 横方向移動のフラグを立てる
            isXMoved = true;
        }
        
        // 右側との接触フラグを立てる
        newHitSide |= kAKHitSideRight;
    }
    // x方向左に進んでいる時に衝突した場合
    else if (m_position.x < m_prevPosition.x &&
             m_position.x < character->getPosition()->x + (m_size.width + character->getSize()->width) / 2) {

        
        // 前回の左端が障害物の前回の右端よりも左側ならば
        // 障害物内部に入り込んでいるものとみなし、前回値に戻す
        if (m_prevPosition.x < character->getPrevPosition()->x + (m_size.width + character->getSize()->width) / 2) {
            
            AKLog(kAKLogCharacter_4, "左側が障害物内部に入り込み");
            newPoint.x = m_prevPosition.x;
        }
        // そうでない場合は左端を障害物の右端に合わせる
        else {
            
            AKLog(kAKLogCharacter_4, "左側が接触");
            newPoint.x = character->getPosition()->x + (m_size.width + character->getSize()->width) / 2;
            
            // 横方向移動のフラグを立てる
            isXMoved = true;
        }
        
        // 左側との接触フラグを立てる
        newHitSide |= kAKHitSideLeft;
    }
    // x方向に進んでいない、または衝突していない場合は無処理
    else {
        AKLog(kAKLogCharacter_4, "横方向の接触なし");
        // 無処理
    }
    
    // 障害物の縦方向の端に合わせて移動した場合は
    // 横方向の移動は不要になるため、フラグを立てて後で使用する。
    bool isYMoved = false;
    
    // y方向上に進んでいる時に衝突した場合
    if (m_position.y > m_prevPosition.y &&
        m_position.y > character->getPosition()->y - (m_size.height + character->getSize()->height) / 2) {
        
        // 前回の上端が障害物の前回の下端よりも上側ならば
        // 障害物内部に入り込んでいるものとみなし、前回値に戻す
        if (m_prevPosition.y > character->getPrevPosition()->y - (m_size.height + character->getSize()->height) / 2) {

            AKLog(kAKLogCharacter_4, "上側が障害物内部に入り込み");
            newPoint.y = m_prevPosition.y;
        }
        // そうでない場合は上端を障害物の下端に合わせる
        else {
            
            AKLog(kAKLogCharacter_4, "上側が接触");
            newPoint.y = character->getPosition()->y - (m_size.height + character->getSize()->height) / 2;
            
            // 縦方向移動のフラグを立てる
            isYMoved = true;
        }
        
        // 上側との接触フラグを立てる
        newHitSide |= kAKHitSideTop;
    }
    // y方向下に進んでいる時に衝突した場合
    else if (m_position.y < m_prevPosition.y &&
             m_position.y < character->getPosition()->y + (m_size.height + character->getSize()->height) / 2) {
        
        // 前回の下端が障害物の前回の上端よりも下側ならば
        // 障害物内部に入り込んでいるものとみなし、前回値に戻す
        if (m_prevPosition.y < character->getPrevPosition()->y + (m_size.height + character->getSize()->height) / 2) {

            AKLog(kAKLogCharacter_4, "下側が障害物内部に入り込み");
            newPoint.y = m_prevPosition.y;
        }
        // そうでない場合は下端を障害物の上端に合わせる
        else {
            
            AKLog(kAKLogCharacter_4, "下側が接触");
            newPoint.y = character->getPosition()->y + (m_size.height + character->getSize()->height) / 2;
            
            // 縦方向移動のフラグを立てる
            isYMoved = true;
        }
        
        // 下側との接触フラグを立てる
        newHitSide |= kAKHitSideBottom;
    }
    // y方向に進んでいない、または衝突していない場合は無処理
    else {
        AKLog(kAKLogCharacter_4, "縦方向の接触なし");
        // 無処理
    }
    
    // 横方向へ移動した場合
    if (isXMoved) {
        // 横方向の座標と接触フラグを採用する
        m_position.x = newPoint.x;
        m_blockHitSide |= (newHitSide & (kAKHitSideLeft | kAKHitSideRight));
    }
    // 縦方向へ移動した場合
    else if (isYMoved) {
        // 縦方向の座標と接触フラグを採用する
        m_position.y = newPoint.y;
        m_blockHitSide |= (newHitSide & (kAKHitSideTop | kAKHitSideBottom));
    }
    // 障害物内部に入り込んでいる場合
    else {
        // 縦横両方の座標と接触フラグを採用する
        m_position.x = newPoint.x;
        m_position.y = newPoint.y;
        m_blockHitSide = newHitSide;
    }
}

/*!
 @brief 障害物との衝突による消滅
 
 障害物との衝突時に行う消滅処理。自分のHPを0にする。
 @param character 衝突した相手
 @param data ゲームデータ
 */
void AKCharacter::disappearOfBlockHit(AKCharacter *character, AKPlayDataInterface *data)
{
    m_hitPoint = 0.0;
}

/*!
 @brief 画面外配置判定
 
 キャラクターが画面範囲外に配置されているか調べる。
 座標が範囲外で、外側に向かって移動している場合は範囲外とみなす。
 範囲内に向かって移動している場合は範囲内とみなす。
 @param data ゲームデータ
 @return 範囲外に出ている場合はYES、範囲内にある場合はNO
 */
bool AKCharacter::isOutOfStage(AKPlayDataInterface *data)
{
    // 表示範囲外でキャラクターを残す範囲
    const float kAKBorder = 50.0f;
    
    if ((m_position.x < -kAKBorder &&
         (m_speedX - data->getScrollSpeedX() * m_scrollSpeed) < 0.0f) ||
        (m_position.x > AKScreenSize::stageSize().width + kAKBorder &&
         (m_speedX - data->getScrollSpeedX() * m_scrollSpeed) > 0.0f) ||
        (m_position.y < -kAKBorder &&
         (m_speedY - data->getScrollSpeedY() * m_scrollSpeed) < 0.0f) ||
        (m_position.y > AKScreenSize::stageSize().height + kAKBorder &&
         (m_speedY - data->getScrollSpeedY() * m_scrollSpeed) > 0.0f)) {
        
        AKLog(kAKLogCharacter_1, "画面外に出たため削除");
        
        return true;
    }
    else {
        return false;
    }
}

/*!
 @brief 画像表示位置更新
 
 画像の表示位置を現在のキャラクター位置とオフセットをもとに更新を行う。
 */
void AKCharacter::updateImagePosition()
{
    // 画像の回転している角度を取得する
    float angle = AKAngle::convertAngleScr2Rad(m_image->getRotation());
    
    // 回転している方向に合わせて画像をずらす距離を計算する
    float dx = m_offset.x * sinf(angle) + m_offset.y * cosf(angle);
    float dy = - m_offset.x * cosf(angle) + m_offset.y * sinf(angle);
    
    // 移動後の座標を計算する
    float x = AKScreenSize::xOfStage(m_position.x + dx);
    float y = AKScreenSize::yOfStage(m_position.y + dy);
    
    m_image->setPosition(CCPoint(x, y));
}
