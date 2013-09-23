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
 @file AKNavigationController.h
 @brief UINavigationControllerのカスタマイズ
 
 UINavigationControllerのカスタマイズクラスを定義する。
 */

#import <UIKit/UIKit.h>
#import <GameKit/GameKit.h>
#import "AKLib.h"
#import "GADBannerView.h"
#import "GADBannerViewDelegate.h"

/// 広告バナーx方向の位置
enum AKBannerPosX {
    kAKBannerPosXLeft = 0,  ///< 左寄せ
    kAKBannerPosXCenter,    ///< 中央揃え
    kAKBannerPosXRight      ///< 右寄せ
};

/// 広告バナーy方向の位置
enum AKBannerPosY {
    kAKBannerPosYTop = 0,   ///< 上寄せ
    kAKBannerPosYBottom     ///< 下寄せ
};

// アプリのURL(アプリケーションで個別に定義する)
extern NSString *kAKAplUrl;
// AdMobパブリッシャーID(アプリケーションで個別に定義する)
extern NSString *kAKAdMobID;
// 広告バナーx方向の位置(アプリケーションで個別に定義する)
extern enum AKBannerPosX kAKBannerPosX;
// 広告バナーy方向の位置(アプリケーションで個別に定義する)
extern enum AKBannerPosY kAKBannerPosY;

// 広告表示位置取得処理(アプリケーションで個別に定義する)
CGRect bannerRect(void);

// UINavigationControllerのカスタマイズ
@interface AKNavigationController : UINavigationController
<GKLeaderboardViewControllerDelegate, GKAchievementViewControllerDelegate, GADBannerViewDelegate> {

    /// 広告バナー
    GADBannerView *bannerView_;
    /// 広告表示
    BOOL isViewBanner_;
}

/// 広告バナー
@property (nonatomic, retain)GADBannerView *bannerView;

// 広告バナーを作成
- (void)createAdBanner;
// 広告バナーを削除
- (void)deleteAdBanner;
// 広告バナー表示
- (void)viewAdBanner;
// 広告バナー非表示
- (void)hiddenAdBanner;
// 広告バナー位置x座標取得
- (float)bannerPosX;
// 広告バナー位置y座標取得
- (float)bannerPosY;
// Twitter Viewの表示
- (void)viewTwitterWithInitialString:(NSString *)string;
@end
