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
 @file AKTileMap.cpp
 @brief タイルマップ管理クラス
 
 ステージ構成定義のタイルマップファイルを読み込む。
 */

#include "AKTileMap.h"
#include "AKPlayingScene.h"

using cocos2d::Vector2;
using cocos2d::TMXTiledMap;
using cocos2d::TMXLayer;
using cocos2d::__String;
using cocos2d::Node;
using CocosDenshion::SimpleAudioEngine;
using std::vector;
using cocos2d::Value;
using cocos2d::ValueMap;

/// タイルマップのファイル名
static const char *kAKTileMapFileName = "Stage_%02d.tmx";

/*!
 @brief ステージと親ノードを指定したコンストラクタ
 
 ステージ番号に対応したタイルマップファイルを開き、指定された親ノードに配置する。
 @param stage ステージ番号
 @param parent 親ノード
 */
AKTileMap::AKTileMap(int stage, Node *parent) :
m_currentCol(0), m_progress(0), m_isClear(false)
{
    // ステージ番号からタイルマップのファイル名を決定する
    char fileName[16] = "";
    snprintf(fileName, sizeof(fileName), kAKTileMapFileName, stage);
    
    // タイルマップファイルを開く
    m_tileMap = TMXTiledMap::create(fileName);
    AKAssert(m_tileMap != NULL, "タイルマップ読み込みに失敗");
    m_tileMap->retain();
    
    // 各レイヤーを取得する
    m_background = m_tileMap->getLayer("Background");
    AKAssert(m_background != NULL, "背景レイヤーの取得に失敗");
    m_background->retain();
    
    m_foreground = m_tileMap->getLayer("Foreground");
    AKAssert(m_foreground != NULL, "前景レイヤーの取得に失敗");
    m_foreground->retain();
    
    m_block = m_tileMap->getLayer("Block");
    AKAssert(m_block != NULL, "障害物レイヤーの取得に失敗");
    m_block->retain();
    
    m_event = m_tileMap->getLayer("Event");
    AKAssert(m_event != NULL, "イベントレイヤーの取得に失敗");
    m_event->retain();

    m_enemy = m_tileMap->getLayer("Enemy");
    AKAssert(m_enemy != NULL, "敵レイヤーの取得に失敗");
    m_enemy->retain();
    
    // 背景・前景以外は非表示とする
    m_block->setVisible(false);
    m_enemy->setVisible(false);
    m_event->setVisible(false);
    
    // レイヤーに配置する
    parent->addChild(m_tileMap, 1);
    
    // 左端に初期位置を移動する
    float x = AKScreenSize::xOfStage(0.0f);
    float y = AKScreenSize::yOfStage(0.0f);
    m_tileMap->setPosition(Vector2(x, y));
}

/*!
 @brief デストラクタ
 
 メンバを解放する。
 */
AKTileMap::~AKTileMap()
{
    // メンバを解放する
    m_background->release();
    m_foreground->release();
    m_block->release();
    m_event->release();
    m_enemy->release();
    m_tileMap->release();
}

/*!
 @brief 更新処理
 
 マップをスクロールスピードに応じてスクロールする。
 現在のスクロール位置までイベントを実行する。
 現在処理済みの列から表示中の一番右側の列+2列分までのタイルのイベントを処理する。
 @param data ゲームデータ
 */
void AKTileMap::update(AKPlayDataInterface *data)
{
    // 背景をスクロールする
    float x = m_tileMap->getPosition().x - data->getScrollSpeedX();
    float y = m_tileMap->getPosition().y - data->getScrollSpeedY();
    m_tileMap->setPosition(Vector2(x, y));
    
    // 画面に表示されているタイルマップの右端の座標を計算する
    int right = AKScreenSize::stageSize().width - AKScreenSize::xOfDevice(m_tileMap->getPosition().x);

    // 右端のタイルの2個右の
    int maxCol = right / m_tileMap->getTileSize().width + 2;
    
    AKLog(kAKLogTileMap_2, "m_currentCol=%d maxCol=%d", m_currentCol, maxCol);
    
    // 現在処理済みの列から最終列まで処理する
    for (; m_currentCol <= maxCol; m_currentCol++) {
        
        execEventByCol(m_currentCol, data);
    }

    // 待機イベントを処理する
    vector<ValueMap>::iterator it = m_waitEvents.begin();
    while (it != m_waitEvents.end()) {
        
        // 実行する進行状況の値を取得する
        const std::string progressString = it->at("Progress").asString();
        int progress = std::stoi(progressString);
        
        // 進行度に到達している場合はイベントを実行する
        if (progress <= m_progress) {

            // パラメータを作成する
            AKTileMapEventParameter param(Vector2(0.0f, 0.0f), *it);

            // イベントを実行する
            execEvent(param, data);
            
            // 実行したイベントを待機イベントキューから取り除く
            m_waitEvents.erase(it);
            
            // 削除によってイテレータがずれるのでイテレータインクリメントは飛ばす
            continue;
        }
        
        // イテレータを次に進める
        it++;
    }
}

/*!
 @brief デバイス座標からマップ座標の取得
 
 デバイススクリーン座標からタイルマップ上の行列番号を取得する。
 左上を(0,0)として扱う。
 @param devicePosition デバイススクリーン座標
 @return マップ座標
 */
Vector2 AKTileMap::getMapPositionFromDevicePosition(const Vector2 &devicePosition)
{
    // タイルマップの左端からの距離をタイル幅で割った値を列番号とする
    int col = (devicePosition.x - m_tileMap->getPosition().x) / m_tileMap->getTileSize().width;
    
    // タイルマップの下端からの距離をタイル高さで割り、上下を反転させた値を行番号とする
    int row = m_tileMap->getMapSize().height -
        (devicePosition.y - m_tileMap->getPosition().y) / m_tileMap->getTileSize().height;
    
    return Vector2(col, row);
}

/*!
 @brief タイルの座標取得
 
 マップの行列番号からその位置のタイルのデバイススクリーン座標を取得する。
 タイルマップのデバイススクリーン座標の端数を四捨五入した上で計算を行う。
 @param mapPosition マップ座標
 @return タイルの座標
 */
Vector2 AKTileMap::getTilePositionFromMapPosition(const Vector2 &mapPosition)
{
    // x座標はマップの左端 + タイルサイズ * 列番号 (列番号は左から0,1,2,…)
    // タイルの真ん中を指定するために列番号には+0.5する
    int x = round(m_tileMap->getPosition().x) + m_tileMap->getTileSize().width * (mapPosition.x + 0.5);

    // y座標はマップの下端 + (マップの行数 - 行番号) * タイルサイズ (行番号は上から0,1,2…)
    // タイルの真ん中を指定するために行番号には+0.5する
    int y = round(m_tileMap->getPosition().y) +
        m_tileMap->getTileSize().height * (m_tileMap->getMapSize().height - (mapPosition.y + 0.5));
    
    return Vector2(x, y);
}

/*!
 @brief ステージ進行度取得
 
 ステージ進行度を取得する。
 @return ステージ進行度
 */
int AKTileMap::getProgress()
{
    return m_progress;
}

/*!
 @brief ステージ進行度設定
 
 ステージ進行度を設定する。
 @param progress ステージ進行度
 */
void AKTileMap::setProgress(int progress)
{
    m_progress = progress;
}

/*!
 @brief クリアしたかどうか取得
 
 クリアしたかどうかを取得する。
 @return クリアしたかどうか
 */
bool AKTileMap::isClear()
{
    return m_isClear;
}

/*!
 @brief 列単位のイベント実行
 
 指定した列番号のタイルのイベントを実行する。
 @param col 列番号
 @param data ゲームデータ
 */
void AKTileMap::execEventByCol(int col, AKPlayDataInterface *data)
{
    // x座標はマップの左端 + タイルサイズ * 列番号 (列番号は左から0,1,2,…)
    // タイルの真ん中を指定するために列番号には+0.5する
    float x = AKScreenSize::xOfDevice(m_tileMap->getPosition().x) +
                m_tileMap->getTileSize().width * (col + 0.5);
    
    AKLog(kAKLogTileMap_1, "position.x=%f, xOfDevice=%f, width=%f",
          m_tileMap->getPosition().x,
          AKScreenSize::xOfDevice(m_tileMap->getPosition().x),
          m_tileMap->getTileSize().width);
    AKLog(kAKLogTileMap_1, "col=%d, x=%f", col, x);
    
    // イベントレイヤーの処理を行う
    execEventLayer(m_event, col, x, data, &AKTileMap::execEvent);
    
    // 障害物レイヤーの処理を行う
    execEventLayer(m_block, col, x, data, &AKTileMap::createBlock);
    
    // 敵レイヤーの処理を行う
    execEventLayer(m_enemy, col, x, data, &AKTileMap::createEnemy);
}

/*!
 @brief レイヤーごとのイベント実行
 
 レイヤーごとにイベントを実行する。
 指定されたレイヤーの1列分のイベントを実行する。
 @param layer レイヤー
 @param col 列番号
 @param x x座標
 @param data ゲームデータ
 @param execFunc イベント処理関数
 */
void AKTileMap::execEventLayer(TMXLayer *layer,
                               int col,
                               float x,
                               AKPlayDataInterface *data,
                               AKExecFunc execFunc)
{
    // レイヤーの一番上の行から一番下の行まで処理を行う
    for (int i = 0; i < m_tileMap->getMapSize().height; i++) {
        
        // 処理対象のタイルの座標を作成する
        Vector2 tilePos(col, i);
        
        // タイルのGIDを取得する
        int tileGid = layer->getTileGIDAt(tilePos);
        
        AKLog(kAKLogTileMap_2, "i=%d tileGid=%d", i, tileGid);
        
        // タイルが存在する場合
        if (tileGid > 0) {
            
            // プロパティを取得する
            Value value = m_tileMap->getPropertiesForGID(tileGid);
            if (value.isNull()) {
                continue;
            }
            
            ValueMap properties = value.asValueMap();
            
            // y座標はマップの下端 + (マップの行数 - 行番号) * タイルサイズ (行番号は上から0,1,2…)
            // タイルの真ん中を指定するために行番号には+0.5する
            float y = AKScreenSize::yOfDevice(m_tileMap->getPosition().y) +
                (m_tileMap->getMapSize().height - (i + 0.5)) * m_tileMap->getTileSize().height;
                
            // パラメータを作成する
            AKTileMapEventParameter param(Vector2(x, y), properties);
                
            // イベントを実行する
            (this->*execFunc)(param, data);
        }
    }
}

/*!
 @brief 障害物作成
 
 障害物を作成する。
 障害物レイヤーのプロパティから以下の項目を取得し、障害物の作成を行う。
 Type:障害物の種別
 @param param イベント実行パラメータ
 @param data ゲームデータ
 */
void AKTileMap::createBlock(const AKTileMapEventParameter &param, AKPlayDataInterface *data)
{
    // 種別を取得する
    const std::string typeString = param.getProperties()->at("Type").asString();
    int type = std::stoi(typeString);
    
    // 障害物を作成する
    data->createBlock(type, *param.getPosition());
}

/*!
 @brief 敵作成
 
 敵を作成する。
 障害物レイヤーのプロパティから以下の項目を取得し、敵の作成を行う。
 Type:敵の種別
 Progress:倒した時に進む進行度
 @param param イベント実行パラメータ
 @param data ゲームデータ
 */
void AKTileMap::createEnemy(const AKTileMapEventParameter &param, AKPlayDataInterface *data)
{
    // 種別を取得する
    const std::string typeString = param.getProperties()->at("Type").asString();
    int type = std::stoi(typeString);
    
    // 倒した時に進む進行度を取得する
    const std::string progressString = param.getProperties()->at("Progress").asString();
    int progress = std::stoi(progressString);
    
    AKLog(kAKLogTileMap_1, "type=%d position=(%f, %f) progress=%d", type, param.getPosition()->x, param.getPosition()->y, progress);

    // 敵を作成する
    data->createEnemy(type, *param.getPosition(), progress);
}

/*!
 @brief イベント実行
 
 イベントを実行する。
 イベントレイヤーのプロパティから以下の項目を取得し、イベントを実行する。
 Type:イベントの種類
 Value:イベント実行で使用する値
 Progress:ステージ進行度がこの値以上のときにイベント実行する
 
 イベントの種類は以下のとおり、
 bgm:BGMを変更する
 hspeed:水平方向のスクロールスピードを変更する
 clear:ステージクリアのフラグを立てる
 @param param イベント実行パラメータ
 @param data ゲームデータ
 */
void AKTileMap::execEvent(const AKTileMapEventParameter &param, AKPlayDataInterface *data)
{
    // 実行する進行状況の値を取得する
    const std::string progressString = param.getProperties()->at("Progress").asString();
    int progress = std::stoi(progressString);
    
    AKLog(kAKLogTileMap_1 && progress > 0, "progress=%d m_progress=%d", progress, m_progress);
    
    // 実行する進行度に到達していない場合は待機イベントの配列に入れて処理を終了する
    if (progress > m_progress) {
        
        // 座標が0以上の場合はマップからの呼び出しと判断する。
        // マップ読み込みからの実行時は待機イベント配列へ入れる。
        // 座標が0の場合は待機イベントの実行なので無処理とする。
        if (!(param.getPosition()->x < 0.0f && param.getPosition()->y < 0.0f)) {
            
            m_waitEvents.push_back(*param.getProperties());
        }
        
        return;
    }
    
    // 種別を取得する
    const std::string type = param.getProperties()->at("Type").asString();
    
    // 値を取得する
    const std::string valueString = param.getProperties()->at("Value").asString();
    int value = std::stoi(valueString);
    
    // 水平方向のスクロールスピード変更の場合
    if (type.compare("hspeed") == 0) {
        // スピードは0.1単位で指定するものとする
        data->setScrollSpeedX(value / 10.0f);
    }
    // BGM変更の場合
    else if (type.compare("bgm") == 0) {

        // ファイル名を作成する
        char fileName[32] = "";
        snprintf(fileName, sizeof(fileName), kAKStageBGMFileName, value);
        
        AKLog(kAKLogTileMap_1, "BGM:%.32sを再生", fileName);

        // BGMを再生する
        SimpleAudioEngine::getInstance()->playBackgroundMusic(fileName, true);
    }
    // ステージクリアの場合
    else if (type.compare("clear") == 0) {
        
        AKLog(kAKLogTileMap_1, "progress=%d m_progress=%d", progress, m_progress);
        AKLog(kAKLogTileMap_1, "stage clear.");

        // ステージクリアフラグを立てる
        m_isClear = true;
    }
    // 不明な種別の場合
    else {
        AKAssert(false, "不明な種別:%s", type.c_str());
    }
}
