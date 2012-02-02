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


#ifndef __LH_TOUCH_MGR_SINGLETON
#define __LH_TOUCH_MGR_SINGLETON

#include "cocos2d.h"
//#include "LHSprite.h"
//#include "LHBezierNode.h"
//#include "LevelHelperLoader.h"

class LHBezierNode;
class LHSprite;

using namespace cocos2d;

class LHTouchInfo : public CCObject
{
public:
    CCPoint relativePoint; //the touch point relative to the center of the sprite (does not work for bezier shapes)
    CCPoint glPoint; //the touch point converted to OpenGL coordinates;
    CCPoint delta; //the delta movement -- (0, 0) on touchBegin
    

    CCEvent* event; //the event
    CCTouch* touch; //the touch 
    LHSprite* sprite; //the sprite on which the touch was performed (nil if touch was performed on a bezier)
    LHBezierNode* bezier;//the bezier tile shape on which the touch was performed (nil if touch was performed on a sprite)
    
    virtual ~LHTouchInfo(void);
    
    static LHTouchInfo* touchInfo();
    
private:
    LHTouchInfo(void);    
};


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class LHObserverPair : public CCObject{
    
public:
    SelectorProtocol* object;
    SEL_CallFuncO selector;     
    
    virtual ~LHObserverPair(void);
    static LHObserverPair* observerPair();
    LHObserverPair(void);    
};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


class LHTouchMgr
{
public:
    static LHTouchMgr* sharedInstance(void);
    virtual ~LHTouchMgr(void);

    //REGISTER FOR TOUCH EVENTS BEFORE LOADING THE LEVEL FILE
    void setPriorityForTouchesOfTag(int priority, int tag); //call this only if you want other priority then 0
    void swallowTouchesForTag(int tag); //call this only if you want to swallow the touches
    
    //selector needs to have this signature -(void) touchXXX:(LHTouchInfo*)info
    //sprite will be the LHSprite* instance on which the touch was performed that has the registered tag
    //for specific touch on a certain sprite use the observer from LHSprite
    void registerTouchBeginObserverForTag(SelectorProtocol* observer, SEL_CallFuncO selector, int tag);
    void registerTouchMovedObserverForTag(SelectorProtocol* observer, SEL_CallFuncO selector, int tag);
    void registerTouchEndedObserverForTag(SelectorProtocol* observer, SEL_CallFuncO selector, int tag);
    
    
    //get back the observer that was registered for a specific tag
    LHObserverPair* onTouchBeginObserverForTag(int tag);
    LHObserverPair* onTouchMovedObserverForTag(int tag);
    LHObserverPair* onTouchEndedObserverForTag(int tag);
    
    bool shouldTouchesBeSwallowedForTag(int tag);
    int priorityForTag(int tag);

private:
    CCMutableDictionary<int> onTouchBeginOnSpriteOfTag;
    CCMutableDictionary<int> onTouchMovedOnSpriteOfTag;
    CCMutableDictionary<int> onTouchEndedOnSpriteOfTag;
    std::map<int, bool> swallowTouchesOnTag;
    std::map<int, int> priorityForTouchesOfTag;
    
    LHTouchMgr();
    static LHTouchMgr *m_sharedInstance;
};

#endif
