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

namespace aklib
{
    /*!
     @brief 接続処理
     
     課金処理の接続処理を行う。
     オブザーバーの登録を行う。
     */
    void Payment::Open()
    {
        // ネイティブ呼び出し用クラスのインスタンスを作成する
        Payment_objc *naitiveInstance = [Payment_objc sharedInstance];
        
        // 課金処理のオブザーバーを登録する
        [[SKPaymentQueue defaultQueue] addTransactionObserver:naitiveInstance];
    }
    
    /*!
     @brief 切断処理
     
     課金処理の切断処理を行う。
     オブザーバーの解除を行う。
     */
    void Payment::Close()
    {
        // ネイティブ呼び出し用クラスのインスタンスを作成する
        Payment_objc *naitiveInstance = [Payment_objc sharedInstance];
        
        // 課金処理のオブザーバーを解除する
        [[SKPaymentQueue defaultQueue] removeTransactionObserver:naitiveInstance];
        
    }
    
    /*!
     @brief プロダクト追加

     処理対象のプロダクトを追加する。
     プロダクト情報の要求を行う。
     */
    void Payment::AddProduct(const char *productID)
    {
        // ネイティブ呼び出し用クラスのインスタンスを作成する
        Payment_objc *naitiveInstance = [Payment_objc sharedInstance];

        // char配列をNSStringに変換する
        NSString *productIDString = [NSString stringWithUTF8String:productID];

        // プロダクト情報を要求する
        [naitiveInstance requestProduct:productIDString];
    }
    
    /*!
     @brief 課金処理が可能か確認する
     
     課金処理が可能か確認する。
     @return 課金処理が可能ならtrue、不可能ならfalse
     */
    bool Payment::CanMakePayments()
    {
        // ネイティブ呼び出し用クラスのインスタンスを作成する
        Payment_objc *naitiveInstance = [Payment_objc sharedInstance];

        // 課金処理が可能か確認し、結果を返す
        return [naitiveInstance canMakePayments];
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
    const char *Payment::GetPriceString(const char *productID)
    {
        // ネイティブ呼び出し用クラスのインスタンスを作成する
        Payment_objc *naitiveInstance = [Payment_objc sharedInstance];
        
        // char配列をNSStringに変換する
        NSString *productIDString = [NSString stringWithUTF8String:productID];
        
        // 価格文字列を取得して返す
        return [[naitiveInstance getPriceString:productIDString] UTF8String];
    }
    
    /*!
     @brief 支払処理
     
     指定したプロダクトIDの支払いを行う。
     支払処理終了を通知するためのデリゲートを設定する。
     @param productID プロダクトID
     @param delegate 支払処理終了受信デリゲート
     @return 正常終了時true、異常終了時false
     */
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
    
    /*!
     @brief リストア処理
     
     リストア処理を行う。
     支払処理終了を通知するためのデリゲートを設定する。
     @param delegate 支払処理終了受信デリゲート
     @return 正常終了時true、異常終了時false
     */
    bool Payment::Restore(PaymentDelegate *delegate)
    {
        // ネイティブ呼び出し用クラスのインスタンスを作成する
        Payment_objc *naitiveInstance = [Payment_objc sharedInstance];
        
        // デリゲートを設定する
        naitiveInstance.delegate = delegate;
        
        // リストア処理を行い、結果を返す
        return [naitiveInstance restore];
    }
    
    /*!
     @brief 終了処理
     
     支払い終了時に実行する。
     ネイティブコードに設定したデリゲートを初期化する。
     */
    void Payment::Finish()
    {
        // ネイティブ呼び出し用クラスのインスタンスを作成する
        Payment_objc *naitiveInstance = [Payment_objc sharedInstance];
        
        // デリゲートを設定する
        naitiveInstance.delegate = NULL;
    }
}