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
 @file AKLogNoDef.h
 @brief ログ区分定義
 
 ログの開始、停止を管理するための定数を定義する。
 定数名はkAK<クラス名>_<区分>とする。
 区分は0:エラー処理、1:通常処理、2〜:ループ処理等頻繁に呼ばれる処理とする。
 */

#ifndef AKLOGNODEF_H
#define AKLOGNODEF_H

#ifdef DEBUG
extern bool kAKLogBack_0;
extern bool kAKLogBack_1;
extern bool kAKLogBlock_0;
extern bool kAKLogBlock_1;
extern bool kAKLogCharacter_0;
extern bool kAKLogCharacter_1;
extern bool kAKLogCharacter_2;
extern bool kAKLogCharacter_3;
extern bool kAKLogCharacter_4;
extern bool kAKLogCharacterPool_0;
extern bool kAKLogCharacterPool_1;
extern bool kAKLogChickenGauge_0;
extern bool kAKLogChickenGauge_1;
extern bool kAKLogEffect_0;
extern bool kAKLogEffect_1;
extern bool kAKLogEnemy_0;
extern bool kAKLogEnemy_1;
extern bool kAKLogEnemy_2;
extern bool kAKLogEnemy_3;
extern bool kAKLogEnemyShot_0;
extern bool kAKLogEnemyShot_1;
extern bool kAKLogEnemyShot_2;
extern bool kAKLogGameCenterHelper_0;
extern bool kAKLogGameCenterHelper_1;
extern bool kAKLogHowToPlayScene_0;
extern bool kAKLogHowToPlayScene_1;
extern bool kAKLogLife_0;
extern bool kAKLogLife_1;
extern bool kAKLogOption_0;
extern bool kAKLogOption_1;
extern bool kAKLogOptionScene_0;
extern bool kAKLogOptionScene_1;
extern bool kAKLogPlayData_0;
extern bool kAKLogPlayData_1;
extern bool kAKLogPlayData_2;
extern bool kAKLogPlayData_3;
extern bool kAKLogPlayer_0;
extern bool kAKLogPlayer_1;
extern bool kAKLogPlayerShot_0;
extern bool kAKLogPlayerShot_1;
extern bool kAKLogPlayingScene_0;
extern bool kAKLogPlayingScene_1;
extern bool kAKLogPlayingScene_2;
extern bool kAKLogPlayingScene_3;
extern bool kAKLogPlayingSceneIF_0;
extern bool kAKLogPlayingSceneIF_1;
extern bool kAKLogTileMap_0;
extern bool kAKLogTileMap_1;
extern bool kAKLogTileMap_2;
extern bool kAKLogTileMap_3;
extern bool kAKLogTileMap_4;
extern bool kAKLogScriptData_0;
extern bool kAKLogScriptData_1;
extern bool kAKLogTitleScene_0;
extern bool kAKLogTitleScene_1;
#endif

#endif
