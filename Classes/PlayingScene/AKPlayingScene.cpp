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
 @file AKPlayingScene.m
 @brief プレイシーンの定義
 
 プレイシーンクラスを定義する。
 */

#include "AKPlayingScene.h"
#include "AppDelegate.h"

using cocos2d::CCSpriteFrameCache;
using cocos2d::CCLayer;
using cocos2d::CCScene;
using cocos2d::CCDirector;
using cocos2d::CCPoint;
using cocos2d::CCBlink;
using cocos2d::CCCallFunc;
using cocos2d::CCSequence;
using cocos2d::SEL_CallFunc;
using cocos2d::CCTransitionFade;
using cocos2d::CCSpriteBatchNode;
using cocos2d::CCRect;
using cocos2d::CCSprite;

/// レイヤーのz座標、タグの値にも使用する
enum {
    kAKLayerPosZBack = 0,   ///< 背景レイヤー
    kAKLayerPosZCharacter,  ///< キャラクターレイヤー
    kAKLayerPosZFrameBack,  ///< 枠背景レイヤー
    kAKLayerPosZFrameBar,   ///< 枠棒レイヤー
    kAKLayerPosZInfo,       ///< 情報レイヤー
    kAKLayerPosZResult,     ///< ステージクリアレイヤー
    kAKLayerPosZInterface   ///< インターフェースレイヤー
};

/// 情報レイヤーに配置するノードのタグ
enum {
    kAKInfoTagChickenGauge = 0, ///< チキンゲージ
    kAKInfoTagLife,             ///< 残機
    kAKInfoTagScore,            ///< スコア
    kAKInfoTagHiScore,          ///< ハイスコア
};

//======================================================================
// 動作に関する定数
//======================================================================
/// 自機移動をスライド量の何倍にするか
static const float kAKPlayerMoveVal = 1.8f;
/// 開始ステージ番号
static const int kAKStartStage = 1;
/// ゲームオーバー時の待機フレーム数
static const int kAKGameOverWaitFrame = 60;

//======================================================================
// コントロールの表示に関する定数
//======================================================================
/// チキンゲージ配置位置、下からの座標
static const float kAKChickenGaugePosFromBottomPoint = 12.0f;
/// 残機表示の位置、ステージ座標x座標
static const float kAKLifePosXOfStage = 4.0f;
/// 残機表示の位置、ステージ座標y座標
static const float kAKLifePosYOfStage = 272.0f;
/// スコアの表示位置、ステージ座標y座標
static const float kAKScorePosYOfStage = 272.0f;
/// スコア表示のフォーマット
static const char *kAKScoreFormat = "SCORE:%06d";

//======================================================================
// 枠の表示に関する定数
//======================================================================
/// 枠背景1ブロックのサイズ
static const int kAKFrameBackSize = 32;
/// 枠棒1ブロックのサイズ
static const int kAKFrameBarSize = 8;
/// 枠背景の画像名
static const char *kAKFrameBackName = "FrameBack.png";
/// 枠棒左の画像名
static const char *kAKFrameBarLeft = "FrameLeft.png";
/// 枠棒右の画像名
static const char *kAKFrameBarRight = "FrameRight.png";
/// 枠棒上の画像名
static const char *kAKFrameBarTop = "FrameTop.png";
/// 枠棒下の画像名
static const char *kAKFrameBarBottom = "FrameBottom.png";
/// 枠棒左上の画像名
static const char *kAKFrameBarLeftTop = "FrameLeftTop.png";
/// 枠棒左下の画像名
static const char *kAKFrameBarLeftBottom = "FrameLeftBottom.png";
/// 枠棒右上の画像名
static const char *kAKFrameBarRightTop = "FrameRightTop.png";
/// 枠棒右下の画像名
static const char *kAKFrameBarRightBottom = "FrameRightBottom.png";

#pragma mark オブジェクト生成/解放

/*!
 @brief コンビニエンスコンストラクタ

 インスタンスを生成し、初期化処理を行い、autoreleaseを行う。
 @return 生成したインスタンス
 */
AKPlayingScene* AKPlayingScene::create()
{
    AKPlayingScene *instance = new AKPlayingScene();
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
 @brief コンストラクタ
 
 オブジェクトの初期化を行う。
 */
AKPlayingScene::AKPlayingScene() :
m_data(NULL), m_state(kAKGameStatePreLoad), m_nextState(kAKGameStatePreLoad),
m_sleepFrame(0), m_backgroundLayer(NULL), m_characterLayer(NULL),
m_infoLayer(NULL), m_interfaceLayer(NULL), m_life(NULL), m_chickenGauge(NULL)
{
    AKLog(kAKLogPlayingScene_1, "start");
    
    // テクスチャアトラスを読み込む
    CCSpriteFrameCache *spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    spriteFrameCache->addSpriteFramesWithFile(kAKControlTextureAtlasDefFile,
                                              kAKControlTextureAtlasFile);

    // 背景レイヤーを作成する
    createBackGround();

    // キャラクターレイヤーを作成する
    createCharacterLayer();
    
    // 情報レイヤーを作成する
    createInfoLayer();
    
    // インターフェースレイヤーを作成する
    createInterface();
    
    // 枠レイヤーを作成する
    createFrame();
    
    // 状態をシーン読み込み前に設定する
    setState(kAKGameStatePreLoad);
    
    // 更新処理開始
    scheduleUpdate();

    // ゲームデータを作成する
    m_data = new AKPlayData(this);
    
    AKLog(kAKLogPlayingScene_1, "end");
}

/*!
 @brief デストラクタ
 
 メンバの解放を行う。
 */
AKPlayingScene::~AKPlayingScene()
{
    AKLog(kAKLogPlayingScene_1, "start");

    // メンバを解放する
    if (m_backgroundLayer != NULL) {
        m_backgroundLayer->release();
    }
    if (m_characterLayer != NULL) {
        m_characterLayer->release();
    }
    if (m_infoLayer != NULL) {
        m_infoLayer->release();
    }
    if (m_interfaceLayer != NULL) {
        m_interfaceLayer->release();
    }
    if (m_life != NULL) {
        m_life->release();
    }
    if (m_score != NULL) {
        m_score->release();
    }
    if (m_chickenGauge != NULL) {
        m_chickenGauge->release();
    }
    delete m_data;
    
    // 未使用のスプライトフレームを解放する
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();

    AKLog(kAKLogPlayingScene_1, "end");
}

#pragma mark アクセサ

/*!
 @brief ゲームプレイの状態設定
 
 ゲームプレイの状態を設定する。
 インターフェースの有効項目を変更する。
 @param state ゲームプレイの状態
 */
void AKPlayingScene::setState(enum AKGameState state)
{
    // メンバに設定する
    m_state = state;
    
    // インターフェースの有効項目を変更する
    switch (m_state) {
        case kAKGameStatePlaying:   // プレイ中
            m_interfaceLayer->setEnableTag(kAKMenuTagPlaying);
            break;
            
        case kAKGameStatePause:     // 一時停止中
            m_interfaceLayer->setEnableTag(kAKMenuTagPause);
            break;
            
        case kAKGameStateQuitMenu:  // 終了メニュー
            m_interfaceLayer->setEnableTag(kAKMenuTagQuit);
            break;
            
        case kAKGameStateGameOver:  // ゲームオーバー
            m_interfaceLayer->setEnableTag(kAKMenuTagGameOver);
            break;
            
        default:                    // その他
            m_interfaceLayer->setEnableTag(0);
            break;
    }
    
    /* TODO:広告処理を作成する 
    // Root Viewを取得する
    AKNavigationController *viewController = (AKNavigationController *)[UIApplication sharedApplication].keyWindow.rootViewController;

    // 広告バナーの表示・非表示を切り替える
    switch (state) {
        case kAKGameStatePreLoad:   // ゲームシーン読み込み前
        case kAKGameStatePlaying:   // プレイ中
            AKLog(kAKLogPlayingScene_1, "広告バナーを非表示にする。:state=%d", state);
            // プレイ中は広告バナーを非表示にする
            [viewController hiddenAdBanner];
            break;
            
        case kAKGameStateStart:     // ゲーム開始時
        case kAKGameStateWait:      // アクション待機中
        case kAKGameStateSleep:     // スリープ処理中
            // 現在の状態を維持する
            break;
            
        default:                    // その他
            AKLog(kAKLogPlayingScene_1, "広告バナーを表示する。:state=%d", state);
            // 広告バナーを表示する
            [viewController viewAdBanner];
            break;
    }
    */
    
    /* TODO:バックグラウンド判定処理を作成する
    // AppDelegateを取得する
    NSAssert([[[UIApplication sharedApplication] delegate] isKindOfClass:[AppController class]], "");
    AppController *app = (AppController *)[[UIApplication sharedApplication] delegate];
    
    // バックグラウンドで実行中にプレイ中に遷移した場合はバックグラウンド移行処理を行う
    if (app.isBackGround && state == kAKGameStatePlaying) {
        AKLog(kAKLogPlayingScene_0, "バックグラウンドで実行中にプレイ中に遷移した");
        [self onDidEnterBackground];
    }
    */
}

/*!
 @brief 背景レイヤー取得
 
 背景レイヤーを取得する。
 @return 背景レイヤー
 */
CCLayer* AKPlayingScene::getBackgroundLayer()
{
    AKAssert(m_backgroundLayer, "レイヤーが作成されていない");

    return m_backgroundLayer;
}

/*!
 @brief キャラクターレイヤー取得
 
 キャラクターレイヤーを取得する。
 @return キャラクターレイヤー
 */
CCLayer* AKPlayingScene::getCharacterLayer()
{
    AKAssert(m_characterLayer, "レイヤーが作成されていない");

    return m_characterLayer;
}

/*!
 @brief 情報レイヤー取得
 
 情報レイヤーを取得する。
 @return 情報レイヤー
 */
CCLayer* AKPlayingScene::getInfoLayer()
{
    AKAssert(m_infoLayer, "レイヤーが作成されていない");

    return m_infoLayer;
}

/*!
 @brief インターフェースレイヤー取得
 
 インターフェースレイヤーを取得する。
 @return インターフェースレイヤー
 */
AKPlayingSceneIF* AKPlayingScene::getInterfaceLayer()
{
    AKAssert(m_interfaceLayer, "レイヤーが作成されていない");

    return m_interfaceLayer;
}

/*!
 @brief 残機表示取得
 
 残機表示取得を取得する。
 @return 残機表示取得
 */
AKLife* AKPlayingScene::getLife()
{
    return m_life;
}

/*!
 @brief ゲームオーバーかどうか
 
 ゲームオーバーかどうかを返す。
 現在の状態がゲームオーバー、または次の状態がゲームオーバーの場合はYESを返す。
 @return ゲームオーバーかどうか
 */
bool AKPlayingScene::isGameOver()
{
    // 現在の状態がゲームオーバー、または次の状態がゲームオーバーの場合はYESを返す
    if (m_state == kAKGameStateGameOver || m_nextState == kAKGameStateGameOver) {
        return true;
    }
    else {
        return false;
    }
}

#pragma mark イベント処理

/*!
 @brief トランジション終了時の処理
 
 トランジション終了時の処理。
 トランジション途中でBGM再生等が行われないようにするため、
 トランジション終了後にゲーム開始の状態にする。
 */
void AKPlayingScene::onEnterTransitionDidFinish()
{
    // ゲーム状態を開始時に変更する。
    setState(kAKGameStateStart);
    
    // スーパークラスの処理を実行する
    CCScene::onEnterTransitionDidFinish();
}

/*!
 @brief バックグラウンド移行処理
 
 バックグラウンドに移行したときの処理を行う。
 ゲームプレイ中にバックグラウンドに移行したときは一時停止する。
 */
void AKPlayingScene::onDidEnterBackground()
{
    // ゲームプレイ中の場合は一時停止状態にする
    if (m_state == kAKGameStatePlaying) {
        
        // TODO:BGMを一時停止する
//    [[SimpleAudioEngine sharedEngine] pauseBackgroundMusic];
        
        // ゲーム状態を一時停止に変更する
        setState(kAKGameStatePause);
        
        // プレイデータのポーズ処理を行う
        m_data->pause();
    }
}

/*!
 @brief 自機の移動
 
 スライド入力によって自機を移動する。
 @param object メニュー項目
 */
void AKPlayingScene::movePlayer(const AKMenuItem *item)
{
    AKLog(1, "movePlayer() start");
    
    // 画面上のタッチ位置を取得する
    CCPoint locationInView = item->getTouch()->getLocationInView();
    
    // cocos2dの座標系に変換する
    CCPoint location = CCDirector::sharedDirector()->convertToGL(locationInView);
    
    // 自機を移動する
    float x = (location.x - item->getPrevPoint()->x) * kAKPlayerMoveVal;
    float y = (location.y - item->getPrevPoint()->y) * kAKPlayerMoveVal;
    m_data->movePlayer(x, y);
}

/*!
 @brief シールドボタン選択処理
 
 シールドボタン選択時にシールドを有効にする。
 選択解除時にシールドを解除する。
 @param object メニュー項目
 */
void AKPlayingScene::touchShieldButton(const AKMenuItem *item)
{
    // タッチのフェーズによって処理を分ける
    switch (item->getTouchPhase()) {
        case kAKMenuTouchBegan:     // タッチ開始
            // シールドモードを有効にする
            setShieldButtonSelected(true);
            break;
            
        case kAKMenuTouchCancelled: // タッチ取り消し
        case kAKMenuTouchEnded:     // タッチ終了
            // シールドモードを無効にする
            setShieldButtonSelected(false);
            break;
            
        default:                    // その他は無処理
            break;
    }
}

/*!
 @brief ポーズボタン選択処理
 
 ポーズボタン選択時の処理。一時停止メニューを表示し、ゲームの状態を一時停止状態に遷移する。
 @param object メニュー項目
 */
void AKPlayingScene::touchPauseButton()
{
    // TODO:BGMを一時停止する
//    [[SimpleAudioEngine sharedEngine] pauseBackgroundMusic];    

    // TODO:一時停止効果音を鳴らす
//    [[SimpleAudioEngine sharedEngine] playEffect:kAKPauseSE];

    // ゲーム状態を一時停止に変更する
    setState(kAKGameStatePause);
    
    // プレイデータのポーズ処理を行う
    m_data->pause();
}

/*!
 @brief 再開ボタン選択処理
 
 再開ボタン選択時の処理。再開ボタンをブリンクし、アクション完了時にゲーム再開処理が呼ばれるようにする。
 */
void AKPlayingScene::touchResumeButton()
{
    // 他の処理が動作しないように待機状態にする
    setState(kAKGameStateWait);
    
    // ボタンのブリンクアクションを作成する
    CCBlink *blink = CCBlink::create(0.2f, 2);
    CCCallFunc *callFunc = CCCallFunc::create(this, callfunc_selector(AKPlayingScene::resume));
    CCSequence *action = CCSequence::create(blink, callFunc);
    
    // ボタンを取得する
    CCNode *button = m_interfaceLayer->getResumeButton();
    
    // ブリンクアクションを開始する
    button->runAction(action);
    
    // TODO:一時停止効果音を鳴らす
//    [[SimpleAudioEngine sharedEngine] playEffect:kAKPauseSE];
}

/*!
 @brief 終了ボタン選択処理
 
 終了ボタンが選択された時の処理を行う。
 効果音を鳴らし、終了ボタンをブリンクし、アクション完了時に終了メニュー表示処理が呼ばれるようにする。
 @param object メニュー項目
 */
void AKPlayingScene::touchQuitButton()
{
    // 他の処理が動作しないように待機状態にする
    setState(kAKGameStateWait);
    
    // ボタンのブリンクアクションを作成する
    CCBlink *blink = CCBlink::create(0.2f, 2);
    CCCallFunc *callFunc = CCCallFunc::create(this, callfunc_selector(AKPlayingScene::resume));
    CCSequence *action = CCSequence::create(blink, callFunc);
    
    // ボタンを取得する
    CCNode *button = m_interfaceLayer->getQuitButton();
    
    // ブリンクアクションを開始する
    button->runAction(action);
    
    // TODO:メニュー選択時の効果音を鳴らす
//    [[SimpleAudioEngine sharedEngine] playEffect:kAKMenuSelectSE];
}

/*!
 @brief 終了メニューYESボタン選択処理
 
 終了メニューでYESボタンが選択された時の処理を行う。
 効果音を鳴らし、タイトルシーンへと遷移する。
 @param object メニュー項目
 */
void AKPlayingScene::touchQuitYesButton()
{
    AKLog(kAKLogPlayingScene_1, "start");
    
    // ハイスコアをファイルに保存する
    m_data->writeHiScore();
    
    // タイトルシーンへの遷移を作成する
    CCTransitionFade *transition = CCTransitionFade::create(0.5f, AKTitleScene::create());
    
    // タイトルシーンへと遷移する
    CCDirector::sharedDirector()->replaceScene(transition);
}

/*!
 @brief 終了メニューNOボタン選択
 
 終了メニューのNOボタン選択時の処理。
 NOボタンをブリンクし、アクション完了時に一時停止メニュー表示処理が呼ばれるようにする。
 */
void AKPlayingScene::touchQuitNoButton()
{
    // 他の処理が動作しないように待機状態にする
    setState(kAKGameStateWait);
    
    // ボタンのブリンクアクションを作成する
    CCBlink *blink = CCBlink::create(0.2f, 2);
    CCCallFunc *callFunc = CCCallFunc::create(this, callfunc_selector(AKPlayingScene::resume));
    CCSequence *action = CCSequence::create(blink, callFunc);
    
    // ボタンを取得する
    CCNode *button = m_interfaceLayer->getQuitNoButton();
    
    // ブリンクアクションを開始する
    button->runAction(action);
    
    // TODO:メニュー選択時の効果音を鳴らす
//    [[SimpleAudioEngine sharedEngine] playEffect:kAKMenuSelectSE];
}

/*!
 @brief ツイートボタン選択処理
 
 ツイートボタンが選択された時の処理を行う。
 
 */
void AKPlayingScene::touchTweetButton()
{
    // TODO:メニュー選択時の効果音を鳴らす
//    [[SimpleAudioEngine sharedEngine] playEffect:kAKMenuSelectSE];
    
    // TODO:ツイートビューを表示する
//    [[AKTwitterHelper sharedHelper] viewTwitterWithInitialString:[data_ makeTweet]];
}

/*!
 @brief 更新処理
 
 ゲームの状態によって、更新処理を行う。
 @param dt フレーム更新間隔
 */
void AKPlayingScene::update(float delta)
{
    // ゲームの状態によって処理を分岐する
    switch (m_state) {
        case kAKGameStateStart:     // ゲーム開始時
            updateStart();
            break;
            
        case kAKGameStatePlaying:   // プレイ中
            updatePlaying();
            break;
            
        case kAKGameStateSleep:     // スリープ中
            updateSleep();
            break;
            
        default:
            // その他の状態のときは変化はないため、無処理とする
            break;
    }
    
}

/*!
 @brief イベント実行
 
 イベントを実行する。
 @param item 選択されたメニュー項目
 */
void AKPlayingScene::execEvent(const AKMenuItem *item)
{
    // 選択された項目に応じて処理を行う
    switch (item->getEventNo()) {
        case kAKEventTouchShieldButton:     // シールドボタン
            touchShieldButton(item);
            break;
            
        case kAKEventTouchPauseButton:      // ポーズボタン
            touchPauseButton();
            break;
            
        case kAKEventSlide:                 // 画面スライド
            movePlayer(item);
            break;
            
        case kAKEventTouchResumeButton:     // ポーズ解除ボタン
            touchResumeButton();
            break;
            
        case kAKEventTouchQuitButton:       // 終了ボタン
            touchQuitButton();
            break;
            
        case kAKEventTouchQuitYesButton:    // 終了メニューYESボタン
            touchQuitYesButton();
            break;
            
        case kAKEventTouchQuitNoButton:     // 終了メニューNOボタン
            touchQuitNoButton();
            break;
            
        case kAKEventTouchBackButton:       // タイトルへ戻るボタン
            touchQuitButton();
            break;
            
        case kAKEventTouchTwitterButton:    // Twitterボタン
            touchTweetButton();
            break;
            
        default:
            AKAssert(false, "不正なイベント番号:%d", item->getEventNo());
            break;
    }
}

#pragma mark AKPlayDataからのシーン操作用

/*!
 @brief チキンゲージ取得
 
 チキンゲージを取得する。
 @return チキンゲージ
 */
AKChickenGauge* AKPlayingScene::getChickenGauge()
{
    return m_chickenGauge;
}

/*!
 @brief シールドボタン表示切替
 
 シールドボタンの表示を選択・非選択状態で切り替えを行う。
 @param seleted 選択状態かどうか
 */
void AKPlayingScene::setShieldButtonSelected(bool selected)
{
    m_interfaceLayer->setShieldButtonSelected(selected);
}

/*!
 @brief スコアラベル更新
 
 スコアラベルの文字列を更新する。
 @param score スコア
 */
void AKPlayingScene::setScoreLabel(int score)
{
    // ラベルに設定する文字列を作成する
    char scoreString[16] = "";
    snprintf(scoreString, sizeof(scoreString), kAKScoreFormat, score);

    // ラベルの文字列を変更する
    m_score->setString(scoreString);
}

/*!
 @brief ゲームオーバー
 
 ゲームオーバー時はBGMをOFFにし、一定時間待機する。
 待機後はゲームオーバーのラベルとコントロールを有効にする。
 ここでは状態を待機中、次の状態をゲームオーバーに設定する。
 */
void AKPlayingScene::gameOver()
{
    AKLog(kAKLogPlayingScene_1, "start");
    
    // 状態を待機中へ遷移する
    setState(kAKGameStateSleep);
    
    // 待機後の状態をゲームオーバーに設定する
    m_nextState = kAKGameStateGameOver;
    
    // 待機フレーム数を設定する
    m_sleepFrame = kAKGameOverWaitFrame;

    AKLog(kAKLogPlayingScene_1, "end");
}

#pragma mark プライベートメソッド_インスタンス初期化

/*!
 @brief 背景レイヤー作成
 
 背景レイヤーを作成する。
 */
void AKPlayingScene::createBackGround()
{
    // 背景レイヤーを作成する
    m_backgroundLayer = AKCreateBackColorLayer();
    m_backgroundLayer->retain();

    // 背景レイヤーを画面に配置する
    addChild(m_backgroundLayer, kAKLayerPosZBack, kAKLayerPosZBack);
}

/*!
 @brief キャラクターレイヤー作成
 
 キャラクターレイヤーを作成する。
 */
void AKPlayingScene::createCharacterLayer()
{
    // キャラクターを配置するレイヤーを作成する
    m_characterLayer = CCLayer::create();
    m_characterLayer->retain();
    
    AKLog(kAKLogPlayingScene_1, "キャラクターレイヤー作成:%p", m_characterLayer);
    
    // キャラクターレイヤーを画面に配置する
    addChild(m_characterLayer, kAKLayerPosZCharacter, kAKLayerPosZCharacter);
}

/*!
 @brief 情報レイヤー作成
 
 情報レイヤーを作成する。レイヤーに配置するものも作成する。
 */
void AKPlayingScene::createInfoLayer()
{
    // 情報レイヤーを作成する
    m_infoLayer = CCLayer::create();
    m_infoLayer->retain();
    
    // 情報レイヤーを画面に配置する
    addChild(m_infoLayer, kAKLayerPosZInfo, kAKLayerPosZInfo);
    
    // チキンゲージを作成する
    m_chickenGauge = AKChickenGauge::create();
    m_chickenGauge->retain();
    
    // チキンゲージを情報レイヤーに配置する
    m_infoLayer->addChild(m_chickenGauge, 0, kAKInfoTagChickenGauge);
    
    // チキンゲージの座標を設定する
    float x = AKScreenSize::center().x;
    float y = AKScreenSize::positionFromBottomPoint(kAKChickenGaugePosFromBottomPoint);
    m_chickenGauge->setPosition(CCPoint(x, y));
    
    // 残機表示を作成する
    m_life = AKLife::create();
    m_life->retain();
    
    // 残機表示を情報レイヤーに配置する
    m_infoLayer->addChild(m_life, 0, kAKInfoTagLife);
    
    // 残機表示の座標を設定する
    x = AKScreenSize::xOfStage(kAKLifePosXOfStage) + m_life->getWidth() / 2;
    m_life->setPosition(CCPoint(x, y));
    
    // スコア表示の文字列を作成する
    char scoreString[16] = "";
    snprintf(scoreString, sizeof(scoreString), kAKScoreFormat, 0);
    
    // スコア表示を作成する
    m_score = AKLabel::createLabel(scoreString, strlen(scoreString), 1, kAKLabelFrameNone);
    
    // スコア表示を情報レイヤーに配置する
    m_infoLayer->addChild(m_score, 0, kAKInfoTagScore);
    
    // スコア表示の座標を設定する
    x = AKScreenSize::center().x;
    y = AKScreenSize::yOfStage(kAKScorePosYOfStage);
    m_score->setPosition(CCPoint(x, y));
}

/*!
 @brief インターフェースレイヤー作成
 
 インターフェースレイヤーを作成する。レイヤーに配置するものも作成する。
 */
void AKPlayingScene::createInterface()
{
    // インターフェースレイヤーを作成する
    m_interfaceLayer = AKPlayingSceneIF::createInterface(this);
    m_interfaceLayer->retain();
    
    // インターフェースレイヤーを画面に配置する
    addChild(m_interfaceLayer, kAKLayerPosZInterface, kAKLayerPosZInterface);
}

/*!
 @brief 枠レイヤー作成
 
 枠レイヤーを作成する。
 */
void AKPlayingScene::createFrame()
{
    // 枠の背景を作成する
    createFrameBack();
    
    // 枠の棒を作成する
    createFrameBar();
}

/*!
 @brief 枠背景作成
 
 枠の背景部分を作成する。
 */
void AKPlayingScene::createFrameBack()
{
    // 枠の背景用バッチノードを作成する
    CCSpriteBatchNode *frameBackBatch = CCSpriteBatchNode::create(kAKControlTextureAtlasFile);
    
    // ブロックサイズをデバイスに合わせて計算する
    int frameBackSize = AKScreenSize::deviceLength(kAKFrameBackSize);
    
    // 左側の枠の座標を作成する
    CCRect rect(0.0f,
                0.0f,
                (AKScreenSize::screenSize().width - AKScreenSize::stageSize().width) / 2.0f,
                AKScreenSize::screenSize().height);
    
    // 右端揃えにするため、ブロックのはみ出している分だけ左にずらす
    if ((int)rect.size.width % frameBackSize > 0) {
        rect.origin.x -= frameBackSize - (int)rect.size.width % frameBackSize;
        rect.size.width += frameBackSize - (int)rect.size.width % frameBackSize;
    }
    
    // ステージの下端に揃えるため、ブロックのはみ出している分だけ下にずらす
    if ((int)AKScreenSize::yOfStage(0.0f) % frameBackSize > 0) {
        rect.origin.y -= frameBackSize - (int)AKScreenSize::yOfStage(0.0f) % frameBackSize;
        rect.size.height += frameBackSize - (int)AKScreenSize::yOfStage(0.0f) % frameBackSize;
    }
    
    // 枠背景のブロックを配置する
    createFrameBlock(frameBackBatch, kAKFrameBackName, frameBackSize, rect);
    
    // 右側の枠の座標を作成する
    rect.origin.x = AKScreenSize::center().x + AKScreenSize::stageSize().width / 2;
    rect.origin.y = 0.0f;
    rect.size.width = AKScreenSize::screenSize().width - AKScreenSize::stageSize().width / 2;
    rect.size.height = AKScreenSize::screenSize().height;
    
    // ステージの下端に揃えるため、ブロックのはみ出している分だけ下にずらす
    if ((int)AKScreenSize::yOfStage(0.0f) % frameBackSize > 0) {
        rect.origin.y -= frameBackSize - (int)AKScreenSize::yOfStage(0.0f) % frameBackSize;
        rect.size.height += frameBackSize - (int)AKScreenSize::yOfStage(0.0f) % frameBackSize;
    }

    // 枠背景のブロックを配置する
    createFrameBlock(frameBackBatch, kAKFrameBackName, frameBackSize, rect);
    
    // 下側の枠の座標を作成する
    rect.origin.x = (AKScreenSize::screenSize().width - AKScreenSize::stageSize().width) / 2.0f;
    rect.origin.y = 0.0f;
    rect.size.width = AKScreenSize::screenSize().width;
    rect.size.height = AKScreenSize::yOfStage(0.0f);
    
    // 上端揃えにするため、ブロックのはみ出している分だけ下にずらす
    if ((int)rect.size.height % frameBackSize > 0) {
        rect.origin.y -= frameBackSize - (int)rect.size.height % frameBackSize;
        rect.size.height += frameBackSize - (int)rect.size.height % frameBackSize;
    }
    
    // 枠背景のブロックを配置する
    createFrameBlock(frameBackBatch, kAKFrameBackName, frameBackSize, rect);
    
    // 上側の枠の座標を作成する
    rect.origin.x = (AKScreenSize::screenSize().width - AKScreenSize::stageSize().width) / 2.0f;
    rect.origin.y = AKScreenSize::yOfStage(0.0f) + AKScreenSize::stageSize().height;
    rect.size.width = AKScreenSize::screenSize().width;
    rect.size.height = AKScreenSize::screenSize().height - 
        AKScreenSize::stageSize().height - AKScreenSize::yOfStage(0.0f);
    
    // 高さがある場合は上側の枠レイヤーを作成する
    if (rect.size.height > 0.0f) {
        createFrameBlock(frameBackBatch, kAKFrameBackName, frameBackSize, rect);
    }
    
    // 枠の背景用バッチノードをシーンに配置する
    addChild(frameBackBatch, kAKLayerPosZFrameBack, kAKLayerPosZFrameBack);
}

/*!
 @brief 枠棒作成
 
 枠の棒の部分を作成する。
 */
void AKPlayingScene::createFrameBar()
{
    // 枠の棒用バッチノードを作成する
    CCSpriteBatchNode *frameBarBatch = CCSpriteBatchNode::create(kAKControlTextureAtlasFile);
    
    // ブロックサイズをデバイスに合わせて計算する
    int frameBarSize = AKScreenSize::deviceLength(kAKFrameBarSize);
    
    // 左側の棒の位置を決定する
    CCRect rect(AKScreenSize::xOfStage(0.0f) - frameBarSize,
                AKScreenSize::yOfStage(0.0f),
                frameBarSize,
                AKScreenSize::stageSize().height);
    
    // 左側の棒を配置する
    createFrameBlock(frameBarBatch, kAKFrameBarLeft, frameBarSize, rect);
    
    // 右側の棒の位置を決定する
    rect.origin.x = AKScreenSize::xOfStage(0.0f) + AKScreenSize::stageSize().width;
    rect.origin.y = AKScreenSize::yOfStage(0.0f);
    rect.size.width = frameBarSize;
    rect.size.height = AKScreenSize::stageSize().height;
    
    // 右側の棒を配置する
    createFrameBlock(frameBarBatch, kAKFrameBarRight, frameBarSize, rect);
    
    // 下側の棒の位置を決定する
    rect.origin.x = AKScreenSize::xOfStage(0.0f);
    rect.origin.y = AKScreenSize::yOfStage(0.0f) - frameBarSize;
    rect.size.width = AKScreenSize::stageSize().width;
    rect.size.height = frameBarSize;
    
    // 下側の棒を配置する
    createFrameBlock(frameBarBatch, kAKFrameBarBottom, frameBarSize, rect);
    
    // 左下の棒の位置を決定する
    rect.origin.x = AKScreenSize::xOfStage(0.0f) - frameBarSize;
    rect.origin.y = AKScreenSize::yOfStage(0.0f) - frameBarSize;
    rect.size.width = frameBarSize;
    rect.size.height = frameBarSize;
    
    // 左下の棒を配置する
    createFrameBlock(frameBarBatch, kAKFrameBarLeftBottom, frameBarSize, rect);
    
    // 右下の棒の位置を決定する
    rect.origin.x = AKScreenSize::xOfStage(0.0f) + AKScreenSize::stageSize().width;
    rect.origin.y = AKScreenSize::yOfStage(0.0f) - frameBarSize;
    rect.size.width = frameBarSize;
    rect.size.height = frameBarSize;
    
    // 右下の棒を配置する
    createFrameBlock(frameBarBatch, kAKFrameBarRightBottom, frameBarSize, rect);

    // 上側の棒の位置を決定する
    rect.origin.x = AKScreenSize::xOfStage(0.0f);
    rect.origin.y = AKScreenSize::yOfStage(0.0f) + AKScreenSize::stageSize().height;
    rect.size.width = AKScreenSize::stageSize().width;
    rect.size.height = frameBarSize;

    // 上側に棒を配置する隙間がある場合
    if (rect.origin.y < AKScreenSize::screenSize().height) {
        
        // 上側の棒を配置する
        createFrameBlock(frameBarBatch, kAKFrameBarTop, frameBarSize, rect);

        // 左上の棒の位置を決定する
        rect.origin.x = AKScreenSize::xOfStage(0.0f) - frameBarSize;
        rect.origin.y = AKScreenSize::yOfStage(0.0f) + AKScreenSize::stageSize().height;
        rect.size.width = frameBarSize;
        rect.size.height = frameBarSize;
        
        // 左上の棒を配置する
        createFrameBlock(frameBarBatch, kAKFrameBarLeftTop, frameBarSize, rect);
        
        // 右上の棒の位置を決定する
        rect.origin.x = AKScreenSize::xOfStage(0.0f) + AKScreenSize::stageSize().width;
        rect.origin.y = AKScreenSize::yOfStage(0.0f) + AKScreenSize::stageSize().height;
        rect.size.width = frameBarSize;
        rect.size.height = frameBarSize;
        
        // 右上の棒を配置する
        createFrameBlock(frameBarBatch, kAKFrameBarRightTop, frameBarSize, rect);
    }
    
    // 枠の棒用バッチノードをシーンに配置する
    addChild(frameBarBatch, kAKLayerPosZFrameBar, kAKLayerPosZFrameBar);
}

/*!
 @brief 枠背景ブロック配置
 
 指定された範囲に枠背景のブロックを配置する。
 @param node 配置先のノード
 @param name ブロックの画像名
 @param size 1ブロックのサイズ
 @param rect 配置先の範囲
 */
void AKPlayingScene::createFrameBlock(CCNode *node,
                                      const char *name,
                                      int size,
                                      const CCRect &rect)
{
    // 枠の背景ブロックを指定範囲に敷き詰める
    for (int y = rect.origin.y; y < rect.origin.y + rect.size.height; y += size) {
        for (int x = rect.origin.x; x < rect.origin.x + rect.size.width; x += size) {
            
            AKLog(kAKLogPlayingScene_3, "x=%d, y=%d", x, y);
            
            // 背景ブロック画像のスプライトを作成する
            CCSprite *sprite = CCSprite::createWithSpriteFrameName(name);
            
            // 位置を設定する
            // 左下を(0, 0)に合うようにするため、サイズの半分ずらす
            sprite->setPosition(CCPoint(x + size / 2, y + size / 2));
            
            // 背景ブロックをノードに配置する
            node->addChild(sprite);
        }
    }
}

#pragma mark プライベートメソッド_更新処理

/*!
 @brief ゲーム開始時の更新処理
 
 ステージ定義ファイルを読み込み、敵を配置する。
 */
void AKPlayingScene::updateStart()
{
    AKLog(kAKLogPlayingScene_1, "start");
    
    // 開始ステージのスクリプトを読み込む
    m_data->readScript(kAKStartStage);
    
    // 状態をプレイ中へと進める
    setState(kAKGameStatePlaying);
}

/*!
 @brief プレイ中の更新処理
 
 各キャラクターの移動処理、衝突判定を行う。
 */
void AKPlayingScene::updatePlaying()
{
    // ゲームデータの更新を行う
    m_data->update();
}

/*!
 @brief スリープ処理中の更新処理
 
 スリープ時間が経過したあと、次の状態へ遷移する。
 ゲームオーバーへの遷移の場合は遷移するまでプレイ中の状態更新を行う。
 */
void AKPlayingScene::updateSleep()
{
    // スリープフレーム数をカウントする
    m_sleepFrame--;
    
    // スリープフレーム数が経過した時に次の状態へ遷移する
    if (m_sleepFrame <= 0) {
        
        setState(m_nextState);
    }
    // まだスリープ時間が残っている場合
    else {
        
        // ゲームオーバーへの遷移の場合はプレイ中の状態を更新する
        if (m_nextState == kAKGameStateGameOver) {
            updatePlaying();
        }
    }
}

#pragma mark プライベートメソッド_状態遷移

/*!
 @brief ゲーム再開
 
 一時停止中の状態からゲームを再会する。
 */
void AKPlayingScene::resume()
{
    // 一時停止中から以外の変更の場合はエラー
    AKAssert(m_state == kAKGameStateWait, "状態遷移異常");
    
    // TODO:一時停止したBGMを再開する
//    [[SimpleAudioEngine sharedEngine] resumeBackgroundMusic];
    
    // ゲーム状態をプレイ中に変更する
    setState(kAKGameStatePlaying);
    
    // プレイデータのゲーム再開処理を行う
    m_data->resume();
}

/*!
 @brief 終了メニュー表示
 
 ゲームの状態を終了メニュー表示中に遷移する。
 */
void AKPlayingScene::viewQuitMenu()
{
    // ゲーム状態を終了メニュー表示中に遷移する
    setState(kAKGameStateQuitMenu);
}

/*!
 @brief 一時停止メニュー表示
 
 ゲームの状態を一時停止中に遷移する。
 */
void AKPlayingScene::viewPauseMenu()
{
    // ゲーム状態を一時停止中に遷移する
    setState(kAKGameStatePause);
}
