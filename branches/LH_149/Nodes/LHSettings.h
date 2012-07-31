//  This file was generated by LevelHelper
//  http://www.levelhelper.org
//
//  LevelHelperLoader.h
//  Created by Bogdan Vladu
//  Copyright 2011 Bogdan Vladu. All rights reserved.
//
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

#ifndef __LH_SETTINGS_SINGLETON
#define __LH_SETTINGS_SINGLETON

#include "cocos2d.h"
#include "cocoa/CCNS.h"
using namespace cocos2d;

class LHLayer;
class LHSprite;
class LHJoint;
class LHBezier;
class LHLayer;
class LHBatch;
class b2World;


class LHSettings : CCObject
{
private:
	bool m_useRetinaOnIpad;
	float m_lhPtmRatio;
	float m_customAlpha; //used by SceneTester
	int m_newBodyId;
	CCPoint m_convertRatio;
    CCPoint m_realConvertRatio;
	bool m_convertLevel;
	
    bool m_stretchArt;
    CCPoint m_possitionOffset;
    bool m_levelPaused;
    std::string m_imagesFolder;
    bool m_isCoronaUser;
    
    bool m_preloadBatchNodes;
    
    CCArray* allLHMainLayers;
    
#if COCOS2D_VERSION >= 0x00020000
    CCDictionary markedNodes;
    
//    CCDictionary markedSprites;
//    CCDictionary markedLayers;
//    CCDictionary markedBatches;
//    CCDictionary markedJoints;
//    CCDictionary markedBeziers;
#else
    CCMutableDictionary<std::string> markedNodes;
    
//    CCMutableDictionary<std::string> markedSprites;
//    CCMutableDictionary<std::string> markedJoints;
//    CCMutableDictionary<std::string> markedBeziers;    
#endif
    
 
    b2World* activeBox2dWorld;
public:
    
    void addLHMainLayer(LHLayer* layer);
    void removeLHMainLayer(LHLayer* layer);
    
    CCArray* getAllLHMainLayers();
    
    b2World* getActiveBox2dWorld();
    void setActiveBox2dWorld(b2World* world);
    
    void markNodeForRemoval(CCObject* node);
    void removeMarkedNodes();
    
//    void markLayerForRemoval(LHLayer* layer);
//    void markBatchForRemoval(LHBatch* batch);
//    void markSpriteForRemoval(LHSprite* sprite);
//    void markBezierForRemoval(LHBezier* node); 
//    void markJointForRemoval(LHJoint* jt);
//    
//    void removeMarkedLayers();
//    void removeMarkedBatches();
//    void removeMarkedSprites();
//    void removeMarkedBeziers();
//    void removeMarkedJoints();
    
    
    CCPoint transformedScalePointToCocos2d(CCPoint point);
    CCPoint transformedPointToCocos2d(CCPoint point);
    CCPoint transformedPoint(CCPoint point, const std::string& image);
    CCRect transformedTextureRect(CCRect rect, const std::string& image);
    CCSize transformedSize(CCSize size, const std::string& image);
    
    const std::string imagePath(const std::string& file);//will return -hd image when appropriate
    bool isHDImage(const std::string& image);

    
    int newBodyId();
    void setImageFolder(const char* img);
    const std::string& imageFolder(void);
    
    
//    bool shouldScaleImageOnRetina(const std::string& image);
    bool isIpad(void);
    
    void setStretchArt(const bool& value);
    bool getStretchArt(void);
    
    CCPoint possitionOffset(void);
    void setConvertRatio(CCPoint val);
    CCPoint convertRatio(void);
    CCPoint realConvertRatio(void);
    
    int getDevice(){return device;}
    void setDevice(int d){device = d;}
    
    void setHDSuffix(const std::string& str){hdSuffix = str;}
    void setHD2xSuffix(const std::string& str){hd2xSuffix = str;}

    const std::string& getHDSuffix(){return hdSuffix;}
    const std::string& getHD2xSuffix(){return hd2xSuffix;}


    bool useRetinaOnIpad(){return m_useRetinaOnIpad;}
    void setUseRetinaOnIpad(const bool& r){ m_useRetinaOnIpad = r;}
    
    float lhPtmRatio(void){return m_lhPtmRatio;}
    void setLhPtmRatio(const float& ptm){m_lhPtmRatio = ptm;}

    float customAlpha(void){return m_customAlpha;}
    void setCustomAlpha(const float& a){m_customAlpha = a;}
        
    bool convertLevel(void){return m_convertLevel;}
    void setConvertLevel(const bool& c){m_convertLevel = c;}
    
    bool levelPaused(void){return m_levelPaused;}
    void setLevelPaused(const bool& p){m_levelPaused = p;}
    
    bool isCoronaUser(void){return m_isCoronaUser;}
    void setIsCoronaUser(const bool& u){ m_isCoronaUser = u;}
    
    bool preloadBatchNodes(void){return m_preloadBatchNodes;}
    void setPreloadBatchNodes(const bool& p){m_preloadBatchNodes = p;}
    
    static LHSettings* sharedInstance(void);
    
    virtual ~LHSettings(void);
    
private:
    
    int device;//0 iphone only; 1 ipad only; 2 universal; 3 mac - dont do any transformations
    std::string hdSuffix;
    std::string hd2xSuffix;

    void removeMarkedNode(CCObject* node);
    
    LHSettings();
    static LHSettings *m_sharedInstance;
};

#endif
