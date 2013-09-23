/*
 * Copyright (c) 2012-2013 Akihiro Kaneda.
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
 @file AKAppBankNetworkBanner.m
 @brief AppBankNetworkの広告バナー
 
 AppBankNetworkの広告バナーを管理するクラスを定義する。
 */

#import "AKAppBankNetworkBanner.h"
#import "AKLib.h"

/*!
 @brief AppBankNetworkの広告バナー
 
 AppBankNetworkの広告バナーを管理する。
 */
@implementation AKAppBankNetworkBanner

@synthesize delegate = delegate_;
@synthesize nadView = nadView_;

/*!
 @brief インスタンス解放処理
 
 インスタンス解放時の処理を行う。
 メンバを解放する。
 */
- (void)dealloc
{
    AKLog(kAKLogAppBankNetworkBanner_1, @"start");
    
    // 広告バナーのデリゲートを削除する
    self.nadView.delegate = nil;
    
    // メンバを解放する
    self.delegate = nil;
    self.nadView = nil;
    
    // スーパークラスの処理を実行する
    [super dealloc];
    
    AKLog(kAKLogAppBankNetworkBanner_1, @"end");
}

/*!
 @brief 広告バナー表示要求
 
 広告バナー表示要求があった時の処理。
 AddBankNetworkの広告バナーを生成する。
 @param adSize 広告サイズ
 @param serverParameter メディエーションで設定したパラメータ
 @param serverLabel メディエーションで設定したラベル
 @param request リクエスト情報
 */
- (void)requestBannerAd:(GADAdSize)adSize
              parameter:(NSString *)serverParameter
                  label:(NSString *)serverLabel
                request:(GADCustomEventRequest *)request
{
    AKLog(kAKLogAppBankNetworkBanner_1, @"広告バナー要求");
    
    // 広告バナーを生成する
    self.nadView = [[[NADView alloc] initWithFrame:CGRectMake(0, 0, adSize.size.width, adSize.size.height)] autorelease];
    
    // ログ出力を設定する
#ifdef DEBUG
    AKLog(kAKLogAppBankNetworkBanner_1, @"ログ有効化");
    [self.nadView setIsOutputLog:YES];
#else
    [self.nadView setIsOutputLog:NO];
#endif
    
    // apiKeyとspotIDを設定する
    [self.nadView setNendID:kAKAPIKey spotID:kAKSpotID];
    
    // デリゲートを設定する
    self.nadView.delegate = self;
    
    // 広告を読み込む
    [self.nadView load];
    
    AKLog(kAKLogAppBankNetworkBanner_1, @"end");
}

/*!
 @brief ロード完了処理
 
 広告読み込み完了時の処理。
 デリゲートに通知を行う。
 @param adView 広告バナー
 */
- (void)nadViewDidFinishLoad:(NADView *)adView
{
    AKLog(kAKLogAppBankNetworkBanner_1, @"広告読み込み完了");
    
    [self.delegate customEventBanner:self didReceiveAd:adView];
    
    AKLog(kAKLogAppBankNetworkBanner_1, @"end");
}

@end
