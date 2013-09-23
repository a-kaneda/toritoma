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
 @file AKString.m
 @brief 文字列管理クラス
 
 文字列を管理するクラスを定義する。
 */

#include "AKStringSplitter.h"

/*!
 @brief 元の文字列を指定したコンストラクタ
 
 元の文字列を指定したコンストラクタ。
 @param org 元の文字列
 */
AKStringSplitter::AKStringSplitter(const std::string org) :
m_org(org), m_char(""), m_position(0)
{
    
}

/*!
 @brief 1文字切り取る
 
 1文字切り取って、その文字列のポインタを返す。
 次回この関数を呼んだ時に値が上書きされるため、その前に呼び出し元でコピーをしておくこと。
 すでにすべての文字を切り取り終わっている場合はNULLを返す。
 @return 切り取った文字
 */
const char *AKStringSplitter::split()
{
    // すでにすべての文字を切り取っている場合はNULLを返す
    if (m_position >= m_org.size()) {
        return NULL;
    }
    
    // ロケールを設定する
    setlocale(LC_CTYPE, "ja_JP.UTF-8");
    
    // string型をchar型に変換する
    const char *orgChar = m_org.c_str();
    
    // 1文字の長さを取得する
    int charLength = mblen(&orgChar[m_position], MB_CUR_MAX);
    
    // 出力文字列をクリアする
    memset(m_char, '\0', sizeof(m_char));
    
    // 出力文字列に1文字分の文字列を設定する
    strncpy(m_char, &orgChar[m_position], charLength);
    
    // 切り取った文字の位置を進める
    m_position += charLength;
    
    // 切り取った文字を返す
    return m_char;
}

/*!
 @brief 現在位置の1byte取得
 
 現在位置の1byte分の文字を取得する。
 すでにすべての文字を切り取り終わっている場合はNULL文字を返す。
 @return 現在位置の1byte
 */
char AKStringSplitter::getCurrentByte()
{
    // すでにすべての文字を切り取っている場合はNULLを返す
    if (m_position >= m_org.size()) {
        return '\0';
    }

    // 現在位置の1byteを返す
    return m_org.at(m_position);
}