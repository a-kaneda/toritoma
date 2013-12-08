#include "AppDelegate.h"
#include "AKTitleScene.h"
#include "AKPlayingScene.h"
#include "AKScreenSize.h"

USING_NS_CC;
using CocosDenshion::SimpleAudioEngine;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    pDirector->setDisplayStats(true);
    
    // 解像度を調整する
    AKScreenSize::init();

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // サウンドファイルを読み込む
    preloadSound();

    // create a scene. it's an autorelease object
    CCScene *pScene = AKTitleScene::create();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // 実行中のシーンを取得する
    CCScene *scene = CCDirector::sharedDirector()->getRunningScene();

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
        snprintf(fileName, sizeof(fileName), kAKStageBGMFileName, i);

        // ファイルを読み込む
        SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(fileName);
    }
    /// エンディングBGMのファイル名
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(kAKEndingBGMFileName);
    /// ステージクリアジングルのファイル名
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(kAKClearJingleFileName);
    /// ボタン選択効果音のファイル名
    SimpleAudioEngine::sharedEngine()->preloadEffect(kAKSelectSEFileName);
    /// ポーズ効果音のファイル名
    SimpleAudioEngine::sharedEngine()->preloadEffect(kAKPauseSEFileName);
    /// 爆発(小)効果音のファイル名
    SimpleAudioEngine::sharedEngine()->preloadEffect(kAKBombMinSEFileName);
    /// 爆発(大)効果音のファイル名
    SimpleAudioEngine::sharedEngine()->preloadEffect(kAKBombBigSEFileName);
}
