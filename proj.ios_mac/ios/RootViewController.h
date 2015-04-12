/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import <UIKit/UIKit.h>
#import <GameKit/GameKit.h>
#import "GoogleMobileAds/GADBannerView.h"
#import "GoogleMobileAds/GADInterstitial.h"

@interface RootViewController : UIViewController<GKGameCenterControllerDelegate, GADInterstitialDelegate> {
    
    // AdMobバナー広告
    GADBannerView *bannerView_;
    // AdMobインタースティシャル広告
    GADInterstitial *interstitialView_;
}

/// AdMobバナー広告
@property (nonatomic, retain)GADBannerView *bannerView;
/// AdMobインタースティシャル広告
@property (nonatomic, retain)GADInterstitial *interstitialView;


- (BOOL) prefersStatusBarHidden;

// バナー広告を作成
- (void)createAdBanner;
// バナー広告を削除
- (void)deleteAdBanner;
// インタースティシャル広告を作成
- (void)createAdInterstitial;
// インタースティシャル広告を表示
- (void)viewAdInterstitial;

@end
