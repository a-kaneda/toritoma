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
 @file AKCharacter.h
 @brief キャラクタークラス定義
 
 当たり判定を持つオブジェクトの基本クラスを定義する。
 */

#ifndef AKCHARACTER_H
#define AKCHARACTER_H

#include "AKToritoma.h"
#include "AKPlayDataInterface.h"

/// 障害物と衝突した時の動作
enum AKBlockHitAction {
    kAKBlockHitNone = 0,    ///< 無処理
    kAKBlockHitMove,        ///< 移動
    kAKBlockHitDisappear,   ///< 消滅
    kAKBlockHitPlayer,      ///< 自機
};

/// 障害物と接している面
enum AKBlockHitSide {
    kAKHitSideLeft = 1,     ///< 左側
    kAKHitSideRight = 2,    ///< 右側
    kAKHitSideTop = 4,      ///< 上側
    kAKHitSideBottom = 8    ///< 下側
};

/*!
 @brief キャラクタークラス
 
 当たり判定を持つオブジェクトの基本クラス。
 */
class AKCharacter {
public:
    /// 衝突処理関数の型
    using AKHitFunc = void (AKCharacter::*)(AKCharacter *character, AKPlayDataInterface *data);
    
private:
    /// 画像
    cocos2d::CCSprite *m_image;
    /// 画像名
    std::string m_imageName;
    /// アニメーション初期パターン
    int m_animationInitPattern;
    
protected:
    /// 当たり判定サイズ幅
    cocos2d::CCSize m_size;
    /// 位置
    cocos2d::CCPoint m_position;
    /// 移動の位置
    cocos2d::CCPoint m_prevPosition;
    /// 速度x方向
    float m_speedX;
    /// 速度y方向
    float m_speedY;
    /// HP
    int m_hitPoint;
    /// 攻撃力
    int m_power;
    /// ステージ上に存在しているかどうか
    bool m_isStaged;
    /// アニメーションパターン数
    int m_animationPattern;
    /// アニメーション間隔
    int m_animationInterval;
    /// アニメーションフレーム数
    int m_animationFrame;
    /// アニメーション繰り返し回数
    int m_animationRepeat;
    /// スクロール速度の影響を受ける割合
    float m_scrollSpeed;
    /// 障害物と衝突した時の動作
    enum AKBlockHitAction m_blockHitAction;
    /// 障害物と接している面
    unsigned int m_blockHitSide;
    /// 画像表示のオフセット
    cocos2d::CCPoint m_offset;
    
public:
    // コンストラクタ
    AKCharacter();
    // デストラクタ
    ~AKCharacter();
    
public:
    // 位置取得
    const cocos2d::CCPoint* getPosition();
    // 位置設定
    void setPosition(const cocos2d::CCPoint &position);
    // 前回位置取得
    const cocos2d::CCPoint* getPrevPosition();
    // サイズ取得
    const cocos2d::CCSize* getSize();
    // x方向の速度取得
    float getSpeedX();
    // y方向の速度取得
    float getSpeedY();
    // 画像名の取得
    const std::string* getImageName();
    // アニメーションパターン数取得
    int getAnimationPattern();
    // アニメーション間隔取得
    int getAnimationInterval();
    // 障害物と衝突した時の動作の取得
    enum AKBlockHitAction getBlockHitAction();
    // ステージ上に存在しているかどうか取得
    bool isStaged();
    // ヒットポイント取得
    int getHitPoint();
    // ヒットポイント設定
    void setHitPoint(int hitPoint);
    // 攻撃力取得
    int getPower();
    // 現在の位置を記憶する
    void savePosition();
    // 記憶した位置を復元する
    void restorePosition();
    // 移動処理
    virtual void move(AKPlayDataInterface *data);
    // 画像の取得
    cocos2d::CCSprite* getImage();
    // 画像有無チェック
    bool hasImage();
    
    /*!
     @brief 衝突判定(動作なし)
     
     衝突判定のみを行い、衝突処理を行わない。
     @param characters 判定対象のキャラクター群
     @param data ゲームデータ
     @return 衝突したかどうか
     */
    template<typename T>
    bool checkHitNoFunc(const std::vector<T*> &characters, AKPlayDataInterface *data)
    {
        return checkHit(characters, data, NULL);
    }
    
    /*!
     @brief キャラクター衝突判定
     
     キャラクターが衝突しているか調べ、衝突しているときはHPを減らす。
     @param characters 判定対象のキャラクター群
     @param data ゲームデータ
     */
    template<typename T>
    bool checkHit(const std::vector<T*> &characters, AKPlayDataInterface *data)
    {
        return checkHit(characters, data, &AKCharacter::hit);
    }
    
    
protected:
    // 画像名の設定
    void setImageName(const std::string &imageName);
    // アニメーション初期パターンの設定
    void setAnimationInitPattern(int animationPattern);
    // キャラクター固有の動作
    virtual void action(AKPlayDataInterface *data);
    // 破壊処理
    virtual void destroy(AKPlayDataInterface *data);
    // 衝突処理
    virtual void hit(AKCharacter *character, AKPlayDataInterface *data);
    // 障害物との衝突による移動
    void moveOfBlockHit(AKCharacter *character, AKPlayDataInterface *data);
    // 障害物との衝突による消滅
    void disappearOfBlockHit(AKCharacter *character, AKPlayDataInterface *data);
    // 画面外配置判定
    bool isOutOfStage(AKPlayDataInterface *data);
    // 画像表示位置更新
    void updateImagePosition();
    
    /*!
     @brief 衝突判定(汎用)
     
     衝突判定を行う。衝突時にどのような処理を行うかをパラメータで指定する。
     @param characters 判定対象のキャラクター群
     @param data ゲームデータ
     @param func 衝突時処理
     @return 衝突したかどうか
     */
    template<typename T>
    bool checkHit(const std::vector<T*> &characters, AKPlayDataInterface *data, AKHitFunc func)
    {
        // 画面に配置されていない場合は処理しない
        if (!m_isStaged) {
            return false;
        }
        
        // 自キャラの上下左右の端を計算する
        float myleft = m_position.x - m_size.width / 2.0f;
        float myright = m_position.x + m_size.width / 2.0f;
        float mytop = m_position.y + m_size.height / 2.0f;
        float mybottom = m_position.y - m_size.height / 2.0f;
        
        // 衝突したかどうかを記憶する
        bool isHit = false;
        
        // 衝突している方向を初期化する
        m_blockHitSide = 0;
        
        // 判定対象のキャラクターごとに判定を行う
        for (T *target : characters) {
            
            // 相手が画面に配置されていない場合は処理しない
            if (!target->isStaged()) {
                continue;
            }
            
            // 相手の上下左右の端を計算する
            float targetleft = target->getPosition()->x - target->getSize()->width / 2.0f;
            float targetright = target->getPosition()->x + target->getSize()->width / 2.0f;
            float targettop = target->getPosition()->y + target->getSize()->height / 2.0f;
            float targetbottom = target->getPosition()->y - target->getSize()->height / 2.0f;
            
            // 以下のすべての条件を満たしている時、衝突していると判断する。
            //   ・相手の右端が自キャラの左端よりも右側にある
            //   ・相手の左端が自キャラの右端よりも左側にある
            //   ・相手の上端が自キャラの下端よりも上側にある
            //   ・相手の下端が自キャラの上端よりも下側にある
            if ((targetright > myleft) &&
                (targetleft < myright) &&
                (targettop > mybottom) &&
                (targetbottom < mytop)) {
                
                // 衝突処理を行う
                if (func != NULL) {
                    (this->*func)(target, data);
                }
                
                // 衝突したかどうかを記憶する
                isHit = true;
            }
        }
        
        // 衝突したかどうかを返す
        return isHit;
    }
};

#endif