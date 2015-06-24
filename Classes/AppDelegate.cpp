#include "AppDelegate.h"
#include "AKTitleScene.h"
#include "AKPlayingScene.h"
#include "AKScreenSize.h"
#include "OnlineScore.h"

USING_NS_CC;
using CocosDenshion::SimpleAudioEngine;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);
    
    // 解像度を調整する
    AKScreenSize::init(glview);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // サウンドファイルを読み込む
    preloadSound();

    // create a scene. it's an autorelease object
    auto scene = AKTitleScene::create();

    // run
    director->runWithScene(scene);
    
    // オンラインスコア管理にログインする
    aklib::OnlineScore::login();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // 実行中のシーンを取得する
    Scene *scene = Director::getInstance()->getRunningScene();

    // ゲームプレイシーンの場合は固有のバックグラウンド移行処理を実行する
    AKPlayingScene *playingScene = dynamic_cast<AKPlayingScene*>(scene);
    if (playingScene != NULL) {
        playingScene->onWillEnterForeground();
    }
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

/*!
 @brief サウンドファイルのプリロード

 サウンドファイルの読み込みを行う。
 */
void AppDelegate::preloadSound()
{
    // ステージのBGMを読み込む
    for (int i = 0; i < kAKStageBGMCount; i++) {

        // ファイル名を作成する
        char fileName[32] = "";
        snprintf(fileName, sizeof(fileName), kAKStageBGMFileName, i + 1);

        // ファイルを読み込む
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(fileName);
    }
    // ステージクリアジングルのファイルを読み込む
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(kAKClearJingleFileName);
    // ゲームクリアジングルのファイルを読み込む
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(kAKGameClearJingleFileName);
    // ボタン選択効果音のファイルを読み込む
    SimpleAudioEngine::getInstance()->preloadEffect(kAKSelectSEFileName);
    // ポーズ効果音のファイルを読み込む
    SimpleAudioEngine::getInstance()->preloadEffect(kAKPauseSEFileName);
    // 爆発(小)効果音のファイルを読み込む
    SimpleAudioEngine::getInstance()->preloadEffect(kAKBombMinSEFileName);
    // 爆発(大)効果音のファイルを読み込む
    SimpleAudioEngine::getInstance()->preloadEffect(kAKBombBigSEFileName);
    // 弾衝突効果音のファイルを読み込む
    SimpleAudioEngine::getInstance()->preloadEffect(kAKHitSEFileName);
    // 自機死亡時の効果音のファイルを読み込む
    SimpleAudioEngine::getInstance()->preloadEffect(kAKMissSEFileName);
    // 1Up時の効果音のファイルを読み込む
    SimpleAudioEngine::getInstance()->preloadEffect(kAK1UpSEFileName);
    
    // 音量を調整する
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);
}