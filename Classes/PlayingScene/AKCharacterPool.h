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
 @file AKCharacterPool.h
 @brief キャラクタープールクラス定義
 
 複数のキャラクターのメモリ管理を行うクラスを定義する。
 */

#ifndef AKCHARACTERPOOL_H
#define AKCHARACTERPOOL_H

#include "AKCharacter.h"

/*!
 @brief キャラクタープールクラス
 
 複数のキャラクターのメモリ管理を行う。
 */
template <class T>
class AKCharacterPool {
private:
    /// キャラクターを管理する配列
    std::vector<T*> m_pool;
    /// 配列サイズ
    int m_size;
    /// 次にキャラクターを追加するインデックス
    int m_next;
    
private:
    // デフォルトコンストラクタは使用禁止にする
    AKCharacterPool();
    
public:
    /*!
     @brief サイズを指定したコンストラクタ
     
     管理するプールのサイズを指定したコンストラクタ。
     @param size 管理するプールのサイズ
     */
    AKCharacterPool(int size) :
    m_size(size), m_next(0)
    {
        // キャラクターをあらかじめ作成しておく
        for (int i = 0; i < m_size; i++) {
            T *character = new T();
            m_pool.push_back(character);
        }
    }
    
    /*!
     @brief インスタンス解放時処理
     
     インスタンス解放時にオブジェクトを解放する。
     */
    ~AKCharacterPool()
    {
        // プールのメモリを解放する
        for (T *character : m_pool) {
            delete character;
        }
    }
    
    // 全キャラクター削除
    void reset();

    /*!
     @brief キャラクター配列取得
     
     キャラクター配列を取得する。
     @return キャラクター配列
     */
    std::vector<T*>* getPool()
    {
        return &m_pool;
    }
    
    /*!
     @brief 未使用キャラクター取得
     
     キャラクタープールの中から未使用のキャラクターを検索して返す。
     @return 未使用キャラクター。見つからないときはnilを返す。
     */
    T* getNext()
    {
        // 戻り値はNULLで初期化しておく
        T *ret = NULL;
        
        // 未使用のキャラクターを検索する
        for (int i = 0; i < m_size; i++) {
            
            // キャラクターを取得する
            T *work = m_pool.at(i);
            
            // インデックスを進める
            m_next = (m_next + 1) % m_size;
            
            // 使用中かどうか調べる
            if (!work->isStaged()) {
                // 戻り値に設定する
                ret = work;
                // 処理を終了する
                break;
            }
        }
        
        return ret;
    }
};

#endif