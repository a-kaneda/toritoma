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
 @file AKToritoma.h
 @brief とりとま共通インクルード
 
 アプリ全体で共通で使用するヘッダーのインクルードと定数定義を行う。
 */

#ifndef AKTORITOMA_H
#define AKTORITOMA_H

#include "AKLib.h"
#include "AKLogNoDef.h"
#include "SimpleAudioEngine.h"

// コントロールテクスチャアトラス定義ファイル名
extern const char *kAKControlTextureAtlasDefFile;
// コントロールテクスチャアトラスファイル名
extern const char *kAKControlTextureAtlasFile;
// カーソル画像のファイル名
extern const char *CursorImageFileName;
// Bボタン画像のファイル名
extern const char *BButtonImageFileName;
// Lボタン画像のファイル名
extern const char *LButtonImageFileName;
// Rボタン画像のファイル名
extern const char *RButtonImageFileName;
// コントローラー操作のしきい値
extern const float kAKControllerAxisThreshold;

// ステージBGMの種類数
extern const int kAKStageBGMCount;
// ステージBGMのファイル名
extern const char *kAKStageBGMFileName;
// ステージクリアジングルのファイル名
extern const char *kAKClearJingleFileName;
// ゲームクリアジングルのファイル名
extern const char *kAKGameClearJingleFileName;
// ボタン選択効果音のファイル名
extern const char *kAKSelectSEFileName;
// ポーズ効果音のファイル名
extern const char *kAKPauseSEFileName;
// 爆発(小)効果音のファイル名
extern const char *kAKBombMinSEFileName;
// 爆発(大)効果音のファイル名
extern const char *kAKBombBigSEFileName;
// 弾衝突効果音のファイル名
extern const char *kAKHitSEFileName;
// 自機死亡時の効果音
extern const char *kAKMissSEFileName;
// 1UP時の効果音
extern const char *kAK1UpSEFileName;
// カーソル移動時の効果音
extern const char *kAKCursorSEFileName;

// ユーザーデフォルトキー、ハイスコア
extern const char *kAKUDKeyHighScore;

// デバッグ用フラグ
#ifdef DEBUG

/// 自機当たり判定無効化
//#define DEBUG_MODE_PLAYER_INVINCIBLE
/// 開始周回数
#define DEBUG_MODE_START_LOOP 1
/// 開始ステージ番号
#define DEBUG_MODE_START_STAGE 1
/// 2周目開始ステージ番号
#define DEBUG_MODE_2ND_START_STAGE 1
/// 初期残機
#define DEBUG_MODE_INITIAL_LIFE 10

#else

/// 開始周回数
#define DEBUG_MODE_START_LOOP 1
/// 開始ステージ番号
#define DEBUG_MODE_START_STAGE 1
/// 2周目開始ステージ番号
#define DEBUG_MODE_2ND_START_STAGE 1
/// 初期残機
#define DEBUG_MODE_INITIAL_LIFE 10

#endif

#endif
