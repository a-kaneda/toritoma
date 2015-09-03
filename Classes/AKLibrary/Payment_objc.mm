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
 @file Payment_objc.m
 @brief 課金処理クラス（iOSネイティブコード）
 
 課金処理を行うクラスを定義する。iOSネイティブコードを呼び出す。
 */

#import "Payment_objc.h"
#include "cocos2d.h"

using cocos2d::UserDefault;

@implementation Payment_objc

// シングルトンオブジェクト
static Payment_objc *sharedInstance_ = nil;

@synthesize products;
@synthesize delegate;

/*!
 @brief シングルトンオブジェクト取得
 
 シングルトンオブジェクトを取得する。
 まだ生成されていない場合は生成を行う。
 @return シングルトンオブジェクト
 */
+ (Payment_objc *)sharedInstance
{
    // 他のスレッドで同時に実行されないようにする
    @synchronized(self) {
        
        // シングルトンオブジェクトが生成されていない場合は生成する
        if (!sharedInstance_) {
            sharedInstance_ = [[Payment_objc alloc] init];
        }
        
        return sharedInstance_;
    }
    
    return nil;
}

/*!
 @brief インスタンス初期化処理
 
 メンバ変数の初期化を行う。
 @return 初期化したインスタンス
 */
- (instancetype)init
{
    // スーパークラスの処理を実行する
    self = [super init];
    if (!self) {
        return nil;
    }
    
    // メンバ変数を初期化する
    self.products = [NSMutableDictionary dictionary];
    
    return self;
}

/*!
 @brief インスタンス解放処理
 
 インスタンス解放時の処理を行う。
 メンバ変数の解放処理を行う。
 */
- (void)dealloc
{
    // メンバ変数を解放する
    self.products = nil;
    
    // スーパークラスの処理を実行する
    [super dealloc];
}

/*!
 @brief 課金処理が可能か確認する
 
 課金処理が可能か確認する。
 @return 課金処理が可能ならtrue、不可能ならfalse
 */
- (bool)canMakePayments
{
    // 課金処理が可能か確認する
    if ([SKPaymentQueue canMakePayments]) {
        return true;
    }
    else {
        return false;
    }
}

/*!
 @brief プロダクト情報要求
 
 プロダクト情報を要求する。
 @param productID プロダクトID
 */
- (void)requestProduct:(NSString *)productID
{
    // iTunes connectにて登録したプロダクトIDのリストを作る
    NSSet *identifiers = [NSSet setWithObjects:productID, nil];

    // プロダクト情報リクエストをプロダクトIDを元に作成する
    SKProductsRequest *productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:identifiers];
    
    // プロダクト情報受信のためのデリゲートを設定する
    productsRequest.delegate = self;
    
    // 要求を開始する
    [productsRequest start];
}

/*!
 @brief プロダクト情報受信
 
 プロダクト情報を受信する。
 SKProductRequestのデリゲート。
 取得したいプロダクト情報をメンバ変数に格納する。
 @param request リクエスト
 @param response プロダクト情報
 */
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    // 応答の中から必要なプロダクト情報を取得する
    for (SKProduct *product in response.products) {
        
        // メンバ変数に格納する
        self.products[product.productIdentifier] = product;
    }
}

/*!
 @brief 価格文字列を取得する
 
 プロダクト情報から価格文字列を取得する。
 価格に表示すべき内容はロケール設定と一致するとは限らない。
 例えば、アメリカに住んでいる日本人は日本語設定でiphoneを使用しているかもしれないが、
 価格は現地の$で表示するべきである。
 そのため、価格部分についてはiTunesから取得したプロダクト情報を元に表示を行う必要がある。
 @param productID プロダクトID
 @return 価格文字列
 */
- (NSString *)getPriceString:(NSString *)productID
{
    // プロダクト情報を取得する
    SKProduct *product = self.products[productID];
    
    // 取得できない場合はnilを返す
    if (product == nil) {
        return nil;
    }
    
    // NSNumberFormatterのインスタンスを作成する
    NSNumberFormatter *numberFormatter = [[[NSNumberFormatter alloc] init] autorelease];
    
    // 動作をOS X 10.4以降にする
    [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
    
    // ロケールで設定した通過形式とする
    [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
    
    // プロダクト情報から価格のロケール設定を取得して使用する
    [numberFormatter setLocale:product.priceLocale];
    
    // プロダクト情報の価格をロケールに対応した文字列に変換する
    NSString *formattedPrice = [numberFormatter stringFromNumber:product.price];
    
    // 作成した文字列を返す
    return formattedPrice;
}

/*!
 @brief 支払処理
 
 指定したプロダクトIDの支払いを行う。
 支払情報を作成し、キューに登録する。
 @param productID プロダクトID
 @return 正常終了時true、異常終了時false
 */
- (bool)pay:(NSString *)productID
{
    // プロダクト情報を取得する
    SKProduct *product = self.products[productID];
    
    // 取得できない場合は支払い失敗とする
    if (product == nil) {
        return false;
    }
    
    // 支払情報をプロダクト情報から作成する
    SKMutablePayment *payment = [SKMutablePayment paymentWithProduct:product];
    
    // 購入個数は1個とする
    payment.quantity = 1;
    
    // 支払情報をキューに追加する
    [[SKPaymentQueue defaultQueue] addPayment:payment];
    
    return true;
}

/*!
 @brief リストア処理
 
 リストア要求を行う。
 @return 正常終了時true、異常終了時false
 */
- (bool)restore
{
    // リストア要求を行う
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
    
    return true;
}

/*!
 @brief 支払いキュー状態変更受信処理
 
 支払いキューの状態変化を受信し、トランザクション終了処理を行う。
 @param queue ペイメントキュー
 @param transactions トランザクション
 */
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    // 受信した各トランザクションを処理する
    for (SKPaymentTransaction *transaction in transactions) {
        
        // トランザクションの状態に応じて処理を分岐する
        switch (transaction.transactionState) {
                
            case SKPaymentTransactionStatePurchasing:
                break;
            case SKPaymentTransactionStateDeferred:
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                [self completeTransaction:transaction];
                break;
            default:
                break;
        }
    }
}

/*!
 @brief トランザクション完了処理
 
 トランザクション完了時の処理を行う。
 UserDefaultに書き込みを行い、ペイメントキューとデリゲートに終了を通知する。
 @param transaction トランザクション
 */
- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
    // プロダクト情報を持っている場合はUserDefaultに書き込む
    if (self.products[transaction.payment.productIdentifier] != nil) {
        
        // UserDefaultを取得する
        UserDefault *userdefault = UserDefault::getInstance();
        
        // UserDefaultに書き込む
        userdefault->setBoolForKey([transaction.payment.productIdentifier UTF8String], true);
    }

    // デリゲートが設定されていれば終了を通知する
    if (self.delegate != NULL) {
        self.delegate->completePayment();
    }
    
    // ペイメントキューにトランザクション終了を通知する
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

/*!
 @brief トランザクション失敗処理
 
 トランザクションが失敗した時の処理を行う。
 ペイメントキューとデリゲートに終了を通知する。
 @param transaction トランザクション
 */
- (void)failedTransaction:(SKPaymentTransaction *)transaction
{
    // デリゲートが設定されていれば終了を通知する
    if (self.delegate != NULL) {
        self.delegate->completePayment();
    }
    
    // ペイメントキューにトランザクション終了を通知する
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

@end
