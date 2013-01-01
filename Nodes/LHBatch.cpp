//  This file was generated by LevelHelper
//  http://www.levelhelper.org
//
//  LevelHelperLoader.mm
//  Created by Bogdan Vladu
//  Copyright 2011 Bogdan Vladu. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//  The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//  Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//  This notice may not be removed or altered from any source distribution.
//  By "software" the author refers to this code file and not the application 
//  that was used to generate this file.
//
////////////////////////////////////////////////////////////////////////////////
#include "LHBatch.h"
#include "LHLayer.h"
#include "LHSprite.h"
#include "LHBezier.h"
//#include "LHDictionaryExt.h"
#include "SHDocumentLoader.h"

#include "LHSettings.h"
#include "../LevelHelperLoader.h"

#include "LHSettings.h"


#include "../CustomClasses/LHAbstractClass.h"
#include "../CustomClasses/LHCustomClasses.h"

static int untitledBatchCount = 0;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void LHBatch::removeSelf(){
    removeFromParentAndCleanup(true);
}
LevelHelperLoader* LHBatch::parentLoader(){
    
    CCNode* layerParent = this->getParent();
    
    while (layerParent && !LHLayer::isLHLayer(layerParent)){
        layerParent = layerParent->getParent();
    }
    
    if(layerParent && LHLayer::isLHLayer(layerParent)) {
        return ((LHLayer*)layerParent)->parentLoader();
    }
    return NULL;
}
//------------------------------------------------------------------------------
void LHBatch::loadUserCustomInfoFromDictionary(LHDictionary* dictionary){
    userCustomInfo = NULL;
    
    if(!dictionary)return;
    
    std::string className = dictionary->stringForKey("ClassName");
    
    userCustomInfo = LHCustomClassesMgr::customClassInstanceWithName(className);
    
    if(!userCustomInfo) return;
    
    LHDictionary* dict = dictionary->dictForKey("ClassRepresentation");
    
    if(dict){
        //        CCLog("SETTING PROPERTIES FROM DICT");
        ((LHAbstractClass*)userCustomInfo)->setPropertiesFromDictionary(dict);
    }
}
bool LHBatch::initWithDictionary(LHDictionary* dictionary,  LHLayer* layer){
    
    
    std::string imgPath = LHSettings::sharedInstance()->imagePath(dictionary->stringForKey("SheetImage"));
    
    if(imgPath == "")
        return false;
    
    if(!initWithFile(imgPath.c_str(), 29))
        return false;
    
    
    uniqueName = dictionary->stringForKey("UniqueName");

    setTag(dictionary->intForKey("Tag"));
    
    if(uniqueName == "")
    {
        uniqueName = dictionary->stringForKey("SheetName");

        if(uniqueName == "")
        {          
            printf("ERROR - CREATING LHBatch WITH NO SHEET NAME");
            uniqueName = "UntitledLayer_" + stringFromInt(untitledBatchCount);
            ++untitledBatchCount;
        }
    }
        
    if(dictionary->objectForKey("SHScene"))
        shFile = dictionary->stringForKey("SHScene");
        
    imagePath = imgPath;
        
    m_nZOrder = dictionary->intForKey("ZOrder");
        
    setTag(dictionary->intForKey("Tag"));
    
    if(layer){
        layer->addChild(this, getZOrder());
    }
    
    loadUserCustomInfoFromDictionary(dictionary->dictForKey("CustomClassInfo"));
    
    LHArray* childrenInfo = dictionary->arrayForKey("Children");
    if(childrenInfo)
    {
        for(int i = 0; i< childrenInfo->count(); ++i){
            LHDictionary* childDict = childrenInfo->dictAtIndex(i);
            addChildFromDictionary(childDict);
        }
    }
    
    
    
    return true;
}
//------------------------------------------------------------------------------
LHBatch::~LHBatch(void){
//    printf("LH Batch Dealloc %s\n", uniqueName.c_str());
    
    if(userCustomInfo){
        delete userCustomInfo;
        userCustomInfo = NULL;
    }
}
//------------------------------------------------------------------------------
LHBatch::LHBatch(){
    
}
//------------------------------------------------------------------------------
LHBatch* LHBatch::batchWithDictionary(LHDictionary* dictionary,  LHLayer* layer){
    LHBatch *pobNode = new LHBatch();
	if (pobNode && pobNode->initWithDictionary(dictionary, layer))
    {
	    pobNode->autorelease();
        return pobNode;
    }
    CC_SAFE_DELETE(pobNode);
	return NULL;
}
//------------------------------------------------------------------------------
LHBatch* LHBatch::batchWithSheetName(const std::string& sheetName, 
                                     const std::string& spriteHelperFile){
    
    LHDictionary* dictionary = SHDocumentLoader::sharedInstance()->dictionaryForSheetNamed(sheetName, 
                                                                                           spriteHelperFile);

    LHBatch* batch = LHBatch::batchWithDictionary(dictionary, NULL);    
    batch->setSHFile(spriteHelperFile);
    
    return batch;
}
//------------------------------------------------------------------------------
//if sprite is child of this batch node you can retrieve it
LHSprite* LHBatch::spriteWithUniqueName(const std::string& name){

    CCArray* children = getChildren();
    for(int i = 0; i < children->count(); ++i){
        CCNode* node = (CCNode*)children->objectAtIndex(i);

        if(LHSprite::isLHSprite(node)){
            if(((LHSprite*)node)->getUniqueName() == name){
                return (LHSprite*)node;
            }
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------
CCArray* LHBatch::allSprites(){
#if COCOS2D_VERSION >= 0x00020000
    CCArray* array = CCArray::create();
#else
    CCArray* array = CCArray::array();
#endif
    CCArray* children = getChildren();
    for(int i = 0; i < children->count(); ++i){
        CCNode* node = (CCNode*)children->objectAtIndex(i);

        if(LHSprite::isLHSprite(node)){
            array->addObject(node);    
        }
    }
    return array;
}
//------------------------------------------------------------------------------
CCArray* LHBatch::spritesWithTag(int tag){
#if COCOS2D_VERSION >= 0x00020000
    CCArray* array = CCArray::create();
#else
    CCArray* array = CCArray::array();
#endif
    
    CCArray* children = getChildren();
    for(int i = 0; i < children->count(); ++i){
        CCNode* node = (CCNode*)children->objectAtIndex(i);

        if(LHSprite::isLHSprite(node)){
            if(node->getTag() == tag)
                array->addObject(node);    
        }
    }
    return array;  
}
//------------------------------------------------------------------------------
std::string LHBatch::getImagePath(){
    return imagePath;
}
//------------------------------------------------------------------------------
std::string LHBatch::getSHFile(){
    return shFile;
}
void LHBatch::setSHFile(const std::string& file){
    shFile = std::string(file);
}

std::string LHBatch::userInfoClassName(){
    if(userCustomInfo)
        return ((LHAbstractClass*)userCustomInfo)->className();
    return "No Class";
}
//------------------------------------------------------------------------------
void* LHBatch::userInfo(){
    return userCustomInfo;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool LHBatch::isLHBatch(CCNode* node){
    if( 0 != dynamic_cast<LHBatch*>(node))
        return true;
    return false;
}
//------------------------------------------------------------------------------
void LHBatch::addChildFromDictionary(LHDictionary* childDict)
{
    if(childDict->stringForKey("NodeType") == "LHSprite")
    {
        LHDictionary* texDict = childDict->dictForKey("TextureProperties");
        int sprTag = texDict->intForKey("Tag");
        
        
        lh_spriteCreationMethods methods = LHCustomSpriteMgr::sharedInstance()->customSpriteClassForTag(sprTag);
        
        LHSprite* sprite =  (*methods.second)(childDict, this); //spriteWithDictionary
                
        addChild(sprite);
        sprite->postInit();
    }
    else if(childDict->stringForKey("NodeType") == "LHLayer"){
        printf("ERROR: Batch nodes should not have LHLayer as children.");
    }
    else if(childDict->stringForKey("NodeType") == "LHBatch"){
        printf("ERROR: Batch nodes should not have LHBatch as children.");
    }
    else if(childDict->stringForKey("NodeType") == "LHBezier"){
        printf("ERROR: Batch nodes should not have LHBezier as children.");
    }
}
//------------------------------------------------------------------------------
