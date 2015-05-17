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

#import <Social/Social.h>
#import "RootViewController.h"
#import "cocos2d.h"
#import "CCEAGLView.h"

/// AdMobパブリッシャーID
static NSString *kAKAdMobID = @"ca-app-pub-8055460627535570/8664101241";
/// テストデバイスID
static NSString *kAKTestDeviceID = @"ab81ab90de92aeeb1e4b20b4647be22883e80300";
static NSString *kAKTestDeviceID2 = @"59d89c955b8adbe31a45ec3f07ad5ea813b11c24";

@implementation RootViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}

*/
// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskAllButUpsideDown;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];

    cocos2d::GLView *glview = cocos2d::Director::getInstance()->getOpenGLView();

    if (glview)
    {
        CCEAGLView *eaglview = (CCEAGLView*) glview->getEAGLView();

        if (eaglview)
        {
            CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);
            cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) s.width, (int) s.height);
        }
    }
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    
    self.bannerView = nil;
    [super dealloc];
}

/*!
 @brief リーダーボードで完了タップ時の処理
 
 リーダーボードで完了タップ時の処理。前の画面に戻る。
 @param gameCenterViewController The view controller the player finished interacting with.
 */
- (void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)gameCenterViewController
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

/*!
 @brief バナー広告を作成
 
 バナー広告を作成する。
 */
- (void)createAdBanner
{
    // すでに作成済みの場合は処理しない
    if (self.bannerView != nil) {
        return;
    }
    
    // 画面下部に標準サイズのビューを作成する
    self.bannerView = [[[GADBannerView alloc] initWithAdSize:kGADAdSizeBanner] autorelease];
    
    // 広告の「ユニット ID」を指定する。これは AdMob パブリッシャー ID です。
    self.bannerView.adUnitID = kAKAdMobID;
    
    // ユーザーに広告を表示した場所に後で復元する UIViewController をランタイムに知らせて
    // ビュー階層に追加する。
    self.bannerView.rootViewController = self;
    [self.view addSubview:self.bannerView];
    
    // 広告リクエストを作成する
    GADRequest *request = [GADRequest request];
    
    // テスト広告のリクエストを行う。
    request.testDevices = [NSArray arrayWithObjects:kAKTestDeviceID, kAKTestDeviceID2, nil];
    
    // リクエストを行って広告を読み込む
    [self.bannerView loadRequest:request];
}

/*!
 @brief バナー広告を削除
 
 バナー広告を削除する。
 */
- (void)deleteAdBanner
{
    // バナーを取り除く
    [self.bannerView removeFromSuperview];
    
    // バナーを削除する
    self.bannerView = nil;
}

/*!
 @brief インタースティシャル広告を作成
 
 インタースティシャル広告を作成する。
 */
- (void)createAdInterstitial
{
    // インタースティシャル広告を作成する
    self.interstitialView = [[[GADInterstitial alloc] init] autorelease];
    
    // 広告のユニットIDを指定する。
    self.interstitialView.adUnitID = kAKAdMobID;
    
    // デリゲートを設定する
    self.interstitialView.delegate = self;
    
    // 広告リクエストを作成する
    GADRequest *request = [GADRequest request];
    
    // テスト広告のリクエストを行う。
    request.testDevices = [NSArray arrayWithObjects:kAKTestDeviceID, kAKTestDeviceID2, nil];
    
    // リクエストを行って広告を読み込む
    [self.interstitialView loadRequest:request];
}

/*!
 @brief インタースティシャル広告を表示
 
 インタースティシャル広告を表示する。
 */
- (void)viewAdInterstitial
{
    // インタースティシャル広告を表示する準備ができている場合、
    // 広告を表示する。
    if ([self.interstitialView isReady]) {
        [self.interstitialView presentFromRootViewController:self];
    }
}

/*!
 @brief インタースティシャル広告表示終了時処理
 
 インタースティシャル広告の表示が終了した時に、再度インタースティシャル広告の作成を行う。
 @param ad インタースティシャル広告
 */
- (void)interstitialDidDismissScreen:(GADInterstitial *)ad
{
    [self createAdInterstitial];
}

/*!
 @brief Twitter投稿
 
 Twitter投稿ビューを表示する。
 @param message 初期投稿メッセージ
 @param url URL
 @param image 投稿画像
 */
- (void)postTwitterMessage:(NSString *)message URL:(NSURL *)url Image:(UIImage *)image
{
    // Twitterが使用可能な場合のみ処理を行う
    if ([SLComposeViewController isAvailableForServiceType:SLServiceTypeTwitter]) {
        
        // Twitter投稿ビューを作成する
        SLComposeViewController *composeVC = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeTwitter];
        
        // 完了時のハンドラを設定する
        [composeVC setCompletionHandler:^(SLComposeViewControllerResult result) {
            
            // Twitter投稿ビューを閉じる
            [self dismissViewControllerAnimated:YES completion:nil];
            
        }];
        
        // 投稿するテキストの初期設定を行う
        [composeVC setInitialText:message];
        
        // 投稿するURLの設定を行う
        if (url != nil) {
            [composeVC addURL:url];
        }
        
        // 投稿する画像の設定を行う
        if (image != nil) {
            [composeVC addImage:image];
        }
        
        // Twitter投稿ビューを表示する
        [self presentViewController:composeVC animated:YES completion:nil];
    }
}

@end
