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
 @file AKTileMap.h
 @brief タイルマップ管理クラス
 
 ステージ構成定義のタイルマップファイルを読み込む。
 */

#ifndef AKTILEMAP_H
#define AKTILEMAP_H

#include "AKToritoma.h"
#include "AKPlayDataInterface.h"
#include "AKTileMapEventParameter.h"

class AKTileMap;

/*!
 @brief タイルマップ管理クラス
 
 ステージ構成定義のタイルマップファイルを読み込む。
 */
class AKTileMap {
public:
    /// イベント実行関数の型
    using AKExecFunc = void (AKTileMap::*)(const AKTileMapEventParameter &param, AKPlayDataInterface *data);
    
    // ステージと親ノードを指定したコンストラクタ
    AKTileMap(int stage, cocos2d::Node *parent);
    // デストラクタ
    ~AKTileMap();
    // 更新処理
    void update(AKPlayDataInterface *data);
    // デバイス座標からマップ座標の取得
    cocos2d::Vec2 getMapPositionFromDevicePosition(const cocos2d::Vec2 &devicePosition);
    // タイルの座標取得
    cocos2d::Vec2 getTilePositionFromMapPosition(const cocos2d::Vec2 &mapPosition);
    // ステージ進行度取得
    int getProgress();
    // ステージ進行度設定
    void setProgress(int progress);
    // クリアしたかどうか取得
    bool isClear();
    
private:
    /// タイルサイズ。ファイルから読み込むとContentScaleFactorを変えた時に数値が合わなくなるので、固定で持つ。
    static const int TileSize;
    /// タイルマップ
    cocos2d::TMXTiledMap *m_tileMap;
    /// 背景レイヤー
    cocos2d::TMXLayer *m_background;
    /// 前景レイヤー
    cocos2d::TMXLayer *m_foreground;
    /// 障害物レイヤー
    cocos2d::TMXLayer *m_block;
    /// イベントレイヤー
    cocos2d::TMXLayer *m_event;
    /// 敵レイヤー
    cocos2d::TMXLayer *m_enemy;
    /// 実行した列番号
    int m_currentCol;
    /// ステージ進行度
    int m_progress;
    /// クリアしたかどうか
    bool m_isClear;
    /// 進行待ちのイベント
    std::vector<cocos2d::ValueMap> m_waitEvents;
    
    // デフォルトコンストラクタは使用禁止にする
    AKTileMap();
    // 列単位のイベント実行
    void execEventByCol(int col, AKPlayDataInterface *data);
    // レイヤーごとのイベント実行
    void execEventLayer(cocos2d::TMXLayer *layer,
                        int col,
                        float x,
                        AKPlayDataInterface *data,
                        AKExecFunc execFunc);
    // 障害物作成
    void createBlock(const AKTileMapEventParameter &param, AKPlayDataInterface *data);
    // 敵作成
    void createEnemy(const AKTileMapEventParameter &param, AKPlayDataInterface *data);
    // イベント実行
    void execEvent(const AKTileMapEventParameter &param, AKPlayDataInterface *data);
};

#endif
