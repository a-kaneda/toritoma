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
 @file OnlineScore.cpp
 @brief オンラインスコア管理
 
 オンラインスコア管理サービスへのインターフェースを提供する。
 */

#include "OnlineScore.h"
#include <assert.h>
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#define JNICLASSNAME "org/cocos2dx/cpp/AppActivity"

using cocos2d::JniMethodInfo;
using cocos2d::JniHelper;

namespace aklib {
    
    // ログイン
    void OnlineScore::login()
    {
        // ActivityのonStartでログイン処理を実施するため、ここでは何もしない
    }
    
    // ランキング画面表示
    void OnlineScore::openRanking()
    {
        // ネイティブコードのランキング画面表示関数を取得する
        JniMethodInfo methodInfo;
        if (!JniHelper::getStaticMethodInfo(methodInfo, JNICLASSNAME, "openRanking", "()V"))
        {
            assert(false);
            return;
        }
        
        // 関数を呼び出す
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
        
        // リソースを解放する
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
    
    // ハイスコア送信
    void OnlineScore::postHighScore(int score)
    {
        // ネイティブコードのハイスコア送信関数を取得する
        JniMethodInfo methodInfo;
        if (!JniHelper::getStaticMethodInfo(methodInfo, JNICLASSNAME, "postHighScore", "(I)V"))
        {
            assert(false);
            return;
        }
        
        // 関数を呼び出す
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, score);
        
        // リソースを解放する
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}
