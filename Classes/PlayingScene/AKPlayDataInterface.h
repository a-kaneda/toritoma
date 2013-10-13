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
 @file AKPlayDataInterface.h
 @brief ゲームデータインターフェースプロトコル
 
 キャラクターやタイルマップのイベント処理がゲームデータにアクセスする
 インターフェースのプロトコルを定義する。
 */

#ifndef AKPLAYDATAINTERFACE_H
#define AKPLAYDATAINTERFACE_H

#include "AKToritoma.h"

class AKBlock;
class AKEnemyShot;
class AKEnemy;

/*!
 @brief ゲームデータインターフェース
 
 キャラクターやタイルマップのイベント処理がゲームデータにアクセスする
 インターフェース。
 */
class AKPlayDataInterface {
public:
    /*!
     @brief x軸方向のスクロールスピード取得
     
     x軸方向のスクロールスピードを取得する。
     @return x軸方向のスクロールスピード
     */
    virtual float getScrollSpeedX() = 0;
    
    /*!
     @brief x軸方向のスクロールスピード設定
     
     x軸方向のスクロールスピードを設定する。
     @param x軸方向のスクロールスピード
     */
    virtual void setScrollSpeedX(float speed) = 0;

    /*!
     @briief y軸方向のスクロールスピード取得
     
     y軸方向のスクロールスピードを取得する。
     @return y軸方向のスクロールスピード
     */
    virtual float getScrollSpeedY() = 0;

    /*!
     @brief y軸方向のスクロールスピード設定
     
     y軸方向のスクロールスピードを設定する。
     @param y軸方向のスクロールスピード
     */
    virtual void setScrollSpeedY(float speed) = 0;

    /*!
     @brief 障害物キャラクター取得
     
     障害物キャラクターの配列を取得する。
     @return 障害物キャラクター
     */
    virtual const std::vector<AKBlock*>* getBlocks() = 0;

    /*!
     @brief 自機の位置情報
     
     自機の位置情報を取得する。
     @return 自機の位置情報
     */
    virtual const cocos2d::CCPoint* getPlayerPosition() = 0;

    /*!
     @brief デバイス座標からタイル座標の取得
     
     デバイススクリーン座標からマップ座標へ、マップ座標からタイルの座標へ変換する。
     @param devicePosition デバイススクリーン座標
     @return タイルの座標
     */
    virtual cocos2d::CCPoint convertDevicePositionToTilePosition(cocos2d::CCPoint devicePosition) = 0;

    /*!
     @brief 自機弾生成
 
     自機弾を生成する。
     @param position 生成位置
     */
    virtual void createPlayerShot(cocos2d::CCPoint position) = 0;

    /*!
     @brief 反射弾生成
 
     反射弾を生成する。
     @param enemyShot 反射する敵弾
     */
    virtual void createReflectShot(AKEnemyShot *enemyShot) = 0;

    /*!
     @brief 敵生成
 
     敵キャラを生成する。
     @param type 敵種別
     @param position 生成位置
     @param progress 倒した時に進む進行度
     @return 生成した敵キャラ
     */
    virtual AKEnemy* createEnemy(int type, cocos2d::CCPoint position, int progress) = 0;

    /*!
     @brief 敵弾インスタンスの取得
 
     敵弾プールから次のインスタンスを取得する。
     @return 敵弾インスタンス
     */
    virtual AKEnemyShot* getEnemyShot() = 0;

    /*!
     @brief 敵弾配置ノードの取得
 
     敵弾を配置するノードを取得する。
     @return 敵弾配置ノード
     */
    virtual cocos2d::CCNode* getEnemyShotParent() = 0;

    /*!
     @brief 画面効果生成
 
     画面効果を生成する。
     @param type 画面効果種別
     @param position 生成位置
     */
    virtual void createEffect(int type, cocos2d::CCPoint position) = 0;

    /*!
     @brief 障害物生成
 
     障害物を生成する。
     @param type 障害物種別
     @param position 生成位置
     */
    virtual void createBlock(int type, cocos2d::CCPoint position) = 0;

    /*!
     @brief 失敗時処理
 
     失敗した時の処理を行う。
     */
    virtual void miss() = 0;

    /*!
     @brief スコア加算
 
     スコアを加算する。ハイスコアを超えている場合はハイスコアも合わせて更新する。
     スコア、ハイスコアの表示を更新する。
     @param score スコア増加量
     */
    virtual void addScore(int score) = 0;

    /*!
     @brief 進行度を進める
 
     進行度を進める。
     @param progress 進行度
     */
    virtual void addProgress(int progress) = 0;
};

#endif
