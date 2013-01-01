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

#include "LHSettings.h"
#include <iostream>
#include <fstream>
#include "Box2D/Box2D.h"

#include "LHLayer.h"
#include "LHSprite.h"
#include "LHBatch.h"
#include "LHJoint.h"
#include "LHBezier.h"
#include "LHNode.h"

LHSettings *LHSettings::m_sharedInstance = 0;

////////////////////////////////////////////////////////////////////////////////
LHSettings* LHSettings::sharedInstance(){
	if (0 == m_sharedInstance){
		m_sharedInstance = new LHSettings();
	}
    return m_sharedInstance;
}
////////////////////////////////////////////////////////////////////////////////
LHSettings::~LHSettings()
{
    delete allLHMainLayers;
}
////////////////////////////////////////////////////////////////////////////////
LHSettings::LHSettings()
{
#if COCOS2D_VERSION >= 0x00020000  
    allLHMainLayers = CCArray::create();
#else
    allLHMainLayers = CCArray::array();
#endif
    allLHMainLayers->retain();
    
    m_userOffset = CCPointMake(0, 0);
    hdSuffix = "-hd";
    hd2xSuffix = "-ipadhd";
    
    m_useRetinaOnIpad = true;
    m_convertLevel = true;
    m_lhPtmRatio = 32.0f;
    m_customAlpha = 1.0f;
    m_convertRatio = CCPointMake(1, 1);
    m_realConvertRatio = CCPointMake(1, 1);
    m_newBodyId = 0;
    m_stretchArt = true;
    m_possitionOffset = CCPointMake(0.0f, 0.0f);
    m_levelPaused = false;
    m_imagesFolder = std::string("");
    m_isCoronaUser = false;
    m_preloadBatchNodes = false;    
    
    activeBox2dWorld = NULL;
}
////////////////////////////////////////////////////////////////////////////////
void LHSettings::addLHMainLayer(LHLayer* layer){
    allLHMainLayers->addObject(layer);
}
void LHSettings::removeLHMainLayer(LHLayer* layer){
    allLHMainLayers->removeObject(layer);
}

CCArray* LHSettings::getAllLHMainLayers()
{
    return allLHMainLayers;
}

b2World* LHSettings::getActiveBox2dWorld(){
    return activeBox2dWorld;
}
void LHSettings::setActiveBox2dWorld(b2World* world){
    activeBox2dWorld = world;
}


void LHSettings::markNodeForRemoval(CCObject* node){
    if(NULL !=node){
        //the object can be LHBatch, LHLayer, LHJoint, LHSprite, LHBezier
        //we are doing a LHNode cast so we dont get an error
        if( 0 != dynamic_cast<LHSprite*>(node)){
            markedNodes.setObject(node, ((LHSprite*)node)->getUniqueName());
        }
        else if( 0 != dynamic_cast<LHBezier*>(node)){
            markedNodes.setObject(node, ((LHBezier*)node)->getUniqueName());
        }
        else if( 0 != dynamic_cast<LHJoint*>(node)){
            markedNodes.setObject(node, ((LHJoint*)node)->getUniqueName());
        }
        else if( 0 != dynamic_cast<LHBatch*>(node)){
            markedNodes.setObject(node, ((LHBatch*)node)->getUniqueName());
        }
        else if( 0 != dynamic_cast<LHLayer*>(node)){
            markedNodes.setObject(node, ((LHLayer*)node)->getUniqueName());
        }
    }
}

void LHSettings::removeMarkedNode(CCObject* node)
{
    if( 0 != dynamic_cast<LHSprite*>(node)){
        ((LHSprite*)node)->removeSelf();
    }
    else if( 0 != dynamic_cast<LHBezier*>(node)){
        ((LHBezier*)node)->removeSelf();
    }
    else if( 0 != dynamic_cast<LHJoint*>(node)){
        ((LHJoint*)node)->removeSelf();
    }
    else if( 0 != dynamic_cast<LHBatch*>(node)){
        ((LHBatch*)node)->removeSelf();
    }
    else if( 0 != dynamic_cast<LHLayer*>(node)){
        ((LHLayer*)node)->removeSelf();
    }
}
void LHSettings::removeMarkedNodes(){
    
#if COCOS2D_VERSION >= 0x00020000
    
    CCArray* keys = markedNodes.allKeys();
    if(keys){
        for(int i = 0; i < keys->count(); ++i){
            LHNode* node = (LHNode*)markedNodes.objectForKey(((CCString*)keys->objectAtIndex(i))->getCString());
            removeMarkedNode(node);
        }
    }
    markedNodes.removeAllObjects();
    
#else
    
    std::vector<std::string> keys = markedNodes.allKeys();
    for(int i = 0; i < keys.size(); ++i){
        LHNode* node = (LHNode*)markedNodes.objectForKey(keys[i]);
        removeMarkedNode(node);
    }
    markedNodes.removeAllObjects();
#endif
}

////////////////////////////////////////////////////////////////////////////////
int LHSettings::newBodyId(void)
{
	return m_newBodyId++;
}

void LHSettings::setImageFolder(const char* img){

    if(0 != img)
        m_imagesFolder = std::string(img);

}
const std::string& LHSettings::imageFolder(void){
    return m_imagesFolder;
}


CCPoint LHSettings::transformedScalePointToCocos2d(CCPoint point){
    
    CCPoint ratio = convertRatio();
    if(isIpad() && device != 1 && device != 3) //and device is not ipad only
    {        
        ratio.x /= 2.0f;
        ratio.y /= 2.0f;
    }
    point.x *= ratio.x;
    point.y *= ratio.y;
    return point;
}

CCPoint LHSettings::transformedPointToCocos2d(CCPoint point){
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCPoint pos_offset = possitionOffset();
    CCPoint  wbConv = convertRatio();
    
    point.x += pos_offset.x/2.0f;
    point.y += pos_offset.y/2.0f;
    
    point.x += m_userOffset.x/2.0f;
    point.y += m_userOffset.y/2.0f;
    
    point.x *= wbConv.x;
    point.y  = winSize.height - point.y*wbConv.y;
    
    return point;
}
CCPoint LHSettings::transformedPoint(CCPoint point, const std::string& image){
    
    if(device != 0 && device != 1 && device != 3) //iphone //ipad
    {
        point.x *= convertRatio().x;
        point.y *= convertRatio().y;
  
        if (std::string::npos != image.find(hdSuffix) ||
            std::string::npos != image.find(hd2xSuffix)
            
#if COCOS2D_VERSION >= 0x00020000
            || isIpad()
#endif

            ) {
            point.x /=2.0f;
            point.y /=2.0f;        
        }
    }
    return point;    
}

CCRect LHSettings::transformedTextureRect(CCRect rect, const std::string& image)
{
    if(device != 0 && device != 1 && device != 3) //iphone //ipad
    {
//        CCLog("transformedTextureRect");
        if (std::string::npos != image.find(hdSuffix) ||
            std::string::npos != image.find(hd2xSuffix)

#if COCOS2D_VERSION >= 0x00020000
            || isIpad()
#endif            
            ){
            rect = CCRectMake(rect.origin.x*2.0f, rect.origin.y*2.0f,
                              rect.size.width*2.0f, rect.size.height*2.0f);
        }
    }
    
    return rect;    
}

CCSize LHSettings::transformedSize(CCSize size, const std::string& image)
{
    if(device != 0 && device != 1 && device != 3) //iphone //ipad
    {
        if (std::string::npos != image.find(hdSuffix) ||
            std::string::npos != image.find(hd2xSuffix)
                                            
#if COCOS2D_VERSION >= 0x00020000
            || isIpad()
#endif
            )
        {
            size = CCSizeMake(size.width*2.0f, size.height*2.0f);
        }
    }
    
    return size;    
}


bool LHSettings::isHDImage(const std::string& image)
{
    if(device == 0 || device == 1 || device == 3) //iphone //ipad
        return false;
    
#if COCOS2D_VERSION >= 0x00020000
    if(isIpad()){
        return true;
    }
#endif
    
    if (std::string::npos == image.find(hdSuffix) &&
        std::string::npos == image.find(hd2xSuffix)){
        return false;
    }
    return true;
}


const std::string LHSettings::imagePath(const std::string& image){
        
    std::string computedFile = image;
    if(isIpad())
    {
       // CCLog("IS IPAD");
        
        if(device != 1 && device != 3)//if ipad only then we dont need to apply transformations
        {
            if(CC_CONTENT_SCALE_FACTOR() == 2)
            {
                //we have ipad retina
                size_t found;
                found=computedFile.find_last_of(".");
                
                #if COCOS2D_VERSION < 0x00020000
                computedFile.insert(found, hd2xSuffix);
                #endif
            }
            else {
                //we have normal ipad - lets use the HD image
                
                size_t found;
                found=computedFile.find_last_of(".");
                #if COCOS2D_VERSION < 0x00020000
                computedFile.insert(found, hdSuffix);
                #endif
            }
        }
#if COCOS2D_VERSION >= 0x00020000
        const char* fullpath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(computedFile.c_str());
#else
        const char* fullpath = CCFileUtils::fullPathFromRelativePath(computedFile.c_str());
#endif
        
        std::ifstream infile;
        infile.open (fullpath);
        if(true == infile.is_open()){ //IF THIS FAILS IT MEANS WE HAVE NO -hd file
            infile.close();
            return std::string(fullpath);
        }
    }
    
//    CCLog("RETURNING %s", image.c_str());
    
    return image;
}

bool LHSettings::isIpad(void){
        
    CCSize wSize = CCDirector::sharedDirector()->getWinSize();
    
    if((wSize.width >= 1024 || wSize.width >= 768) &&
       (wSize.height >= 1024 || wSize.height >= 768))
    {
        return true;
    }
    
    return false;
}

bool LHSettings::isIphone5(void){
    
    CCSize wSize = CCDirector::sharedDirector()->getWinSizeInPixels();
    
    if(wSize.width == 1136 ||
       wSize.height == 1136 ||
       wSize.width == 568 || //may be reported wrong
       wSize.height == 568)
        return true;
    
    return false;
}


void LHSettings::setStretchArt(const bool& value){
    m_stretchArt = value;
}

bool LHSettings::getStretchArt(void)
{
    return m_stretchArt;
}

CCPoint LHSettings::possitionOffset(void){
    return m_possitionOffset;
}
void LHSettings::setConvertRatio(CCPoint val){
    
    m_convertRatio = val;
    m_realConvertRatio = val;
    if(!m_stretchArt)
    {
        if(isIpad())
        {
            if(m_convertRatio.x > 1.0 || m_convertRatio.y > 1.0f)
            {
                m_convertRatio.x = 2.0f;
                m_convertRatio.y = 2.0f;
                                
                if(CCDirector::sharedDirector()->getWinSize().width == 1024.0f)
                {
                    m_possitionOffset.x = 32.0f;
                    m_possitionOffset.y = 64.0f;   
                }
                else {
                    m_possitionOffset.x = 64.0f;
                    m_possitionOffset.y = 32.0f;
                }
            }
            
            if(device == 3)
            {
                m_convertRatio.x = 1.0f;
                m_convertRatio.y = 1.0f;
            }
        }
        
        if(isIphone5()){
            
            if(m_convertRatio.x > 1.0 || m_convertRatio.y > 1.0f)
            {
                m_convertRatio.x = 1.0f;
                m_convertRatio.y = 1.0f;
            
                if(CCDirector::sharedDirector()->getWinSizeInPixels().width == 1136.0f)
                {
                    CCLog("POSITION OFFSET ON X");
                    m_possitionOffset.x = 88.0f;//88.0f;
                    m_possitionOffset.y = 0.0f;
                }
                else {//if(CCDirector::sharedDirector()->getWinSize().height == 1136.0f) {
                    CCLog("POSITION OFFSET ON Y");
                    m_possitionOffset.x = 0.0f;
                    m_possitionOffset.y = 88.0f;//88.0f;
                }
                
            }
        }
    }

}
CCPoint LHSettings::convertRatio(void){
    if(!m_convertLevel)
		return CCPointMake(1, 1);
	
	return m_convertRatio;
}
CCPoint LHSettings::realConvertRatio(void){
    if(!m_convertLevel)
		return CCPointMake(1, 1);
	
	return m_realConvertRatio;
}