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
 @file Twitter.mm
 @brief Twitter管理
 
 Twitterサービスへのインターフェースを提供する。
 */

#include "Twitter.h"
#include "RootViewController.h"

namespace aklib {
    
    /*!
     @brief 投稿ビュー表示
     
     ネイティブコードのTwitter投稿処理を呼び出す。
     @param message 初期投稿メッセージ
     @param url URL
     @param imagepath 投稿画像のファイルパス
     */
    void Twitter::post(const char *message, const char *url, const char *imagepath)
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
            
            // 投稿画像を開く
            UIImage *image = [UIImage imageWithContentsOfFile:[NSString stringWithCString:imagepath encoding:NSUTF8StringEncoding]];
            
            // ネイティブコードのTwitter投稿ビュー表示処理を呼び出す
            [rootViewController postTwitterMessage:[NSString stringWithCString:message encoding:NSUTF8StringEncoding]
                                               URL:[NSURL URLWithString:[NSString stringWithCString:url encoding:NSUTF8StringEncoding]]
                                             Image:image];            
        }
    }
};
