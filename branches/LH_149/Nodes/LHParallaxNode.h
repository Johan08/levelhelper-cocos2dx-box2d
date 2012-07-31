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

#ifndef __LH_PARALLAX_NODE__
#define __LH_PARALLAX_NODE__
#include "cocos2d.h"
#include "../Utilities/LHDictionary.h"
#include "../Utilities/LHObject.h"
#include "../Utilities/LHArray.h"
#include "Box2d/Box2D.h"

using namespace cocos2d;
class LHSprite;
class LevelHelperLoader;

class LHParallaxPointObject : public CCObject
{
public:
	CCPoint position;
	CCPoint	ratio;
	CCPoint offset;
	CCPoint initialPosition;
	CCNode *ccsprite;	//weak ref
	b2Body *body;		//weak ref
    bool isLHSprite;
    

    virtual ~LHParallaxPointObject(void)
    {
        ccsprite = NULL;
        body = NULL;
        //CCLog("LHPARALLAX_POINT_OBJECT_DEALLOC");
    }
    
    LHParallaxPointObject(void){
        ccsprite = NULL;
        body = NULL;
        ratio = ccp(0,0);
        initialPosition = ccp(0,0);
        offset = ccp(0,0);
        position = ccp(0,0);
        isLHSprite = false;
    }
    bool initWithCCPoint(CCPoint point){
        ratio = point;
        return true;
    }
    
    void setOffset(const CCPoint& pt){offset = pt;}
    
    static LHParallaxPointObject* pointWithCCPoint(CCPoint point){

        LHParallaxPointObject *pobPoint = new LHParallaxPointObject();
        if (pobPoint && pobPoint->initWithCCPoint(point))
        {
            pobPoint->autorelease();
            return pobPoint;
        }
        CC_SAFE_DELETE(pobPoint);
        return NULL;
    }
};

//---------------------------------------------------------------------------
class LHParallaxNode : public CCNode
{
private:
    
	bool isContinuous;
	int direction;
	float speed;
	bool paused;
	
	CCPoint lastPosition;
    std::string uniqueName;
	    
    CCObject* movedEndListenerObj;
    SEL_CallFuncN movedEndListenerSEL;
    
	CCSize winSize;
	
	int screenNumberOnTheRight;
	int screenNumberOnTheLeft;
	int screenNumberOnTheTop;
	int screenNumberOnTheBottom;
	
    //CCArray sprites;
//#if COCOS2D_VERSION >= 0x00020000
    CCArray* sprites; //contains LHParallaxPointObject*
//#else
//    CCArray
//    CCMutableArray<LHParallaxPointObject*>* sprites;
//#endif
    
    LHSprite* followedSprite;
    CCPoint lastFollowedSpritePosition;
    bool followChangeX;
    bool followChangeY;
    
    bool removeSpritesOnDelete;
    LevelHelperLoader* parentLoader;
    
    friend class LevelHelperLoader;
    
public:
    
    LHParallaxNode(void);
    virtual ~LHParallaxNode(void);

    void setPosition(CCPoint pos);
    
    //will make the parallax move based on the sprite position (e.g player)
    //pass NULL to this function to unfollow the sprite
    void setFollowSprite(LHSprite* sprite, bool changeXPosition = true, bool changeYPosition = false);
    
    bool initWithDictionary(LHDictionary* properties, LevelHelperLoader* loader);
    static LHParallaxNode* nodeWithDictionary(LHDictionary* properties, LevelHelperLoader* loader);

    bool getIsContinuous(void){return  isContinuous;}
    int getDirection(void){return direction;}
    
    float getSpeed(void){return speed;}
    void setSpeed(const float& s){speed = s;}
    
    bool getIsPaused(void){return paused;}
    void setPaused(const bool& pause){paused = pause;}
    
    void addSprite(LHSprite* sprite, CCPoint ratio);
    void addNode(CCNode* node, CCPoint ratio);
    
    void removeChild(LHSprite* sprite);
    //method needs to be like this -(void)spriteMovedToEnd:(LHSprite*)spr;
    void registerSpriteHasMovedToEndListener(CCObject* object, SEL_CallFuncN method);
    
    std::string getUniqueName(void){return uniqueName;}
    CCArray* spritesInNode(void);
    std::vector<b2Body*> bodiesInNode(void); //better to use spritesInNode and take the body from the LHSprite*
    
//    virtual void visit(void);
    virtual void tick(float timeInterval);
private:
    
    void setPositionOnPointWithOffset(const CCPoint& pos, 
                                      LHParallaxPointObject* point, 
                                      const CCPoint& offset);
    
    CCSize getBounds(float rw, float rh, float radians);
    
    void repositionPoint(LHParallaxPointObject* point, double frameTime);
//    void repositionPoint(LHParallaxPointObject* point);

    LHParallaxPointObject* createParallaxPointObject(CCNode* node, CCPoint ratio);
};

#endif
