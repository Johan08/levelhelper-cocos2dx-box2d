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

#ifndef __LH_BEZIER_NODE__
#define __LH_BEZIER_NODE__

#include "cocos2d.h"
#include "Box2d/Box2D.h"
#include "../Utilities/LHDictionary.h"
#include "LHTouchMgr.h"

using namespace cocos2d;

class LHSprite;
class LHPathNode;
class LevelHelperLoader;

typedef struct __LHBezierBlendingInfo{
  
    CCTexture2D* texture;
    GLenum blendSource;
    GLenum blendDestination;
    bool tile;
} LHBezierBlendingInfo;


class LHBezierNode : public CCNode, public CCStandardTouchDelegate
{
protected:
    
	bool isClosed;
	bool isTile;
	bool isVisible;
	bool isLine;
	bool isPath;
    std::string uniqueName;
	b2Body* body; //can be 0
    std::vector<CCPoint> pathPoints;
    //CCMutableArray<LHPathNode*> pathNodes;
	
	/////////for the tile feature
	CCTexture2D *texture;
	CCRect color;
	CCRect lineColor;
	float lineWidth;
	CCSize winSize;
    std::vector<std::vector<CCPoint> > trianglesHolder;
    std::vector<CCPoint> linesHolder;

    std::vector<LHBezierBlendingInfo> blendingTextures;
    
	CCSize imageSize;
    
    //static int numberOfBezierNodes;
    
    friend class LevelHelperLoader;    
    
    LHObserverPair touchBeginObserver;
    LHObserverPair touchMovedObserver;
    LHObserverPair touchEndedObserver;
    bool swallowTouches;
    LHObserverPair* tagTouchBeginObserver;
    LHObserverPair* tagTouchMovedObserver;
    LHObserverPair* tagTouchEndedObserver;
    
public:
    
    bool getIsClosed(void){return isClosed;}
 
    bool getIsTile(void){return isTile;}
  
    bool getIsVisible(void){return isVisible;}
    void setIsVisible(const bool& v){isVisible = v;}
    
    bool getIsLine(void){return isLine;}

    std::string& getUniqueName(void){return uniqueName;}
  
    b2Body* getBody(void){return body;};
    
    
    void pushBlendingTextureNamed(const std::string& texName, 
                                  bool tile = true,
                                  GLenum blendSource = GL_DST_COLOR, 
                                  GLenum blendDestination = GL_ZERO);
    
    static bool isLHBezierNode(CCNode* obj);
    
    virtual void init(void);
    virtual ~LHBezierNode(void);
    LHBezierNode(void);
    
    bool initWithDictionary(LHDictionary* properties, 
                            CCLayer* ccLayer, 
                            b2World* world);
    static LHBezierNode* nodeWithDictionary(LHDictionary* properties, 
                                            CCLayer* ccLayer, 
                                            b2World* world);
    
    LHPathNode* addSpriteOnPath(LHSprite* spr, 
                                float   pathSpeed, 
                                bool    startAtEndPoint,
                                bool    isCyclic,
                                bool    restartOtherEnd,
                                int     axis,
                                bool    flipx,
                                bool    flipy,
                                bool    deltaMove);
    
    virtual void draw(void);
    
    //TOUCH METHODS
    //------------------------------------------------------------------------------
    //Touches are detected only on bezier tile shapes.
    //If the bezier is not a tile shape no touches events will be performed.
    bool isTouchedAtPoint(CCPoint point);
    
    //selector needs to have this signature void HelloWorldLayer::touchXXX:(LHTouchInfo*)info
    //in visual studio the method signature should be 
    //void HelloWorldLayer::touchXXX(CCObject* cinfo){LHTouchInfo* info = (LHTouchInfo*)info; ...} 
    //because compiler doesn't know how to cast
    //info will have all information regarding the touch (see API Documentation or top of this file)
    //for generic touch on sprites with tag use the observers from LevelHelperLoader
    void registerTouchBeginObserver(CCObject* observer, SEL_CallFuncO selector);
    void registerTouchMovedObserver(CCObject* observer, SEL_CallFuncO selector);
    void registerTouchEndedObserver(CCObject* observer, SEL_CallFuncO selector);

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

    virtual void touchDelegateRetain() {} //compatibility with old cocos2d-x version
    virtual void touchDelegateRelease() {}//compatibility with old cocos2d-x version
private:
    
    static CCPoint pointOnCurve(CCPoint p1, CCPoint p2, CCPoint p3, CCPoint p4, float t);
    
    void initTileVerticesFromDictionary(LHDictionary* bezierDict);
    void initPathPointsFromDictionary(LHDictionary* bezierDict);
    void createBodyFromDictionary(LHDictionary* bezierDict, b2World* world);
    
    void setTagTouchBeginObserver(LHObserverPair* pair);
    void setTagTouchMovedObserver(LHObserverPair* pair);
    void setTagTouchEndedObserver(LHObserverPair* pair);
};

#endif
