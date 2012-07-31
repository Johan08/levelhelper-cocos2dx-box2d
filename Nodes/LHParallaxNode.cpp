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
#include "LHParallaxNode.h"
#include "LHSettings.h"
#include "../LevelHelperLoader.h"
#include "LHSprite.h"
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
LHParallaxNode::LHParallaxNode(void){
    
}
////////////////////////////////////////////////////////////////////////////////
LHParallaxNode::~LHParallaxNode(void){
    CCLog("LHParallaxNode dealloc");
    
    if(NULL != followedSprite)
        followedSprite->parallaxFollowingThisSprite = NULL;
    followedSprite = NULL;
    
    if(removeSpritesOnDelete)
    {
#if COCOS2D_VERSION >= 0x00020000
        CCArray* tempSprites = CCArray::create();
#else
        CCArray* tempSprites = CCArray::array();
#endif
        
        tempSprites->addObjectsFromArray(sprites);
    
        for(int i = 0; i< tempSprites->count(); ++i)
        {        
            LHParallaxPointObject* pt = (LHParallaxPointObject*)tempSprites->objectAtIndex(i);
            if(pt->ccsprite){
                ((LHSprite*)pt->ccsprite)->setParallaxNode(NULL);
                ((LHSprite*)pt->ccsprite)->removeSelf();
            }
        }
        tempSprites->removeAllObjects();
    }
    
    sprites->removeAllObjects();
    
    delete sprites;
}
////////////////////////////////////////////////////////////////////////////////
bool LHParallaxNode::initWithDictionary(LHDictionary* parallaxDict, LevelHelperLoader* loader){

    if(NULL == parallaxDict)
        return false;
    
#if COCOS2D_VERSION >= 0x00020000
    sprites = CCArray::create();
#else
    sprites = CCArray::array();
#endif
    sprites->retain();
    
    
    followedSprite = NULL;
    isContinuous = parallaxDict->objectForKey("ContinuousScrolling")->boolValue();
    direction = parallaxDict->objectForKey("Direction")->intValue();
    speed = parallaxDict->objectForKey("Speed")->floatValue();
    lastPosition = CCPointMake(-100,-100);
    paused = false;
    winSize = CCDirector::sharedDirector()->getWinSize();
    screenNumberOnTheRight = 1;
    screenNumberOnTheLeft = 0;
    screenNumberOnTheTop = 0;
    
    removeSpritesOnDelete = false;
    parentLoader = loader;
    
    movedEndListenerObj = NULL;
    movedEndListenerSEL = NULL;
    
    uniqueName  = parallaxDict->objectForKey("UniqueName")->stringValue();
    if(!isContinuous)
        speed = 1.0f;

    return true;
}
////////////////////////////////////////////////////////////////////////////////
LHParallaxNode* LHParallaxNode::nodeWithDictionary(LHDictionary* properties, LevelHelperLoader* loader){
    
    LHParallaxNode *pobNode = new LHParallaxNode();
	if (pobNode && pobNode->initWithDictionary(properties, loader))
    {
	    pobNode->autorelease();
        return pobNode;
    }
    CC_SAFE_DELETE(pobNode);
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
void LHParallaxNode::addSprite(LHSprite* sprite, CCPoint ratio)
{
	CCAssert( sprite != NULL, "Argument must be non-nil");
	
	LHParallaxPointObject *obj = createParallaxPointObject(sprite, ratio);
    sprite->setParallaxNode(this);
	obj->body = sprite->getBody();
}
////////////////////////////////////////////////////////////////////////////////
void LHParallaxNode::addNode(CCNode* node, CCPoint ratio){
    createParallaxPointObject(node, ratio);
}
////////////////////////////////////////////////////////////////////////////////
LHParallaxPointObject* LHParallaxNode::createParallaxPointObject(CCNode* node, CCPoint ratio){

    CCAssert( node != NULL, "Argument must be non-nil");
	
	LHParallaxPointObject *obj = LHParallaxPointObject::pointWithCCPoint(ratio);
	obj->ccsprite = node;

	obj->position = node->getPosition();
	obj->offset = node->getPosition();
	obj->initialPosition = node->getPosition();
    sprites->addObject(obj);
    
	int scrRight = (int)(obj->initialPosition.x/winSize.width);
	
	if(screenNumberOnTheRight <= scrRight)
		screenNumberOnTheRight = scrRight+1;
    
	int scrLeft = (int)(obj->initialPosition.x/winSize.width);
    
	if(screenNumberOnTheLeft >= scrLeft)
		screenNumberOnTheLeft = scrLeft-1;
    
    
	int scrTop = (int)(obj->initialPosition.y/winSize.height);
	
	if(screenNumberOnTheTop <= scrTop)
		screenNumberOnTheTop = scrTop + 1;
	
	int scrBottom = (int)(obj->initialPosition.y/winSize.height);
    
	if(screenNumberOnTheBottom >= scrBottom)
		screenNumberOnTheBottom = scrBottom-1;
    
    return obj;
}
////////////////////////////////////////////////////////////////////////////////
void LHParallaxNode::removeChild(LHSprite* sprite)
{
    if(NULL == sprite) 
        return;
    
    CCLog("REMOVE CHILD FORM PARALLAX");
        
    for(int i = 0; i< sprites->count(); ++i){        
        LHParallaxPointObject* pt = (LHParallaxPointObject*)sprites->objectAtIndex(i);
	
        if(pt->ccsprite == sprite){
            sprites->removeObjectAtIndex(i);
            return;
        }
	}
    
//#endif
}
////////////////////////////////////////////////////////////////////////////////
void LHParallaxNode::registerSpriteHasMovedToEndListener(CCObject* object, SEL_CallFuncN method)
{
    movedEndListenerObj = object; 
    movedEndListenerSEL = method;
}
////////////////////////////////////////////////////////////////////////////////
CCArray* LHParallaxNode::spritesInNode(void)
{
#if COCOS2D_VERSION >= 0x00020000
    CCArray* sprs = CCArray::create();
#else
    CCArray* sprs = CCArray::array();
#endif
    
    for(int i = 0; i < sprites->count(); ++i)
    {
        LHParallaxPointObject* pt = (LHParallaxPointObject*)sprites->objectAtIndex(i);
        if(NULL != pt->ccsprite)
            sprs->addObject((LHSprite*)pt->ccsprite);
    }
    return sprs;
}
////////////////////////////////////////////////////////////////////////////////
std::vector<b2Body*> LHParallaxNode::bodiesInNode(void){
    
    std::vector<b2Body*> sprs;
    
    for(int i = 0; i < sprites->count(); ++i){
        LHParallaxPointObject* pt = (LHParallaxPointObject*)sprites->objectAtIndex(i);
        if(NULL != pt->body)
            sprs.push_back(pt->body);
    }
    return sprs;
}
////////////////////////////////////////////////////////////////////////////////
void LHParallaxNode::setPositionOnPointWithOffset(const CCPoint& pos, 
                                                  LHParallaxPointObject* point, 
                                                  const CCPoint& offset)
{
    if(point == NULL)
        return;
    
    if(!isContinuous)
    {
        if(point->ccsprite != NULL){
            point->ccsprite->setPosition(pos);
        
            if(point->body != NULL){
            
                float angle = point->ccsprite->getRotation();
                point->body->SetAwake(true);
                
                point->body->SetTransform(b2Vec2(pos.x/LHSettings::sharedInstance()->lhPtmRatio(), 
                                                 pos.y/LHSettings::sharedInstance()->lhPtmRatio()), 
                                         CC_DEGREES_TO_RADIANS(-angle));
            }
        }
    }
    else
    {
        if(point->ccsprite != NULL)
        {
            CCPoint newPos = CCPointMake(point->ccsprite->getPosition().x - offset.x,
                                         point->ccsprite->getPosition().y - offset.y);
            point->ccsprite->setPosition(newPos);
            
            if(point->body != NULL){
            
            float angle = point->ccsprite->getRotation();
            point->body->SetTransform(b2Vec2(newPos.x/LHSettings::sharedInstance()->lhPtmRatio(), 
                                             newPos.y/LHSettings::sharedInstance()->lhPtmRatio()), 
                                     CC_DEGREES_TO_RADIANS(-angle));
            }
            
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
CCSize LHParallaxNode::getBounds(float rw, float rh, float radians)
{
    float x1 = -rw/2;
    float x2 = rw/2;
    float x3 = rw/2;
    float x4 = -rw/2;
    float y1 = rh/2;
    float y2 = rh/2;
    float y3 = -rh/2;
    float y4 = -rh/2;
    
    float x11 = x1 * cos(radians) + y1 * sin(radians);
    float y11 = -x1 * sin(radians) + y1 * cos(radians);
    float x21 = x2 * cos(radians) + y2 * sin(radians);
    float y21 = -x2 * sin(radians) + y2 * cos(radians);
    float x31 = x3 * cos(radians) + y3 * sin(radians);
    float y31 = -x3 * sin(radians) + y3 * cos(radians);
    float x41 = x4 * cos(radians) + y4 * sin(radians);
    float y41 = -x4 * sin(radians) + y4 * cos(radians);

    float x_min = MIN(MIN(x11,x21),MIN(x31,x41));
    float x_max = MAX(MAX(x11,x21),MAX(x31,x41));
    
    float y_min = MIN(MIN(y11,y21),MIN(y31,y41));
    float y_max = MAX(MAX(y11,y21),MAX(y31,y41));
 
    return CCSizeMake(x_max-x_min, y_max-y_min);
}
////////////////////////////////////////////////////////////////////////////////
void LHParallaxNode::repositionPoint(LHParallaxPointObject* point)
{
	CCSize spriteContentSize = point->ccsprite->getContentSize();
    
    float angle = point->ccsprite->getRotation();
    float rotation = CC_DEGREES_TO_RADIANS(angle);
	float scaleX = point->ccsprite->getScaleX();
	float scaleY = point->ccsprite->getScaleY();
    
    CCSize contentSize = getBounds(spriteContentSize.width,
                                   spriteContentSize.height,
                                   rotation);
        
	switch (direction) {
		case 1: //right to left
		{
			if(point->ccsprite->getPosition().x + contentSize.width/2.0f*scaleX <= 0)
			{
				float difX = point->ccsprite->getPosition().x + contentSize.width/2.0f*scaleX;
		
				point->setOffset(ccp(winSize.width*screenNumberOnTheRight - point->ratio.x*speed -  contentSize.width/2.0f*scaleX + difX, 
                                     point->offset.y));
	
                if(NULL != point->ccsprite){
                    CCPoint newPos = CCPointMake(point->offset.x, point->ccsprite->getPosition().y);
                    point->ccsprite->setPosition(newPos);
                
                    if(point->body != NULL){
                    
                        float angle = point->ccsprite->getRotation();
                        point->body->SetTransform(b2Vec2(newPos.x/LHSettings::sharedInstance()->lhPtmRatio(), 
                                                         newPos.y/LHSettings::sharedInstance()->lhPtmRatio()), 
                                                 CC_DEGREES_TO_RADIANS(-angle));
                    }
                }
                    
                
                if(NULL != movedEndListenerObj){
                    (movedEndListenerObj->*movedEndListenerSEL)(point->ccsprite);
                }
			}
		}	
			break;
			
		case 0://left to right
		{
			if(point->ccsprite->getPosition().x - contentSize.width/2.0f*scaleX >= winSize.width)
			{
				float difX = point->ccsprite->getPosition().x - contentSize.width/2.0f*scaleX - winSize.width;
				
				point->setOffset(ccp(winSize.width*screenNumberOnTheLeft + point->ratio.x*speed +  contentSize.width/2.0f*scaleX + difX, 
                                     point->offset.y));
                
                
                
                if(NULL != point->ccsprite){
                    CCPoint newPos = CCPointMake(point->offset.x, point->ccsprite->getPosition().y);
                    point->ccsprite->setPosition(newPos);
                    
                    if(point->body != NULL){
                        
                        float angle = point->ccsprite->getRotation();
                        point->body->SetTransform(b2Vec2(newPos.x/LHSettings::sharedInstance()->lhPtmRatio(), 
                                                         newPos.y/LHSettings::sharedInstance()->lhPtmRatio()), 
                                                 CC_DEGREES_TO_RADIANS(-angle));
                    }
                }

                
                if(NULL != movedEndListenerObj){
                    (movedEndListenerObj->*movedEndListenerSEL)(point->ccsprite);
                }
			}
		}
			break;
			
		case 2://up to bottom
		{
			if(point->ccsprite->getPosition().y + contentSize.height/2.0f*scaleY <= 0)
			{
				float difY = point->ccsprite->getPosition().y + contentSize.height/2.0f*scaleY;
				
				point->setOffset(ccp(point->offset.x, 
                                     winSize.height*screenNumberOnTheTop - point->ratio.y*speed - contentSize.height/2.0f*scaleY + difY));
                
                
                if(NULL != point->ccsprite){
                    CCPoint newPos = CCPointMake(point->ccsprite->getPosition().x, point->offset.y);
                    point->ccsprite->setPosition(newPos);
                    
                    if(point->body != NULL){
                        
                        float angle = point->ccsprite->getRotation();
                        point->body->SetTransform(b2Vec2(newPos.x/LHSettings::sharedInstance()->lhPtmRatio(), 
                                                         newPos.y/LHSettings::sharedInstance()->lhPtmRatio()), 
                                                  CC_DEGREES_TO_RADIANS(-angle));
                    }
                }
                
                if(NULL != movedEndListenerObj){
                    (movedEndListenerObj->*movedEndListenerSEL)(point->ccsprite);
                }
			}
		}
			break;
			
		case 3://bottom to top
		{
			if(point->ccsprite->getPosition().y - contentSize.height/2.0f*scaleY >= winSize.height)
			{
				float difY = point->ccsprite->getPosition().y - contentSize.height/2.0f*scaleY - winSize.height;
				
				point->setOffset(ccp(point->offset.x, 
                                     winSize.height*screenNumberOnTheBottom + point->ratio.y*speed + contentSize.height/2.0f*scaleY + difY));
                
                if(NULL != point->ccsprite){
                    CCPoint newPos = CCPointMake(point->ccsprite->getPosition().x, point->offset.y);
                    point->ccsprite->setPosition(newPos);
                    
                    if(point->body != NULL){
                        
                        float angle = point->ccsprite->getRotation();
                        point->body->SetTransform(b2Vec2(newPos.x/LHSettings::sharedInstance()->lhPtmRatio(), 
                                                         newPos.y/LHSettings::sharedInstance()->lhPtmRatio()), 
                                                 CC_DEGREES_TO_RADIANS(-angle));
                    }
                }
                
                if(NULL != movedEndListenerObj){
                    (movedEndListenerObj->*movedEndListenerSEL)(point->ccsprite);
                }
			}
		}
			break;
		default:
			break;
	}
}
////////////////////////////////////////////////////////////////////////////////
void LHParallaxNode::setPosition(CCPoint newPosition)
{
    CCNode::setPosition(newPosition);
    visit();
}
////////////////////////////////////////////////////////////////////////////////
void LHParallaxNode::setFollowSprite(LHSprite* sprite, 
                                     bool changeXPosition, 
                                     bool changeYPosition){
    
    if(NULL == sprite)
    {
        if(NULL != followedSprite)
            followedSprite->parallaxFollowingThisSprite = NULL;
    }
    
    followedSprite = sprite;
    
    followChangeX = changeXPosition;
    followChangeY = changeYPosition;
    
    if(NULL != sprite)
    {
        lastFollowedSpritePosition = sprite->getPosition();
        sprite->parallaxFollowingThisSprite = this;
    }
}
////////////////////////////////////////////////////////////////////////////////
void LHParallaxNode::visit(void)
{
    if(LHSettings::sharedInstance()->levelPaused()) //level is paused
        return;
    
    if(paused) //this parallax is paused
        return;
    
    
    if(NULL != followedSprite)
    {
        float deltaFX = lastFollowedSpritePosition.x - followedSprite->getPosition().x;
        float deltaFY = lastFollowedSpritePosition.y - followedSprite->getPosition().y;
        lastFollowedSpritePosition = followedSprite->getPosition();
 
        CCPoint lastPosition = this->getPosition();        
        if(followChangeX && !followChangeY)
        {
            CCNode::setPosition(ccp(lastPosition.x + deltaFX, lastPosition.y));
        }
        else if(!followChangeX && followChangeY)
        {
            CCNode::setPosition(ccp(lastPosition.x, lastPosition.y + deltaFY));
        }
        else if(followChangeX && followChangeY)
        {
            CCNode::setPosition(ccp(lastPosition.x + deltaFX, lastPosition.y + deltaFY));
        }
    }
    
	CCPoint pos = getPosition();
	if( ! CCPoint::CCPointEqualToPoint(pos, lastPosition) || isContinuous) 
	{
        for(int k = 0; k < sprites->count(); ++k)
        {
//#if COCOS2D_VERSION >= 0x00020000
            LHParallaxPointObject* point = (LHParallaxPointObject*)sprites->objectAtIndex(k);
//#else
//            LHParallaxPointObject* point = (LHParallaxPointObject*)sprites->getObjectAtIndex(k);
//#endif
            
			if(NULL != point && point->ccsprite != NULL)
            {
                
                float x = pos.x * point->ratio.x + point->offset.x;
                float y = pos.y * point->ratio.y + point->offset.y;	

                int i = -1; //direction left to right //bottom to up
                if(direction == 1 || direction == 2) //right to left //up to bottom
                    i = 1;
            
                setPositionOnPointWithOffset(CCPointMake(x, y), 
                                             point, 
                                             CCPointMake(i*point->ratio.x*speed, 
                                                         i*point->ratio.y*speed));

                if(isContinuous)
                {
                    repositionPoint(point);
			
                    point->setOffset(ccp(point->offset.x + i*point->ratio.x*speed, 
                                         point->offset.y + i*point->ratio.y*speed));
                }
            }
		}
		lastPosition = pos;
	}
}
