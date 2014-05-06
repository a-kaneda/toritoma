/*
 * Copyright (c) 2014 Akihiro Kaneda.
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
 @file LocalizedResource.h
 @brief リソースのローカライズ
 
 リソースのローカライズを管理する。
 */

#ifndef LOCALIZEDRESOURCE_H
#define LOCALIZEDRESOURCE_H

#include "AKCommon.h"

namespace aklib {
    
    class LocalizedResource {
    private:
        /// ローカライズ文字列マップ
        std::map<std::string, std::string> _localizedStrings;
        
    private:
        /*!
         @brief コンストラクタ
         
         シングルトンのため、外部からの使用を禁止する。
         */
        LocalizedResource();
        
        /*!
         @brief コピーコンストラクタ
         
         シングルトンのため、外部からの使用を禁止する。
         @param other コピー元インスタンス
         */
        LocalizedResource(const LocalizedResource &other){}
        
        /*!
         @brief オペレータ =
         
         シングルトンのため、外部からの使用を禁止する。
         @param other コピー元インスタンス
         @return 自インスタンス
         */
        LocalizedResource& operator=(const LocalizedResource &other){ return *this; }
        
    public:
        /*!
         @brief インスタンス取得
         
         シングルトンオブジェクトを取得する。
         @return シングルトンオブジェクト
         */
        static LocalizedResource& getInstance();
        
        /*!
         @brief ローカライズ文字列取得
         
         キーを指定して、ローカライズされた文字列を取得する。
         @param key キー
         @return ローカライズされた文字列
         */
        const std::string& getString(const std::string &key) const;
        
    private:
        /*!
         @brief ローカライズ文字列ファイル名取得
         
         言語設定に対応したローカライズ文字列ファイルのファイル名を取得する。
         @return ローカライズ文字列ファイル名
         */
        std::string getStringFileName() const;
        
        /*!
         @brief ローカライズ文字列読込
         
         ローカライズ文字列ファイルのテキストファイルを読み込み、キーと文字列のマップを作成する。
         @param stringData ローカライズ文字列ファイルのデータ
         @param localizedStrings ローカライズ文字列マップ
         */
        void readStringData(const std::string &stringData, std::map<std::string, std::string> &localizedStrings) const;
    };
    
}

#endif
