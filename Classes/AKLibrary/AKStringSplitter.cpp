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
 @brief 1文字のバイト数取得

 1文字のバイト数を取得する。
 utf-8文字を前提とする。
 2進表記にして先頭の1の個数をカウントする。
 ただし、0から始まるものは1byte文字のため1を返し、
 10から始まるものはマルチバイト文字の2byte目以降のため、
 エラーとして-1を返す。
 @param c utf-8の文字
 @return 1文字のバイト数
 */
int AKStringSplitter::getByteOfCharacter(char c)
{
    // utf-8の最大バイト数
    const int kAKMaxByte = 6;

    // 先頭の1のビット数を数える
    int i = 0;
    for (i = 0; i < kAKMaxByte; i++) {

        // 0が見つかった時点でループを抜ける
        if (((c >> (8 - i - 1)) & 0x01) == 0) {
            break;
        }
    }

    // 0から始まるものは1byte文字
    if (i == 0) {
        return 1;
    }
    // 10から始まるものはマルチバイト文字の2byte目以降
    else if (i == 1) {
        return -1;
    }
    // その他の場合は1の個数がバイト数
    else {
        return i;
    }
}

/*!
 @brief 文字数取得
 
 文字列の文字数を取得する。マルチバイト文字を1文字として扱う。
 文字列がUTF-8でない場合は1文字のバイト数が取得できず、エラーとして-1を返す。
 @param str 文字列
 @return 文字数
 */
int AKStringSplitter::getStringLength(const char *str)
{
    int length = 0;
    int i = 0;
    
    // NULL終端までループする
    while (str[i] != '\0') {
        
        // 文字数をカウントする
        length++;
        
        // 1文字のバイト数を取得する
        int byte = getByteOfCharacter(str[i]);
        
        // マイナスの場合は渡された文字列がUTF-8でないためエラー
        if (byte < 0) {
            return -1;
        }
        
        // 1文字のバイト数を取得し、その分だけインデックスを進める
        i += byte;
    }
    
    return length;
}

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
    
    // string型をchar型に変換する
    const char *orgChar = m_org.c_str();
    
    // \nは改行文字に変換して処理を行う
    if (m_position + 1 < m_org.size() && orgChar[m_position] == '\\' && orgChar[m_position + 1] == 'n') {
        
        // 出力文字列に改行文字を設定する
        m_char[0] = '\n';
        
        // 2文字分位置を進める
        m_position += 2;
    }
    else {
        
        // 1文字の長さを取得する
        int charLength = AKStringSplitter::getByteOfCharacter(orgChar[m_position]);
        
        // 出力文字列をクリアする
        memset(m_char, '\0', sizeof(m_char));
        
        // 出力文字列に1文字分の文字列を設定する
        strncpy(m_char, &orgChar[m_position], charLength);
        
        // 切り取った文字の位置を進める
        m_position += charLength;
    }

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
