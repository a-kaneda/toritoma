/*
 * Copyright (c) 2013 Akihiro Kaneda.
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
 @file AKStringSplitter.h
 @brief 文字列分割クラス
 
 文字列を1文字ずつ分割するクラスを定義する。
 */

#ifndef AKSTRINGSPLITTER_H
#define AKSTRINGSPLITTER_H

#include <stdlib.h>
#include <limits.h>
#include <string>

/*!
 @brief 文字列分割クラス
 
 文字列を1文字ずつ分割する。マルチバイト文字に対応する。
 */
class AKStringSplitter {
public:
    // 1文字のバイト数取得
    static int getByteOfCharacter(char c);
    // 文字数取得
    static int getStringLength(const char *str);

private:
    /// 元の文字列
    const std::string m_org;
    /// 切り取った1文字
    char m_char[MB_LEN_MAX + 1];
    /// 切り取った文字の位置
    int m_position;

private:
    // デフォルトコンストラクタは使用禁止にする
    AKStringSplitter();

public:
    // 元の文字列を指定したコンストラクタ
    AKStringSplitter(const std::string org);
    // 1文字切り取る
    const char *split();
    // 現在位置の1byte取得
    char getCurrentByte();
};

/*!
 @brief 文字列管理クラス
 */

#endif
