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
 @file Advertisement.mm
 @brief 広告管理
 
 広告サービスへのインターフェースを提供する。
 */

#include "Advertisement.h"
#include "RootViewController.h"

namespace aklib {

    // 広告が無効化されているかどうか
    bool Advertisement::_isDisabled = false;
    
    // バナー広告表示
    void Advertisement::viewBanner()
    {
        // 広告無効化フラグが立っている場合は無処理とする
        if (_isDisabled) {
            return;
        }
        
        // RootViewControllerを取得する
        UIWindow *window = [UIApplication sharedApplication].keyWindow;
        if (window == nil) {
            window = [[UIApplication sharedApplication].windows objectAtIndex:0];
        }
        
        // RootViewControllerであることを確認する
        if ([window.rootViewController isKindOfClass:[RootViewController class]]) {
            
            // RootViewControllerにキャストする
            RootViewController *rootViewController = (RootViewController *)window.rootViewController;
            
            // ネイティブコードのバナー広告表示処理を呼び出す
            [rootViewController createAdBanner];
        }
    }
    
    // バナー広告消去
    void Advertisement::hideBanner()
    {
        // RootViewControllerを取得する
        UIWindow *window = [UIApplication sharedApplication].keyWindow;
        if (window == nil) {
            window = [[UIApplication sharedApplication].windows objectAtIndex:0];
        }
        
        // RootViewControllerであることを確認する
        if ([window.rootViewController isKindOfClass:[RootViewController class]]) {
            
            // RootViewControllerにキャストする
            RootViewController *rootViewController = (RootViewController *)window.rootViewController;
            
            // ネイティブコードのバナー広告消去処理を呼び出す
            [rootViewController deleteAdBanner];
        }
    }
    
    // インタースティシャル広告表示
    void Advertisement::viewInterstatial()
    {
        // 広告無効化フラグが立っている場合は無処理とする
        if (_isDisabled) {
            return;
        }
        
        // RootViewControllerを取得する
        UIWindow *window = [UIApplication sharedApplication].keyWindow;
        if (window == nil) {
            window = [[UIApplication sharedApplication].windows objectAtIndex:0];
        }
        
        // RootViewControllerであることを確認する
        if ([window.rootViewController isKindOfClass:[RootViewController class]]) {
            
            // RootViewControllerにキャストする
            RootViewController *rootViewController = (RootViewController *)window.rootViewController;
            
            // ネイティブコードのインタースティシャル広告表示処理を呼び出す
            [rootViewController viewAdInterstitial];
        }
    }
    
    // 広告無効化
    void Advertisement::disableAds()
    {
        // 広告無効化フラグを立てる
        _isDisabled = true;
        
        // 広告を削除する
        hideBanner();
    }
}
