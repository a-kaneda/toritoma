/*
 * Copyright (c) 2014 Akihiro Kaneda.
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
 @file LocalizedResource.cpp
 @brief リソースのローカライズ
 
 リソースのローカライズを管理する。
 */

#include "LocalizedResource.h"

using std::string;
using std::istringstream;
using std::pair;
using std::map;
using cocos2d::Application;
using cocos2d::LanguageType;
using cocos2d::FileUtils;
using cocos2d::Data;

namespace aklib {
    
    static const string STRING_FILE_ENGLISH = "Localized_en";
    static const string STRING_FILE_JAPANESE= "Localized_ja";
    
    static LocalizedResource& singleton = LocalizedResource::getInstance();
    
    LocalizedResource::LocalizedResource()
    {
        string fileName = getStringFileName();
//        AKLog(1, "fileName=%s", fileName.c_str());
        
        string filePath = FileUtils::getInstance()->fullPathForFilename(fileName);
//        AKLog(1, "filePath=%s", filePath.c_str());
        
        string fileData = FileUtils::getInstance()->getStringFromFile(filePath);
        
        readStringData(fileData, _localizedStrings);
    }
    
    LocalizedResource& LocalizedResource::getInstance()
    {
        static LocalizedResource singleton;
        return singleton;
    }
    
    const string& LocalizedResource::getString(const string &key) const
    {
//        AKLog(1, "key=%s, value=%s", key.c_str(), _localizedStrings.at(key).c_str());
        return _localizedStrings.at(key);
    }
    
    string LocalizedResource::getStringFileName() const
    {
        LanguageType lang = Application::getInstance()->getCurrentLanguage();
        
        switch (lang) {
            case cocos2d::LanguageType::ENGLISH:
                return STRING_FILE_ENGLISH;
                
            case cocos2d::LanguageType::JAPANESE:
                return STRING_FILE_JAPANESE;
                
            default:
                return STRING_FILE_ENGLISH;
        }
    }
    
    void LocalizedResource::readStringData(const string &stringData, map<string, string> &localizedStrings) const
    {
        istringstream is(stringData);
        string line;
        while (std::getline(is, line)) {
            
//            AKLog(1, "line=%s", line.c_str());
            
            if (line.compare(0, 2, "//") == 0) {
                continue;
            }
            
            string::size_type sep = line.find("=");
            if (sep == string::npos) {
                continue;
            }
            
            string key = line.substr(0, sep - 1);
            string val = line.substr(sep + 1);
            
            key.erase(0, key.find_first_not_of(" \""));
            key.erase(key.find_last_not_of(" \"") + 1);
            
            val.erase(0, val.find_first_not_of(" \""));
            val.erase(val.find_last_not_of(" \";") + 1);
            
//            AKLog(1, "key=%s value=%s", key.c_str(), val.c_str());
            
            localizedStrings.insert(pair<string, string>(key, val));
        }
    }
}