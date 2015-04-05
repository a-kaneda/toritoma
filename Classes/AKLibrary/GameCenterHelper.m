/*
 * Copyright (c) 2015 Akihiro Kaneda.
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
 @file GameCenterHelper.m
 @brief Game Centerアクセス
 
 Game Centerへのインターフェースを提供する。
 */

#import "GameCenterHelper.h"
#import "RootViewController.h"

static NSString* const LEADERBORD_ID = @"toritoma_score";

@implementation GameCenterHelper

/*!
 @brief ログイン
 
 Game Centerにログインしていない場合はログイン画面を表示する。
 */
+(void)login
{
    // ローカルアカウントを取得する
    GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
    
    // 認証イベントのハンドラを設定する
    localPlayer.authenticateHandler = ^(UIViewController *viewController, NSError *error) {
        
        // ログイン画面が渡された場合
        if (viewController != nil) {
            
            // RootViewControllerを取得し、ログイン画面をモーダル表示する。
            UIWindow *window = [UIApplication sharedApplication].keyWindow;
            if (window == nil) {
                window = [[UIApplication sharedApplication].windows objectAtIndex:0];
            }
            [window.rootViewController presentViewController:viewController animated:YES completion:nil];
        }
    };
}

/*!
 @brief ランキング画面表示
 
 Leaderboardを開く。
 */
+(void)openRanking
{
    // Game Centerビューを作成する
    GKGameCenterViewController *gameCenterController = [[GKGameCenterViewController alloc] init];
    if (gameCenterController != nil) {
        
        // RootViewControllerを取得する
        UIWindow *window = [UIApplication sharedApplication].keyWindow;
        if (window == nil) {
            window = [[UIApplication sharedApplication].windows objectAtIndex:0];
        }
        
        // RootViewControllerであることを確認する
        if ([window.rootViewController isKindOfClass:[RootViewController class]]) {
            
            // RootViewControllerにキャストする
            RootViewController *rootViewController = (RootViewController *)window.rootViewController;
            
            // Game Centerビューの設定を行う
            gameCenterController.gameCenterDelegate = rootViewController;
            gameCenterController.viewState = GKGameCenterViewControllerStateLeaderboards;
            gameCenterController.leaderboardTimeScope = GKLeaderboardTimeScopeToday;
            gameCenterController.leaderboardCategory = LEADERBORD_ID;
            
            // Game Centerビューをモーダル表示する
            [rootViewController presentViewController: gameCenterController animated: YES completion:nil];
        }
    }
}

/*!
 @brief ハイスコア送信
 
 Leaderboardにハイスコアを送信する。
 @param score スコア
 */
+(void)postHighScore:(int)score
{
    // スコアクラスを作成する
    GKScore *scoreReporter = [[GKScore alloc] initWithLeaderboardIdentifier:LEADERBORD_ID];
    scoreReporter.value = score;
    scoreReporter.context = 0;
    
    // スコアを送信する
    NSArray *scores = @[scoreReporter];
    [GKScore reportScores:scores withCompletionHandler:^(NSError *error) {
        
        if (error != nil) {
            NSLog(@"error post high score : %@", [error localizedDescription]);
        }
    }];
}

@end

