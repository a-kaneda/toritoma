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
 @file Payment.h
 @brief 課金処理クラス
 
 課金処理を行うクラスを定義する。
 */

#ifndef __toritoma__Payment__
#define __toritoma__Payment__

#include "PaymentDelegate.h"

namespace aklib {
    
    /*!
     @brief 課金処理クラス
     
     課金処理を行う。
     */
    class Payment {
    public:
        // 接続処理
        static void Open();
        // 切断処理
        static void Close();
        // プロダクトID追加
        static void AddProduct(const char *productID);
        // 課金処理が可能か確認する
        static bool CanMakePayments();
        // 金額の文字列を取得する
        static const char *GetPriceString(const char *productID);
        // 支払いを行う
        static bool Pay(const char *productID, PaymentDelegate *delegate);
        // リストアを行う
        static bool Restore(PaymentDelegate *delegate);
        // 支払い終了処理
        static void Finish();
    };
};

#endif /* defined(__toritoma__Payment__) */
