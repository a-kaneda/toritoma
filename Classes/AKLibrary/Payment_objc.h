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
 @file Payment_objc.h
 @brief 課金処理クラス（iOSネイティブコード）
 
 課金処理を行うクラスを定義する。iOSネイティブコードを呼び出す。
 */

#import <StoreKit/StoreKit.h>
#include "PaymentDelegate.h"

// 課金処理クラス（iOSネイティブコード）
@interface Payment_objc : NSObject<SKProductsRequestDelegate, SKPaymentTransactionObserver> {
    // プロダクト情報
    NSMutableDictionary *products_;
    // デリゲート
    aklib::PaymentDelegate *delegate_;
}

@property (nonatomic, retain)NSMutableDictionary *products;
@property (nonatomic, assign)aklib::PaymentDelegate *delegate;

// シングルトンオブジェクトを取得する
+ (Payment_objc *)sharedInstance;
// 課金処理が可能か確認する
- (bool)canMakePayments;
// プロダクト情報要求
- (void)requestProduct:(NSString *)productID;
// 価格文字列取得
- (NSString *)getPriceString:(NSString *)productID;
// 支払いを行う
- (bool)pay:(NSString *)productID;
// トランザクション終了処理
- (void)completeTransaction:(SKPaymentTransaction *)transaction;
// トランザクション失敗処理
- (void)failedTransaction:(SKPaymentTransaction *)transaction;

@end

