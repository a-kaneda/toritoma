#include "AppDelegate.h"
#include "AKTitleScene.h"
#include "AKPlayingScene.h"
#include "AKScreenSize.h"
#include "OnlineScore.h"
#include "SettingFileIO.h"

USING_NS_CC;
using CocosDenshion::SimpleAudioEngine;
//static cocos2d::Size designResolutionSize = cocos2d::Size(568, 320);
//static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
//static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
//static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("toritoma", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("toritoma");
#endif
        director->setOpenGLView(glview);
    }
    
    // タイルマップの隙間が表示される問題の対策
    director->getInstance()->setProjection(Director::Projection::_2D);
    director->getInstance()->setDepthTest(false);

    // turn on display FPS
    director->setDisplayStats(true);
    
    // 解像度を調整する
    AKScreenSize::init(glview);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // サウンドファイルを読み込む
    preloadSound();
    
    // UserDefaultsを読み込む
    SettingFileIO::GetInstance().ReadFile();

    // Set the design resolution
//    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
//    Size frameSize = glview->getFrameSize();
//    // if the frame's height is larger than the height of medium size.
//    if (frameSize.height > mediumResolutionSize.height)
//    {        
//        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
//    }
//    // if the frame's height is larger than the height of small size.
//    else if (frameSize.height > smallResolutionSize.height)
//    {        
//        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
//    }
//    // if the frame's height is smaller than the height of medium size.
//    else
//    {        
//        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
//    }

    register_all_packages();

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
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
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
    // カーソル移動時の効果音のファイルを読み込む
    SimpleAudioEngine::getInstance()->preloadEffect(kAKCursorSEFileName);
    
    // 音量を調整する
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);
}
