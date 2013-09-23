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
 @file AKBlock.cpp
 @brief 障害物クラス
 
 障害物を管理するクラスを定義する。
 */

#include "AKBlock.h"

using cocos2d::CCPoint;

/// 画像名のフォーマット
static const char *kAKImageNameFormat = "Block_%02d";
/// 障害物の種類の数
static const int kAKBlockDefCount = 6;

/// 障害物定義
static const struct AKBlcokDef kAKBlockDef[kAKBlockDefCount] = {
    {1, 1, 0.0f, 32, 32, 0,  0},   // 地面内側
    {2, 1, 0.0f, 32, 32, 0,  0},   // 地面
    {3, 1, 0.0f, 32, 16, 0,  8},   // 地面半分
    {4, 1, 0.0f, 32, 32, 0,  0},   // 天井
    {5, 1, 0.0f, 32, 16, 0, -8},   // 天井半分
    {6, 1, 0.0f, 32, 32, 0,  0}    // ブロック
};

/*!
 @brief 障害物生成処理
 
 障害物を生成する。
 @param type 障害物種別
 @param position 生成位置
 @param parent 配置する親ノード
 */
void AKBlock::createBlock(int type, const cocos2d::CCPoint &position, cocos2d::CCNode *parent)
{
    AKLog(kAKLogBlock_1, "障害物生成");
    
    // パラメータの内容をメンバに設定する
    m_position = position;
        
    // 配置フラグを立てる
    m_isStaged = true;

    // ヒットポイントは1とする
    m_hitPoint = 1;
        
    AKAssert(type > 0 && type <= kAKBlockDefCount, "障害物種別の値が範囲外:type=%d", type);
        
    // 画像名を作成する
    char imageName[16] = "";
    snprintf(imageName, sizeof(imageName), kAKImageNameFormat, kAKBlockDef[type - 1].image);
    setImageName(imageName);
    
    // アニメーションフレームの個数を設定する
    m_animationPattern = kAKBlockDef[type - 1].animationFrame;
    
    // アニメーションフレーム間隔を設定する
    m_animationInterval = kAKBlockDef[type - 1].animationInterval;
    
    // 当たり判定のサイズを設定する
    m_size.width = kAKBlockDef[type - 1].hitWidth;
    m_size.height = kAKBlockDef[type - 1].hitHeight;
    
    // 画像表示位置オフセットを設定する
    m_offset = cocos2d::CCPointMake(kAKBlockDef[type - 1].offsetX,
                                    kAKBlockDef[type - 1].offsetY);
    
    // 画像のオフセットと逆方向にキャラクター位置を移動する
    m_position.x -= m_offset.x;
    m_position.y -= m_offset.y;
    
    // 障害物は基本的に画面スクロールに応じて移動する
    m_scrollSpeed = 1.0f;
        
    // レイヤーに配置する
    parent->addChild(getImage());
}

/*!
 @brief 衝突処理
 
 衝突した時の処理、
 @param character 衝突した相手
 @param data ゲームデータ
 */
void AKBlock::hit(AKCharacter *character, AKPlayDataInterface *data)
{
    // 相手の障害物衝突時の処理によって処理内容を分岐する
    switch (character->getBlockHitAction()) {
        case kAKBlockHitNone:       // 無処理
            break;
            
        case kAKBlockHitMove:       // 移動
        case kAKBlockHitPlayer:     // 自機
            pushCharacter(character, data);
            break;
            
        case kAKBlockHitDisappear:  // 消滅
            destroyCharacter(character);
            break;
            
        default:
            break;
    }
}

/*!
 @brief ぶつかったキャラクターを押し動かす
 
 ぶつかったキャラクターを押し動かす。
 移動先の座標は以下の優先度で設定し、衝突判定で問題なかった時点で採用する。
 @param character 衝突した相手
 @param data ゲームデータ
 */
void AKBlock::pushCharacter(AKCharacter *character, AKPlayDataInterface *data)
{
    // 障害物左端への移動
    float leftX = m_position.x - (m_size.width + character->getSize()->width) / 2;
    // 障害物右端への移動
    float rightX = m_position.x + (m_size.width + character->getSize()->width) / 2;
    // 障害物下端への移動
    float bottomY = m_position.y - (m_size.height + character->getSize()->height) / 2;
    // 障害物上端への移動
    float topY = m_position.y + (m_size.height + character->getSize()->height) / 2;
    // 移動先座標配列
    float movePosX[4];
    float movePosY[4];
    
    // スクロール速度を取得する
    float scrollX = data->getScrollSpeedX();
    float scrollY = data->getScrollSpeedY();
    
    // 移動の優先度は
    //   1.スクロール方向
    //   2.スクロールと垂直方向の近い方
    //   3.スクロールと垂直方向の遠い方
    //   4.スクロールと反対方向
    
    // x方向への移動の場合
    if (scrollX * scrollX > scrollY * scrollY) {
        
        // 左方向への移動の場合
        if (scrollX > 0) {
            movePosX[0] = leftX;
            movePosY[0] = character->getPosition()->y;
            movePosX[3] = rightX;
            movePosY[3] = character->getPosition()->y;
        }
        // 右方向への移動の場合
        else {
            movePosX[0] = rightX;
            movePosY[0] = character->getPosition()->y;
            movePosX[3] = leftX;
            movePosY[3] = character->getPosition()->y;
        }
        
        // 相手が障害物より下にある場合
        if (m_position.y > character->getPosition()->y) {
            movePosX[1] = character->getPosition()->x;
            movePosY[1] = bottomY;
            movePosX[2] = character->getPosition()->x;
            movePosY[2] = topY;
        }
        // 相手が障害物より上にある場合
        else
        {
            movePosX[1] = character->getPosition()->x;
            movePosY[1] = topY;
            movePosX[2] = character->getPosition()->x;
            movePosY[2] = bottomY;
        }
    }
    // y方向への移動の場合
    else {
        
        // 上方向への移動の場合
        if (scrollY > 0) {
            movePosX[0] = character->getPosition()->x;
            movePosY[0] = bottomY;
            movePosX[3] = character->getPosition()->x;
            movePosY[3] = topY;
        }
        // 下方向への移動の場合
        else {
            movePosX[0] = character->getPosition()->x;
            movePosY[0] = topY;
            movePosX[3] = character->getPosition()->x;
            movePosY[3] = bottomY;
        }
        
        // 相手が障害物より左にある場合
        if (m_position.x > character->getPosition()->x) {
            movePosX[1] = leftX;
            movePosY[1] = character->getPosition()->y;
            movePosX[2] = rightX;
            movePosY[2] = character->getPosition()->y;
        }
        // 相手が障害物より上にある場合
        else
        {
            movePosX[1] = rightX;
            movePosY[1] = character->getPosition()->y;
            movePosX[2] = leftX;
            movePosY[2] = character->getPosition()->y;
        }
    }
    
    // 移動前の位置を記憶する
    character->savePosition();
    
    // 4方向へ移動を試みる
    for (int i = 0; i < 4; i++) {
        
        // 位置を変更する
        character->setPosition(CCPoint(movePosX[i], movePosY[i]));
        
        // 自機の場合は画面外への押し出しは禁止する
        if (character->getBlockHitAction() == kAKBlockHitPlayer) {
            
            if (character->getPosition()->x < 0.0f ||
                character->getPosition()->x > AKScreenSize::stageSize().width ||
                character->getPosition()->y < 0.0f ||
                character->getPosition()->y > AKScreenSize::stageSize().height) {
                
                continue;
            }
        }
        
        // 衝突判定を行う
        if (!character->checkHitNoFunc(*data->getBlocks(), data)) {
            
            // 衝突しなかった場合はこの値を採用して処理を終了する
            return;
        }
    }
    
    // どの方向にも移動できなかったときは元に戻す
    character->restorePosition();
}

/*!
 @brief ぶつかったキャラクターを消す
 
 ぶつかったキャラクターを消す。相手のHPを0にする。
 @param character 衝突した相手
 */
void AKBlock::destroyCharacter(AKCharacter *character)
{
    character->setHitPoint(0);
}

/*!
 @brief キャラクター固有の動作
 
 キャラクター種別ごとの動作を行う。
 実数計算誤差によるタイル間の隙間の発生を防止するため、
 タイルマップの位置を基準として画像表示位置を設定し直す。
 @param data ゲームデータ
 */
void AKBlock::action(AKPlayDataInterface *data)
{
    // デバイススクリーン座標からマップ座標へ、マップ座標からタイルの座標へ変換する
    getImage()->setPosition(data->convertDevicePositionToTilePosition(getImage()->getPosition()));
}
