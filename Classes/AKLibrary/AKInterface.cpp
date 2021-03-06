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
 @file AKInterface.cpp
 @brief 画面入力管理クラス
 
 画面のタッチ入力を管理するクラスを定義する。
 */

#include "AKInterface.h"

using cocos2d::Director;
using cocos2d::Touch;
using cocos2d::Event;
using cocos2d::EventListenerTouchOneByOne;
using cocos2d::Vec2;
using cocos2d::Sprite;
using cocos2d::Rect;
using cocos2d::Node;

/*!
 @brief コンビニエンスコンストラクタ
 
 インスタンスを生成し、初期化処理を行い、autoreleaseを行う。
 @param eventHandler イベントを処理するクラス
 @param capacity メニュー項目数
 @return 生成したインスタンス
 */
AKInterface* AKInterface::create(AKMenuEventHandler * const eventHandler)
{
    AKInterface *instance = new AKInterface(eventHandler);
    if (instance->init()) {
        instance->autorelease();
        return instance;
    }
    else {
        CC_SAFE_DELETE(instance);
        return NULL;
    }
}

/*!
 @brief デストラクタ
 
 メニュー項目を削除する。
 */
AKInterface::~AKInterface()
{
    for (AKMenuItem *item : m_menuItems) {
        delete item;
    }
}

/*!
 @brief 有効化タグ取得
 
 有効化タグを取得する。
 @return 有効化タグ
 */
unsigned int AKInterface::getEnableTag()
{
    return m_enableTag;
}

/*!
 @brief 有効タグの設定
 
 有効化するアイテムのタグを設定する。
 有効化したアイテムのみを表示状態にする。
 @param enableItemTag 有効化するアイテムのタグ
 */
void AKInterface::setEnableTag(unsigned int enableTag)
{
    AKLog(kAKLogInterface_1, "setEnableTag() start:enableTag=%u", enableTag);

    // 有効化タグに値を設定する
    m_enableTag = enableTag;
    
    // メニュー項目の表示非表示に設定内容を反映する
    this->updateVisible();
}

/*!
 @brief タッチ開始処理
 
 タッチが開始されたときの処理。
 @param touch タッチ情報
 @param event イベント情報
 @return タッチイベントを処理するかどうか
 */
bool AKInterface::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    AKLog(kAKLogInterface_1, "ccTouchBegan() start");

    // メニュー項目が登録されていない場合は処理を終了する
    if (m_menuItems.size() == 0) {
        return true;
    }
    
    // 画面上のタッチ位置を取得する
    Vec2 locationInView = pTouch->getLocationInView();
    
    // cocos2dの座標系に変換する
    Vec2 location = Director::getInstance()->convertToGL(locationInView);
    
    // 各項目の選択処理を行う
    std::vector<AKMenuItem*>::iterator it;
    for (it = m_menuItems.begin(); it != m_menuItems.end(); it++) {

        AKMenuItem *item = *it;
        
        AKAssert(item, "メニューアイテムがNULL");
        AKLog(kAKLogInterface_1, "tag=%d, type=%d eventNo=%d", item->getTag(), item->getType(), item->getEventNo());
        
        // 有効な項目で選択されている場合は処理を行う
        if ((item->getTag() & m_enableTag || item->getTag() == 0) && item->isSelect(location)) {
            
            // タッチ状態を設定する
            item->setTouchPhase(kAKMenuTouchBegan);
            
            // メニュー種別に応じて処理を分岐する
            switch (item->getType()) {

                case kAKMenuTypeButton:     // タッチボタン

                    AKLog(kAKLogInterface_1, "タッチボタン");

                    // 選択されていれば親ノードにイベントを送信する
                    m_eventHandler->execEvent(item);
                    return true;
                    
                case kAKMenuTypeMomentary:  // モーメンタリボタン

                    AKLog(kAKLogInterface_1, "モーメンタリボタン");

                    // まだ選択されていない場合はON状態にする
                    if (item->getTouch() == NULL) {
                        item->setTouch(pTouch);
                        m_eventHandler->execEvent(item);
                        return true;
                    }
                    break;
                    
                case kAKMenuTypeSlide:      // スライド入力

                    AKLog(kAKLogInterface_1, "スライド入力");

                    // まだスライド開始していない場合は開始する
                    if (item->getTouch() == NULL) {

                        item->setTouch(pTouch);
                        item->setPrevPoint(location);
                        m_eventHandler->execEvent(item);
                        return true;
                    }
                    break;
                    
                default:
                    AKAssert(false, "メニュー項目の種別が不正:%d", item->getType());
                    break;
            }
        }
    }
    
    AKLog(kAKLogInterface_1, "ccTouchBegan() end");
    return true;
}

/*!
 @brief タッチ中断処理
 
 タッチが中断された時の処理。
 @param touch タッチ情報
 @param event イベント情報
 */
void AKInterface::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    // キャンセルするメニュー項目を検索する
    for (AKMenuItem *item : m_menuItems) {
        
        // メニュー項目のタッチイベントとキャンセルされたタッチイベントが一致した場合
        if (item->getTouch() == pTouch) {
            
            // タッチ状態を設定する
            item->setTouchPhase(kAKMenuTouchCancelled);
            
            // 最後のイベントを実行する
            m_eventHandler->execEvent(item);

            // タッチ情報をクリアする
            item->setTouch(NULL);
            item->setTouchPhase(kAKMenuTouchNone);
            
            break;
        }
    }
}

/*!
 @brief タッチ終了処理
 
 タッチが終了された時の処理。
 @param touch タッチ情報
 @param event イベント情報
 */
void AKInterface::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    // 終了するメニュー項目を検索する
    for (AKMenuItem *item : m_menuItems) {
        
        // メニュー項目のタッチイベントと終了されたタッチイベントが一致した場合
        if (item->getTouch() == pTouch) {
            
            // タッチ状態を設定する
            item->setTouchPhase(kAKMenuTouchEnded);

            // 最後のイベントを実行する
            m_eventHandler->execEvent(item);
            
            // タッチ情報をクリアする
            item->setTouch(NULL);
            item->setTouchPhase(kAKMenuTouchNone);
            
            break;
        }
    }
}

/*!
 @brief タッチ移動処理
 
 タッチが移動された時の処理。
 @param touch タッチ情報
 @param event イベント情報
 */
void AKInterface::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    // 移動するメニュー項目を検索する
    for (AKMenuItem *item : m_menuItems) {
        
        // メニュー項目のタッチイベントと移動されたタッチイベントが一致した場合
        if (item->getTouch() == pTouch) {
            
            // タッチ状態を設定する
            item->setTouchPhase(kAKMenuTouchMove);
            
            // イベントを実行する
            m_eventHandler->execEvent(item);
            
            // 画面上のタッチ位置を取得する
            Vec2 locationInView = pTouch->getLocationInView();
            
            // cocos2dの座標系に変換する
            Vec2 location = Director::getInstance()->convertToGL(locationInView);

            // 前回タッチ位置を更新する
            item->setPrevPoint(location);
            
            return;
        }
    }
    
    // 画面上のタッチ位置を取得する
    Vec2 locationInView = pTouch->getLocationInView();
    
    // cocos2dの座標系に変換する
    Vec2 location = Director::getInstance()->convertToGL(locationInView);
    
    // 一致するメニュー項目がなかった場合に、まだタッチイベントが開始されていない項目がある場合は関連付ける
    for (AKMenuItem *item : m_menuItems) {
        
        // スライド入力以外とすでにタッチイベントが開始されているものは除外する
        if (item->getType() != kAKMenuTypeSlide || item->getTouch() != NULL) {
            continue;
        }
        
        // 有効な項目で選択されている場合は処理を行う
        if ((item->getTag() & m_enableTag || item->getTag() == 0) && item->isSelect(location)) {
            
            item->setTouchPhase(kAKMenuTouchMove);
            item->setTouch(pTouch);
            item->setPrevPoint(location);
            m_eventHandler->execEvent(item);
            return;
        }
    }
}

/*!
 @brief メニュー項目追加
 
 メニュー項目を追加する。
 スライド項目が後ろになるように配列に格納する。
 @param menu メニュー項目
 */
void AKInterface::addMenuItem(AKMenuItem *menu)
{
    AKAssert(menu, "引数にNULLが指定された");
    
    // 追加する項目がスライド入力の場合は末尾に追加する
    if (menu->getType() == kAKMenuTypeSlide) {
        m_menuItems.push_back(menu);
    }
    // その他の場合は先頭に追加する
    else {
        m_menuItems.insert(m_menuItems.begin(), menu);
    }
}

/*!
 @brief スプライトフレームからメニュー項目作成
 
 スプライトフレームを読み込んでスプライトを作成し、同じ位置にメニュー項目を作成する。
 @param spriteName 画像名
 @param pos メニュー項目の位置
 @param event ボタンタップ時の処理
 @param z メニュー項目のz座標
 @param tag メニュー項目のタグ
 @param type メニュータイプ
 @return 作成したメニュー項目のスプライト
 */
Sprite* AKInterface::addSpriteMenu(const std::string &spriteName,
                                     Vec2 position,
                                     int z,
                                     int event,
                                     unsigned int tag,
                                     enum AKMenuType type)
{
    AKLog(kAKLogInterface_1, "addSpriteMenu() start:spriteName=%s, position=(%f, %f), z=%d, event=%d, tag=%u type=%d",
        spriteName.c_str(), position.x, position.y, z, event, tag, type);

    // ボタンの画像を読み込む
    Sprite *itemSprite = Sprite::createWithSpriteFrameName(spriteName.c_str());
    AKAssert(itemSprite != NULL, "ボタンの画像読み込みに失敗");
    
    // ボタンの位置を設定する
    itemSprite->setPosition(position);
    
    // ボタンをレイヤーに配置する
    addChild(itemSprite, z, tag);
    
    // メニュー項目の位置をスプライトの左上の端を設定する
    // メニュー項目の大きさにスプライトのサイズを設定する
    Rect rect(itemSprite->getPosition().x - itemSprite->getContentSize().width / 2,
              itemSprite->getPosition().y - itemSprite->getContentSize().height / 2,
              itemSprite->getContentSize().width,
              itemSprite->getContentSize().height);

    // メニュー項目を追加する
    AKMenuItem *menuItem = new AKMenuItem(rect, type, event, tag);
    addMenuItem(menuItem);
    
    AKLog(kAKLogInterface_1, "addSpriteMenu() end");

    // 作成したメニュー項目のスプライトを返す
    return itemSprite;
}

// スプライトフレームから当たり判定サイズを指定してメニュー項目を作成
Sprite* AKInterface::addSpriteMenu(const std::string &spriteName,
                                   Vec2 position,
                                   int z,
                                   Rect rect,
                                   int event,
                                   unsigned int tag,
                                   enum AKMenuType type)
{
    // ボタンの画像を読み込む
    Sprite *itemSprite = Sprite::createWithSpriteFrameName(spriteName.c_str());
    AKAssert(itemSprite != NULL, "ボタンの画像読み込みに失敗");
    
    // ボタンの位置を設定する
    itemSprite->setPosition(position);
    
    // ボタンをレイヤーに配置する
    addChild(itemSprite, z, tag);
    
    // メニュー項目を追加する
    AKMenuItem *menuItem = new AKMenuItem(rect, type, event, tag);
    addMenuItem(menuItem);
    
    AKLog(kAKLogInterface_1, "addSpriteMenu() end");
    
    // 作成したメニュー項目のスプライトを返す
    return itemSprite;
    
}

/*!
 @brief メニュー項目の追加
 
 メニュー項目のラベルを作成し、インターフェースに項目を追加する。
 @param menuString メニューのキャプション
 @param pos メニューの位置
 @param event メニュー選択時の処理
 @param z メニュー項目のz座標
 @param tag メニュー項目のタグ
 @param withFrame 枠を付けるかどうか
 @return 作成したメニュー項目
 */
AKLabel* AKInterface::addLabelMenu(const std::string menuString,
                                   Vec2 position,
                                   int z,
                                   int event,
                                   unsigned int tag,
                                   bool withFrame)
{
    // ラベルを作成する
    AKLabel *label = AKLabel::createLabel(menuString, (int)menuString.length(), 1, (withFrame ? kAKLabelFrameButton : kAKLabelFrameNone), AKLabel::ControlFont);
    
    // ラベルの位置を設定する
    label->setPosition(position);
    
    // ラベルを画面に配置する
    addChild(label, z, tag);
    
    // メニュー項目をインターフェースに追加する
    AKMenuItem *menuItem = new AKMenuItem(label->getRect(), kAKMenuTypeButton, event, tag);
    addMenuItem(menuItem);
    
    // 作成したメニュー項目を返す
    return label;
}

// スライド入力作成
/*!
 @brief スライド入力作成
 
 スライド入力を作成する。
 @param rect 位置と大きさ
 @param event 項目処理時の処理
 @param tag タグ情報(任意に使用)
 */
void AKInterface::addSlideMenu(Rect rect, int event, unsigned int tag)
{
    AKMenuItem *menuItem = new AKMenuItem(rect, kAKMenuTypeSlide, event, tag);
    addMenuItem(menuItem);
}

/*!
 @brief イベントハンドラを指定したコンストラクタ
 
 イベントハンドラを指定したコンストラクタ。
 @param eventHandler イベントを処理するクラス
 */
AKInterface::AKInterface(AKMenuEventHandler * const eventHandler) :
m_eventHandler(eventHandler), m_enableTag(0), m_menuItems()
{
    // Register Touch Event
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(AKInterface::onTouchBegan, this);
    listener->onTouchCancelled = CC_CALLBACK_2(AKInterface::onTouchCancelled, this);
    listener->onTouchEnded = CC_CALLBACK_2(AKInterface::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(AKInterface::onTouchMoved, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

/*!
 @brief メニュー項目表示非表示設定
 
 メニュー項目の表示非表示を有効化タグ範囲をもとに設定する。
 有効化タグ範囲内の項目は表示、範囲外の項目は非表示とする。
 */
void AKInterface::updateVisible()
{
    // レイヤー上のすべてのノードに対して処理を行う
    for (Node *item : getChildren()) {
        
        // 有効タグならば表示する、タグが0の場合は無条件に表示する
        if ((item->getTag() == 0) || (item->getTag() & m_enableTag)) {
            item->setVisible(true);
        }
        // 範囲外ならば非表示にする
        else {
            item->setVisible(false);
        }
        
        // 個別に表示非表示を設定
        updateVisible(item);
    }
}

/*!
 @brief メニュー項目個別表示設定
 
 メニュー項目の表示非表示を有効タグとは別に設定したい場合に個別に設定を行う。
 派生クラスで使用する。
 @param item 設定するメニュー項目
 */
void AKInterface::updateVisible(Node *item)
{
    // 派生クラスで処理を追加する
}
