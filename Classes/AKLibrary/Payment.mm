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
 @file Payment.mm
 @brief 課金処理クラス
 
 課金処理を行うクラスを定義する。
 */

#include "Payment.h"
#import "Payment_objc.h"

using std::string;

namespace aklib
{
    // 接続処理
    void Payment::Open(PurchaseInfoWriter *writer)
    {
        // ネイティブ呼び出し用クラスのインスタンスを作成する
        Payment_objc *naitiveInstance = [Payment_objc sharedInstance];
        
        // 課金情報書き込みクラスを設定する
        naitiveInstance.writer = writer;
        
        // 課金処理のオブザーバーを登録する
        [[SKPaymentQueue defaultQueue] addTransactionObserver:naitiveInstance];
    }
    
    // 切断処理
    void Payment::Close()
    {
        // ネイティブ呼び出し用クラスのインスタンスを作成する
        Payment_objc *naitiveInstance = [Payment_objc sharedInstance];
        
        // 課金処理のオブザーバーを解除する
        [[SKPaymentQueue defaultQueue] removeTransactionObserver:naitiveInstance];
        
    }
    
    // プロダクト追加
    void Payment::AddProduct(const char *productID)
    {
        // ネイティブ呼び出し用クラスのインスタンスを作成する
        Payment_objc *naitiveInstance = [Payment_objc sharedInstance];

        // char配列をNSStringに変換する
        NSString *productIDString = [NSString stringWithUTF8String:productID];

        // プロダクト情報を要求する
        [naitiveInstance requestProduct:productIDString];
    }
    
    // 課金処理が可能か確認する
    bool Payment::CanMakePayments()
    {
        // ネイティブ呼び出し用クラスのインスタンスを作成する
        Payment_objc *naitiveInstance = [Payment_objc sharedInstance];

        // 課金処理が可能か確認し、結果を返す
        return [naitiveInstance canMakePayments];
    }
    
    // 価格文字列を取得する
    string Payment::GetPriceString(const char *productID)
    {
        // ネイティブ呼び出し用クラスのインスタンスを作成する
        Payment_objc *naitiveInstance = [Payment_objc sharedInstance];
        
        // char配列をNSStringに変換する
        NSString *productIDString = [NSString stringWithUTF8String:productID];
        
        // 価格文字列を取得する
        NSString *priceString = [naitiveInstance getPriceString:productIDString];
        
        // 戻り値を作成する
        if (priceString == nil) {
            string ret = "";
            return ret;
        }
        else {
            string ret = [priceString UTF8String];
            return ret;
        }
    }
    
    // 支払処理
    bool Payment::Pay(const char *productID, PaymentDelegate *delegate)
    {
        // ネイティブ呼び出し用クラスのインスタンスを作成する
        Payment_objc *naitiveInstance = [Payment_objc sharedInstance];
        
        // デリゲートを設定する
        naitiveInstance.delegate = delegate;
        
        // char配列をNSStringに変換する
        NSString *productIDString = [NSString stringWithUTF8String:productID];
        
        // 支払処理を実行し、結果を返す
        return [naitiveInstance pay:productIDString];
    }
    
    // リストア処理
    bool Payment::Restore(PaymentDelegate *delegate)
    {
        // ネイティブ呼び出し用クラスのインスタンスを作成する
        Payment_objc *naitiveInstance = [Payment_objc sharedInstance];
        
        // デリゲートを設定する
        naitiveInstance.delegate = delegate;
        
        // リストア処理を行い、結果を返す
        return [naitiveInstance restore];
    }
    
    // 終了処理
    void Payment::Finish()
    {
        // ネイティブ呼び出し用クラスのインスタンスを作成する
        Payment_objc *naitiveInstance = [Payment_objc sharedInstance];
        
        // デリゲートを設定する
        naitiveInstance.delegate = NULL;
    }
}