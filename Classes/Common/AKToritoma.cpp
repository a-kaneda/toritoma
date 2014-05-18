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
 @file AKToritoma.m
 @brief とりとま共通インクルード
 
 アプリ全体で共通で使用するヘッダーのインクルードと定数定義を行う。
 */

#include "AKToritoma.h"

/// アプリのURL
const char *kAKAplUrl = "https://itunes.apple.com/us/app/toritoma/id614837983?l=ja&ls=1&mt=8";
/// AdMobパブリッシャーID
const char *kAKAdMobID = "f894d395a3f5431b";
/// 広告バナーx方向の位置
//enum AKBannerPosX kAKBannerPosX = kAKBannerPosXLeft;
/// 広告バナーy方向の位置
//enum AKBannerPosY kAKBannerPosY = kAKBannerPosYTop;
/// AppBankNetworkのapiKey
const char *kAKAPIKey = "660d880534668e872da444c003e25ba41b3f44b7";
/// AppBankNetworkのspotID
const char *kAKSpotID = "40141";

/// コントロールテクスチャアトラス定義ファイル名
const char *kAKControlTextureAtlasDefFile = "Control.plist";
/// コントロールテクスチャアトラスファイル名
const char *kAKControlTextureAtlasFile = "Control.png";

/// ステージBGMの種類数
const int kAKStageBGMCount = 8;
/// ステージBGMのファイル名
const char *kAKStageBGMFileName = "stage%d.mp3";
/// エンディングBGMのファイル名
const char *kAKEndingBGMFileName = "ending.mp3";
/// ステージクリアジングルのファイル名
const char *kAKClearJingleFileName = "clear.mp3";
/// ボタン選択効果音のファイル名
const char *kAKSelectSEFileName = "select.mp3";
/// ポーズ効果音のファイル名
const char *kAKPauseSEFileName = "pause.mp3";
/// 爆発(小)効果音のファイル名
const char *kAKBombMinSEFileName = "bomb_min.mp3";
/// 爆発(大)効果音のファイル名
const char *kAKBombBigSEFileName = "bomb_big.mp3";
/// 弾衝突効果音のファイル名
const char *kAKHitSEFileName = "hit.mp3";
/// 自機死亡時の効果音
const char *kAKMissSEFileName = "miss.mp3";


