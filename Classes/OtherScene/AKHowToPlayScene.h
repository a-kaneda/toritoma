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
 @file AKHowToPlayScene.h
 @brief プレイ方法画面シーンクラスの定義
 
 プレイ方法画面シーンクラスを定義する。
 */

#ifndef AKHOWTOPLAYSCENE_H
#define AKHOWTOPLAYSCENE_H

#include "AKToritoma.h"
#include "PageScene.h"
#include "base/CCController.h"

/*!
 @brief プレイ方法画面シーン
 
 プレイ方法画面のシーンを実現する。
 */
class AKHowToPlayScene : public PageScene {
public:
    
    /*!
     @brief コンビニエンスコンストラクタ
     
     インスタンスを生成し、初期化処理を行い、autoreleaseを行う。
     @return 生成したインスタンス
     */
    static AKHowToPlayScene* create();
    
    /*!
     @brief コンストラクタ
     
     初期化処理を行う。
     */
    AKHowToPlayScene();

protected:

    /*!
     @brief 派生クラスの初期化処理
     
     派生クラスの初期化処理を行う。
     */
    virtual bool initSub();
    
    /*!
     @brief ページ表示内容更新
     
     ページ番号に合わせて、ページ表示内容を更新する。
     @param pageNo ページ番号
     @return 有効にするインターフェースタグ
     */
    virtual unsigned int updatePageContents(int pageNo);

private:
    
    /// 説明画像
    cocos2d::Sprite *m_image;
    /// メッセージラベル
    AKLabel *_messageLabel;
};

#endif
