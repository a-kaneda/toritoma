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
 @file AKPlayData.m
 @brief ゲームデータ
 
 プレイ画面シーンのゲームデータを管理するクラスを定義する。
 */

#include "AKPlayData.h"
#include "AKPlayingScene.h"
#include "AKPlayerShot.h"
#include "AKEnemy.h"
#include "AKEffect.h"
#include "AKBlock.h"
#include "AKHiScoreFile.h"
#include "string.h"

using cocos2d::CCNode;
using cocos2d::CCPoint;
using cocos2d::CCSpriteFrameCache;
using cocos2d::CCSpriteBatchNode;

/// ステージの数
const int kAKStageCount = 6;

/// 自機初期位置x座標
static const float kAKPlayerDefaultPosX = 50.0f;
/// 自機初期位置y座標
static const float kAKPlayerDefaultPosY = 128.0f;
/// 自機弾の同時出現最大数
static const int kAKMaxPlayerShotCount = 128;
/// 敵キャラの同時出現最大数
static const int kAKMaxEnemyCount = 32;
/// 敵弾の同時出現最大数
static const int kAKMaxEnemyShotCount = 512;
/// 画面効果の同時出現最大数
static const int kAKMaxEffectCount = 64;
/// 障害物の同時出現最大数
static const int kAKMaxBlockCount = 128;
/// キャラクターテクスチャアトラス定義ファイル名
const char *kAKTextureAtlasDefFile = "Character.plist";
/// キャラクターテクスチャアトラスファイル名
const char *kAKTextureAtlasFile = "Character.png";
// TODO:ハイスコア処理作成
/// ハイスコアファイル名
//static const char *kAKDataFileName = "hiscore.dat";
/// ハイスコアファイルのエンコードキー名
//static const char *kAKDataFileKey = "hiScoreData";
// TODO:ステージクリア処理作成
/// ステージクリア後の待機時間
//static const float kAKStageClearWaitTime = 5.0f;
/// 初期残機
static const int kAKInitialLife = 2;
/// 自機復活待機フレーム数
static const int kAKRebirthInterval = 60;
/// エクステンドするスコア
static const int kAKExtendScore = 50000;
/// クリアした後の待機時間
static const int kAKClearWait = 540;

/// ゲームクリア時のツイートのフォーマットのキー
static const char *kAKGameClearTweetKey = "GameClearTweet";
/// ゲームオーバー時のツイートのフォーマットのキー
static const char *kAKGameOverTweetKey = "GameOverTweet";

/// キャラクター配置のz座標
enum AKCharacterPositionZ {
    kAKCharaPosZBlock = 0,  ///< 障害物
    kAKCharaPosZPlayerShot, ///< 自機弾
    kAKCharaPosZOption,     ///< オプション
    kAKCharaPosZPlayer,     ///< 自機
    kAKCharaPosZEnemy,      ///< 敵
    kAKCharaPosZEffect,     ///< 爆発効果
    kAKCharaPosZEnemyShot,  ///< 敵弾
    kAKCharaPosZCount       ///< z座標種別の数
};

#pragma mark オブジェクト初期化

/*!
 @brief シーンを指定したコンストラクタ
 
 シーンをメンバに設定する。
 テクスチャアトラスを読み込む。
 メンバの初期化を行う。
 @param scene プレイシーン
 */
AKPlayData::AKPlayData(AKPlayingScene *scene) :
m_scene(scene), m_playerShotPool(kAKMaxPlayerShotCount),
m_reflectShotPool(kAKMaxEnemyShotCount), m_enemyPool(kAKMaxEnemyCount),
m_enemyShotPool(kAKMaxEnemyShotCount), m_effectPool(kAKMaxEffectCount),
m_blockPool(kAKMaxBlockCount), m_tileMap(NULL), m_player(NULL)
{
    // シーンを確保する
    m_scene->retain();
    
    // テクスチャアトラスを読み込む
    CCSpriteFrameCache *spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    spriteFrameCache->addSpriteFramesWithFile(kAKTextureAtlasDefFile,
                                              kAKTextureAtlasFile);
    
    // メンバオブジェクトを生成する
    createMember();
    
    // ゲームデータを初期化する
    clearPlayData();
}

/*!
 @brief デストラクタ

 メンバを解放する。
 */
AKPlayData::~AKPlayData()
{
    // メンバを解放する
    m_scene->release();
    delete m_player;
    delete m_tileMap;
    for (CCNode *node : m_batches) {
        node->removeFromParentAndCleanup(true);
    }
}

/*!
 @brief メンバオブジェクト生成処理
 
 メンバオブジェクトを生成する。
 */
void AKPlayData::createMember()
{
    // 各z座標用にバッチノードを作成する
    for (int i = 0; i < kAKCharaPosZCount; i++) {

        // バッチノードをファイルから作成する
        CCSpriteBatchNode *batch = CCSpriteBatchNode::create(kAKTextureAtlasFile);
        AKAssert(batch, "バッチノード作成に失敗:%s", kAKTextureAtlasFile);

        // 配列に保存する
        m_batches.push_back(batch);
        batch->retain();
        
        // シーンに配置する
        m_scene->getCharacterLayer()->addChild(batch, i);
    }

    // 自機を作成する
    m_player = new AKPlayer(m_batches.at(kAKCharaPosZPlayer),
                            m_batches.at(kAKCharaPosZOption));
}

/*!
 @brief x軸方向のスクロールスピード取得
 
 x軸方向のスクロールスピードを取得する。
 @return x軸方向のスクロールスピード
 */
float AKPlayData::getScrollSpeedX()
{
    return m_scrollSpeedX;
}

/*!
 @brief x軸方向のスクロールスピード設定
 
 x軸方向のスクロールスピードを設定する。
 @param speed x軸方向のスクロールスピード
 */
void AKPlayData::setScrollSpeedX(float speed)
{
    m_scrollSpeedX = speed;
}

/*!
 @brief y軸方向のスクロールスピード取得
 
 y軸方向のスクロールスピードを取得する。
 @return y軸方向のスクロールスピード
 */
float AKPlayData::getScrollSpeedY()
{
    return m_scrollSpeedY;
}

/*!
 @brief y軸方向のスクロールスピード設定
 
 y軸方向のスクロールスピードを設定する。
 @param speed y軸方向のスクロールスピード
 */
void AKPlayData::setScrollSpeedY(float speed)
{
    m_scrollSpeedY = speed;
}

/*!
 @brief 初期値設定処理
 
 ゲームデータに初期値を設定する。
 */
void AKPlayData::clearPlayData()
{
    // 自機の初期位置を設定する
    m_player->setPosition(CCPoint(kAKPlayerDefaultPosX, kAKPlayerDefaultPosY), this);
    
    // シールドは無効状態で初期化する
    m_shield = false;
    
    // スクロールスピード・位置は0で初期化する
    m_scrollSpeedX = 0.0f;
    m_scrollSpeedY = 0.0f;
    
    // 残機の初期値を設定する
    setLife(kAKInitialLife);
    
    // ハイスコアをファイルから読み込む
    readHiScore();
    
    // その他のメンバを初期化する
    m_stage = 0;
    m_score = 0;
    m_clearWait = 0;
    m_rebirthWait = 0;
}

#pragma mark アクセサ

/*!
 @brief 自機の位置情報取得
 
 自機の位置情報を取得する。
 @return 自機の位置情報
 */
const CCPoint* AKPlayData::getPlayerPosition()
{
    return m_player->getPosition();
}

/*!
 @brief 障害物キャラクター取得
 
 障害物キャラクターを取得する。
 @return 障害物キャラクター
 */
const std::vector<AKBlock*>* AKPlayData::getBlocks()
{
    return m_blockPool.getPool();
}

/*!
 @brief 残機設定
 
 残機を設定する。
 画面の残機表示の更新も行う。
 @param life 残機
 */
void AKPlayData::setLife(int life)
{
    // メンバに設定する
    m_life = life;
    
    // シーンの残機表示の更新を行う
    m_scene->getLife()->setLifeCount(life);
}

/*!
 @brief シールドモード設定
 
 シールドモードの有効無効を設定する。
 画面のシールドボタンの表示も切り替える。
 @param shield シールドモードが有効かどうか
 */
void AKPlayData::setShield(bool shield)
{
    // メンバに設定する
    m_shield = shield;
    
    // シーンのシールドボタンの表示を切り替える
    m_scene->setShieldButtonSelected(shield);
    
    // 自機・オプションに対してシールド有無を設定する
    // オプションは自機の処理の中で設定される
    m_player->setShield(shield);
}

#pragma mark ファイルアクセス

/*!
 @brief スクリプト読み込み
 
 スクリプトファイルを読み込む。
 @param stage ステージ番号
 */
void AKPlayData::readScript(int stage)
{
    // ステージ番号をメンバに設定する
    m_stage = stage;
    
    // スクリプトファイルを読み込む
    delete m_tileMap;
    m_tileMap = new AKTileMap(stage, m_scene->getBackgroundLayer());
    
    // 初期表示の1画面分の処理を行う
    m_tileMap->update(this);
}

/*!
 @brief ハイスコアファイル読込
 
 ハイスコアファイルを読み込む。
 */
void AKPlayData::readHiScore()
{
    AKLog(kAKLogPlayData_1, "start");
    
    /* TODO:ファイル入出力処理を作成する 
    // HOMEディレクトリのパスを取得する
    const char *homeDir = NSHomeDirectory();
    
    // Documentsディレクトリへのパスを作成する
    const char *docDir = [homeDir stringByAppendingPathComponent:"Documents"];
    
    // ファイルパスを作成する
    const char *filePath = [docDir stringByAppendingPathComponent:kAKDataFileName];
    
    // ファイルを読み込む
    NSData *data = [NSData dataWithContentsOfFile:filePath];
    
    AKLog(kAKLogPlayData_1 && data == NULL, "ファイル読み込み失敗");
    
    // ファイルが読み込めた場合はデータを取得する
    if (data != NULL) {
        
        // ファイルからデコーダーを生成する
        NSKeyedUnarchiver *decoder = [[[NSKeyedUnarchiver alloc] initForReadingWithData:data] autorelease];
        
        // ハイスコアをデコードする
        AKHiScoreFile *hiScore = [decoder decodeObjectForKey:kAKDataFileKey];
        
        // デコードを完了する
        [decoder finishDecoding];
        
        // メンバに読み込む
        hiScore_ = hiScore.hiscore;
        
        AKLog(kAKLogPlayData_1, "hiScore_=%d", hiScore_);
    }
    */
}

/*!
 @brief ハイスコアファイル書込
 
 ハイスコアファイルを書き込む。
 */
void AKPlayData::writeHiScore()
{
    AKLog(kAKLogPlayData_1, "start hiScore=%d", m_hiScore);
    
    /* TODO:ファイル入出力処理を作成する
    // HOMEディレクトリのパスを取得する
    const char *homeDir = NSHomeDirectory();
    
    // Documentsディレクトリへのパスを作成する
    const char *docDir = [homeDir stringByAppendingPathComponent:"Documents"];
    
    // ファイルパスを作成する
    const char *filePath = [docDir stringByAppendingPathComponent:kAKDataFileName];
    
    // ハイスコアファイルオブジェクトを生成する
    AKHiScoreFile *hiScore = [[[AKHiScoreFile alloc] init] autorelease];
    
    // ハイスコアを設定する
    hiScore.hiscore = hiScore_;
    
    // エンコーダーを生成する
    NSMutableData *data = [NSMutableData data];
    NSKeyedArchiver *encoder = [[[NSKeyedArchiver alloc] initForWritingWithMutableData:data] autorelease];
    
    // ファイル内容をエンコードする
    [encoder encodeObject:hiScore forKey:kAKDataFileKey];
    
    // エンコードを完了する
    [encoder finishEncoding];
    
    // ファイルを書き込む
    [data writeToFile:filePath atomically:YES];
    
    // Game Centerにスコアを送信する
    [[AKGameCenterHelper sharedHelper] reportHiScore:score_];
    */
}

#pragma mark シーンクラスからのデータ操作用

/*!
 @brief 状態更新
 
 各オブジェクトの状態を更新する。
 キャラクターの移動、衝突判定を行う。
 */
void AKPlayData::update()
{
    // クリア後の待機中の場合はステージクリア処理を行う
    if (m_clearWait > 0) {
        
        // 自機が破壊されている場合は復活するまで処理しない
        // 自機が存在する場合のみ待機時間のカウントとステージクリア処理を行う
        if (m_player->isStaged()) {
            
            // 待機時フレーム数をカウントする
            m_clearWait--;
            
            AKLog(kAKLogPlayData_2, "m_clearWait=%d", m_clearWait);
            
            // 待機フレーム数が経過した場合は次のステージへと進める
            if (m_clearWait <= 0) {
                
                // TODO:ステージクリアの実績をGame Centerへ送信する
                //[[AKGameCenterHelper sharedHelper] reportStageClear:stage_];
                
                // ステージを進める
                m_stage++;
                
                AKLog(kAKLogPlayData_1, "ステージクリア後の待機時間経過:m_stage=%d", m_stage);
                
                // すべてのステージをクリアしている場合はクリア処理を行う
                if (m_stage > kAKStageCount) {
                    // TODO:ゲームクリア処理を作成する
                }
                else {
                    // 次のステージのスクリプトを読み込む
                    readScript(m_stage);
                    
                    // 待機フレーム数をリセットする
                    m_clearWait = 0;

                    // プレイ中状態に戻す
                    m_scene->nextStage();
                }
            }
        }
    }
    
    // クリアしている場合はクリア時の処理を行う
    if (m_clearWait <= 0 && m_tileMap->isClear()) {
        
        AKLog(kAKLogPlayData_1, "ステージクリア");
        
        // 待機時間を設定する
        m_clearWait = kAKClearWait;

        // ステージクリア状態に遷移する
        m_scene->stageClear();
    }
    
    // 復活待機フレーム数が設定されている場合はフレーム数をカウントする
    if (m_rebirthWait > 0) {
        
        m_rebirthWait--;
        
        // 復活までのフレーム数が経過している場合は自機を復活する
        if (m_rebirthWait <= 0) {
            
            // 自機を復活させる
            m_player->rebirth();
        }
    }
    
    // マップを更新する
    m_tileMap->update(this);
    
    // 障害物を更新する
    for (AKBlock *block : *m_blockPool.getPool()) {
        if (block->isStaged()) {
            block->move(this);
        }
    }
    
    // 自機を更新する
    m_player->move(this);
    
    // 自機弾を更新する
    for (AKPlayerShot *playerShot : *m_playerShotPool.getPool()) {
        if (playerShot->isStaged()) {
            playerShot->move(this);
        }
    }
    
    // 反射弾を更新する
    for (AKEnemyShot *refrectedShot : *m_reflectShotPool.getPool()) {
        if (refrectedShot->isStaged()) {
            refrectedShot->move(this);
        }
    }
    
    // 敵を更新する
    for (AKEnemy *enemy : *m_enemyPool.getPool()) {
        if (enemy->isStaged()) {
            AKLog(kAKLogPlayData_3, "enemy move start.");
            enemy->move(this);
        }
    }
    
    // 敵弾を更新する
    for (AKEnemyShot *enemyShot : *m_enemyShotPool.getPool()) {
        if (enemyShot->isStaged()) {
            enemyShot->move(this);
        }
    }
    
    // 画面効果を更新する
    for (AKEffect *effect : *m_effectPool.getPool()) {
        if (effect->isStaged()) {
            effect->move(this);
        }
    }
    
    // 障害物の当たり判定を行う
    for (AKBlock *block : *m_blockPool.getPool()) {
        if (block->isStaged()) {
            
            // 自機との当たり判定を行う
            std::vector<AKPlayer*> playerArray;
            playerArray.push_back(m_player);
            block->checkHit(playerArray, this);
            
            // 自機弾との当たり判定を行う
            block->checkHit(*m_playerShotPool.getPool(), this);
            
            // 敵は移動処理の中で障害物との当たり判定を処理しているので
            // ここでは処理しない。
            
            // 敵弾との当たり判定を行う
            block->checkHit(*m_enemyShotPool.getPool(), this);
        }
    }
    
    // 敵と自機弾、反射弾の当たり判定を行う
    for (AKEnemy *enemy : *m_enemyPool.getPool()) {
        
        // 自機弾との当たり判定を行う
        enemy->checkHit(*m_playerShotPool.getPool(), this);
        
        // 反射弾との当たり判定を行う
        enemy->checkHit(*m_reflectShotPool.getPool(), this);
    }
    
    // シールド有効時、反射の判定を行う
    if (m_shield) {
        
        // オプションを取得する
        AKOption *option = m_player->getOption();
        
        // 各オプションに対して当たり判定を行う
        while (option != NULL && option->isStaged()) {
            
            AKLog(kAKLogPlayData_2, "反射判定");
            
            // 敵弾との当たり判定を行う
            option->checkHit(*m_enemyShotPool.getPool(), this);
            
            // 次のオプションを取得する
            option = option->getNext();
        }
    }
    
    // 自機が無敵状態でない場合は当たり判定処理を行う
    if (!m_player->isInvincible()) {
        
        // 自機と敵弾のかすり判定処理を行う
        m_player->graze(*m_enemyShotPool.getPool());
        
#if DEBUG_MODE_PLAYER_INVINCIBLE != 1
        
        // 自機と敵の当たり判定処理を行う
        m_player->checkHit(*m_enemyPool.getPool(), this);
        
        // 自機と敵弾の当たり判定処理を行う
        m_player->checkHit(*m_enemyShotPool.getPool(), this);
        
#endif
        
    }
    
    // シールドが有効な場合はチキンゲージを減少させる
    if (m_shield) {
        m_player->setChickenGauge(m_player->getChickenGauge() - 5);
        
        // チキンゲージがなくなった場合は強制的にシールドを無効にする
        if (m_player->getChickenGauge() < 0.0f) {
            m_player->setChickenGauge(0.0f);
            setShield(false);
        }
    }
    
    // チキンゲージの溜まっている比率を更新する
    m_scene->getChickenGauge()->setPercent(m_player->getChickenGaugePercent());
    
    // チキンゲージからオプション個数を決定する
    m_player->updateOptionCount();
}

/*!
 @brief 自機の移動
 
 自機を移動する。
 @param dx x座標の移動量
 @param dy y座標の移動量
 */
void AKPlayData::movePlayer(float dx, float dy)
{
    // 移動先の座標を設定する
    float x = AKRangeCheckF(m_player->getPosition()->x + dx,
                            0.0f,
                            AKScreenSize::stageSize().width);
    float y = AKRangeCheckF(m_player->getPosition()->y + dy,
                            0.0f,
                            AKScreenSize::stageSize().height);
    m_player->setPosition(CCPoint(x, y), this);
}

/*!
 @brief ツイートメッセージの作成
 
 ツイートメッセージを作成する。
 進行したステージ数と獲得したスコアから文字列を作成する。
 @return ツイートメッセージ
 */
std::string AKPlayData::makeTweet()
{
    char tweet[141] = "";
    
    // 全ステージクリアの場合と途中でゲームオーバーになった時でツイート内容を変更する。
    if (m_stage > kAKStageCount) {
        char format[141] = "";
        strncpy(format,
                CCLocalizedString(kAKGameClearTweetKey, "ゲームクリア時のツイート"),
                sizeof(format));
        snprintf(tweet, sizeof(tweet), format, m_score);
    }
    else {
        char format[141] = "";
        strncpy(format,
                CCLocalizedString(kAKGameOverTweetKey, "ゲームオーバー時のツイート"),
                sizeof(format));
        snprintf(tweet, sizeof(tweet), format, m_score);
    }
    
    // Twitterでは同じ内容のツイートは連続して投稿できない。
    // TODO:テスト時はこれを回避するため、末尾に現在日時を付加して同じ内容にならないようにする。
#ifdef DEBUG
//    return [const char stringWithFormat:"%@ %@", tweet, [[NSDate date] description]];
#endif

    return std::string(tweet);
}

/*!
 @brief ゲーム再開
 
 一時停止中の状態からゲームを再会する。
 */
void AKPlayData::resume()
{
    // すべてのキャラクターのアニメーションを再開する
    // 自機
    if (m_player->hasImage()) {
        m_player->getImage()->resumeSchedulerAndActions();
    }
    
    // 自機弾
    for (AKCharacter *character : *m_playerShotPool.getPool()) {
        if (character->hasImage()) {
            character->getImage()->resumeSchedulerAndActions();
        }
    }
    
    // 敵
    for (AKCharacter *character : *m_enemyPool.getPool()) {
        if (character->hasImage()) {
            character->getImage()->resumeSchedulerAndActions();
        }
    }
    
    // 敵弾
    for (AKCharacter *character : *m_enemyShotPool.getPool()) {
        if (character->hasImage()) {
            character->getImage()->resumeSchedulerAndActions();
        }
    }
    
    // 画面効果
    for (AKCharacter *character : *m_effectPool.getPool()) {
        if (character->hasImage()) {
            character->getImage()->resumeSchedulerAndActions();
        }
    }
}

/*!
 @brief ポーズ
 
 プレイ中の状態からゲームを一時停止する。
 */
void AKPlayData::pause()
{
    // すべてのキャラクターのアニメーションを停止する
    // 自機
    if (m_player->hasImage()) {
        m_player->getImage()->pauseSchedulerAndActions();
    }
    
    // 自機弾
    for (AKCharacter *character : *m_playerShotPool.getPool()) {
        if (character->hasImage()) {
            character->getImage()->pauseSchedulerAndActions();
        }
    }
    
    // 敵
    for (AKCharacter *character : *m_enemyPool.getPool()) {
        if (character->hasImage()) {
            character->getImage()->pauseSchedulerAndActions();
        }
    }
    
    // 敵弾
    for (AKCharacter *character : *m_enemyShotPool.getPool()) {
        if (character->hasImage()) {
            character->getImage()->pauseSchedulerAndActions();
        }
    }
    
    // 画面効果
    for (AKCharacter *character : *m_effectPool.getPool()) {
        if (character->hasImage()) {
            character->getImage()->pauseSchedulerAndActions();
        }
    }
}

#pragma mark キャラクタークラスからのデータ操作用

/*!
 @brief デバイス座標からタイル座標の取得
 
 デバイススクリーン座標からマップ座標へ、マップ座標からタイルの座標へ変換する。
 @param devicePosition デバイススクリーン座標
 @return タイルの座標
 */
CCPoint AKPlayData::convertDevicePositionToTilePosition(CCPoint devicePosition)
{
    // デバイススクリーン座標からマップ座標を取得する
    CCPoint mapPosition = m_tileMap->getMapPositionFromDevicePosition(devicePosition);
    
    // マップ座標からタイルの座標を取得する
    return m_tileMap->getTilePositionFromMapPosition(mapPosition);
}

/*!
 @brief 自機弾生成
 
 自機弾を生成する。
 @param position 生成位置
 */
void AKPlayData::createPlayerShot(CCPoint position)
{
    // プールから未使用のメモリを取得する
    AKPlayerShot *playerShot = m_playerShotPool.getNext();
    AKPlayerShot *playerUpwardShot = m_playerShotPool.getNext();
    AKPlayerShot *playerDownwardShot = m_playerShotPool.getNext();
    if (playerShot == NULL || playerUpwardShot == NULL || playerDownwardShot == NULL) {
        // 空きがない場合は処理終了する
        AKAssert(false, "自機弾プールに空きなし");
        return;
    }
    
    // 自機弾を生成する
    playerShot->createPlayerShot(position, m_batches.at(kAKCharaPosZPlayerShot));
    playerUpwardShot->createPlayerShotUpward(position, m_batches.at(kAKCharaPosZPlayerShot));
    playerDownwardShot->createPlayerShotDownward(position, m_batches.at(kAKCharaPosZPlayerShot));
}

/*!
 @brief オプション弾生成
 
 オプション弾を生成する。
 @param position 生成位置
 */
void AKPlayData::createOptionShot(CCPoint position)
{
    // プールから未使用のメモリを取得する
    AKPlayerShot *playerShot = m_playerShotPool.getNext();
    AKPlayerShot *playerUpwardShot = m_playerShotPool.getNext();
    AKPlayerShot *playerDownwardShot = m_playerShotPool.getNext();
    if (playerShot == NULL) {
        // 空きがない場合は処理終了する
        AKAssert(false, "自機弾プールに空きなし");
        return;
    }
    
    // 自機弾を生成する
    playerShot->createOptionShot(position, m_batches.at(kAKCharaPosZPlayerShot));
    playerUpwardShot->createOptionShotUpward(position, m_batches.at(kAKCharaPosZPlayerShot));
    playerDownwardShot->createOptionShotDownward(position, m_batches.at(kAKCharaPosZPlayerShot));
}

/*!
 @brief 反射弾生成
 
 反射弾を生成する。
 @param enemyShot 反射する敵弾
 */
void AKPlayData::createReflectShot(AKEnemyShot *enemyShot)
{
    AKLog(kAKLogPlayData_2, "反射弾生成");

    // プールから未使用のメモリを取得する
    AKEnemyShot *reflectShot = m_reflectShotPool.getNext();
    if (reflectShot == NULL) {
        // 空きがない場合は処理終了する
        AKAssert(false, "反射弾プールに空きなし");
        return;
    }
    
    // 反射する敵弾を元に反射弾を生成する
    reflectShot->createReflectShot(enemyShot, m_batches.at(kAKCharaPosZPlayerShot));
}

/*!
 @brief 敵生成
 
 敵キャラを生成する。
 @param type 敵種別
 @param position 生成位置
 @param progress 倒した時に進む進行度
 @return 生成した敵キャラ
 */
AKEnemy* AKPlayData::createEnemy(int type, CCPoint position, int progress)
{
    // プールから未使用のメモリを取得する
    AKEnemy *enemy = m_enemyPool.getNext();
    if (enemy == NULL) {
        // 空きがない場合は処理終了する
        AKAssert(false, "敵プールに空きなし");
        return NULL;
    }
    
    // 敵を生成する
    enemy->createEnemy(type, position, progress, m_batches.at(kAKCharaPosZEnemy));
    
    return enemy;
}

/*!
 @brief 敵弾インスタンスの取得
 
 敵弾プールから次のインスタンスを取得する。
 @return 敵弾インスタンス
 */
AKEnemyShot* AKPlayData::getEnemyShot()
{
    // プールから未使用のメモリを取得する
    AKEnemyShot *enemyShot = m_enemyShotPool.getNext();
    
    AKAssert(enemyShot, "敵弾プールに空きなし");
    
    return enemyShot;
}

/*!
 @brief 敵弾配置ノードの取得
 
 敵弾を配置するノードを取得する。
 @return 敵弾配置ノード
 */
CCNode* AKPlayData::getEnemyShotParent()
{
    return m_batches.at(kAKCharaPosZEnemyShot);
}

/*!
 @brief 画面効果生成
 
 画面効果を生成する。
 @param type 画面効果種別
 @param x x座標
 @param y y座標
 */
void AKPlayData::createEffect(int type, CCPoint position)
{
    // プールから未使用のメモリを取得する
    AKEffect *effect = m_effectPool.getNext();
    if (effect == NULL) {
        // 空きがない場合は処理終了する
        AKAssert(false, "画面効果プールに空きなし");
        return;
    }
    
    // 画面効果を生成する
    effect->createEffect(type, position, m_batches.at(kAKCharaPosZEffect));
}

/*!
 @brief 障害物生成
 
 障害物を生成する。
 @param type 障害物種別
 @param x x座標
 @param y y座標
 */
void AKPlayData::createBlock(int type, CCPoint position)
{
    AKLog(kAKLogPlayData_3, "createBlock() start:type=%d position=(%f, %f)",
          type, position.x, position.y);
    
    // プールから未使用のメモリを取得する
    AKBlock *block = m_blockPool.getNext();
    if (block == NULL) {
        // 空きがない場合は処理終了する
        AKAssert(false, "障害物プールに空きなし");
        return;
    }
    
    // 障害物を生成する
    block->createBlock(type, position, m_batches.at(kAKCharaPosZBlock)); 
}

/*!
 @brief 失敗時処理
 
 失敗した時の処理を行う。
 */
void AKPlayData::miss()
{
    // 残機がまだ残っている場合は残機を一つ減らして復活処理を行う
    if (m_life > 0) {
        
        // シールドをオフにする
        setShield(false);
        
        // 残機をひとつ減らす
        setLife(m_life - 1);
                
        // 自機復活待機時間を設定する
        m_rebirthWait = kAKRebirthInterval;
    }
    // 残機が残っていない場合はゲームオーバーとする
    else {
        m_scene->gameOver();
    }
}

/*!
 @brief スコア加算
 
 スコアを加算する。ハイスコアを超えている場合はハイスコアも合わせて更新する。
 スコア、ハイスコアの表示を更新する。
 @param score スコア増加量
 */
void AKPlayData::addScore(int score)
{
    // エクステンドの判定を行う
    // ゲームオーバー時にはエクステンドしない(相打ちによってスコアが入った時)
    if ((int)(m_score / kAKExtendScore) < (int)((m_score + score) / kAKExtendScore) &&
        !m_scene->isGameOver()) {
        
        // TODO:エクステンドの効果音を鳴らす
        
        // 残機の数を増やす
        setLife(m_life + 1);
        
        // TODO:実績を解除する
    }
    
    // スコアを加算する
    m_score += score;
    
    // スコア表示を更新する
    m_scene->setScoreLabel(m_score);
    
    // ハイスコアを更新している場合はハイスコアを設定する
    if (m_score > m_hiScore) {
        
        // ハイスコアにスコアの値を設定する
        m_hiScore = m_score;
    }
}

/*!
 @brief 進行度を進める
 
 進行度を進める。
 @param progress 進行度
 */
void AKPlayData::addProgress(int progress)
{
    m_tileMap->setProgress(m_tileMap->getProgress() + progress);
}
