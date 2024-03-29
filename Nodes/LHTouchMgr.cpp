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

#include "LHTouchMgr.h"
#include "LHSprite.h"
#include "LHBezierNode.h"
#include "../LevelHelperLoader.h"


LHTouchInfo::LHTouchInfo(void){
        
    event = NULL;
    touch = NULL;
    sprite= NULL;
    bezier= NULL;
}
LHTouchInfo::~LHTouchInfo(void){
    
}  
LHTouchInfo* LHTouchInfo::touchInfo(){
 
    LHTouchInfo *pobTouch = new LHTouchInfo();
	if (pobTouch)
    {
	    pobTouch->autorelease();
        return pobTouch;
    }
    CC_SAFE_DELETE(pobTouch);
	return NULL;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
LHObserverPair::~LHObserverPair(void){
}
LHObserverPair::LHObserverPair(void){
    object = NULL;
    selector = NULL;
}
LHObserverPair* LHObserverPair::observerPair(){
    LHObserverPair *pobObserver = new LHObserverPair();
	if (pobObserver)
    {
	    pobObserver->autorelease();
        return pobObserver;
    }
    CC_SAFE_DELETE(pobObserver);
	return NULL;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
LHTouchMgr *LHTouchMgr::m_sharedInstance = 0;
//------------------------------------------------------------------------------
LHTouchMgr* LHTouchMgr::sharedInstance(){
	if (0 == m_sharedInstance){
		m_sharedInstance = new LHTouchMgr();
	}
    return m_sharedInstance;
}
//------------------------------------------------------------------------------
LHTouchMgr::~LHTouchMgr(){
    onTouchBeginOnSpriteOfTag.removeAllObjects();
    onTouchMovedOnSpriteOfTag.removeAllObjects();
    onTouchEndedOnSpriteOfTag.removeAllObjects();
    swallowTouchesOnTag.clear();
    priorityForTouchesOfTag.clear();
}
//------------------------------------------------------------------------------
LHTouchMgr::LHTouchMgr(){
}
//------------------------------------------------------------------------------
void LHTouchMgr::setPriorityForTouchesOfTag(int priority, int tag){
    priorityForTouchesOfTag[tag] = priority;
}
//------------------------------------------------------------------------------
void LHTouchMgr::swallowTouchesForTag(int tag){
    swallowTouchesOnTag[tag] = true;
}
//------------------------------------------------------------------------------
void LHTouchMgr::registerTouchBeginObserverForTag(CCObject* observer, 
                                                  SEL_CallFuncO selector, 
                                                  int tag){

    LHObserverPair* pair = LHObserverPair::observerPair();
    pair->object = observer;
    pair->selector = selector;
    onTouchBeginOnSpriteOfTag.setObject(pair, tag);
}
//------------------------------------------------------------------------------
void LHTouchMgr::registerTouchMovedObserverForTag(CCObject* observer, 
                                                  SEL_CallFuncO selector, 
                                                  int tag){
    
    LHObserverPair* pair = LHObserverPair::observerPair();
    pair->object = observer;
    pair->selector = selector;
    onTouchMovedOnSpriteOfTag.setObject(pair, tag);
}
//------------------------------------------------------------------------------
void LHTouchMgr::registerTouchEndedObserverForTag(CCObject* observer, 
                                                  SEL_CallFuncO selector, 
                                                  int tag){
    
    LHObserverPair* pair = LHObserverPair::observerPair();
    pair->object = observer;
    pair->selector = selector;
    onTouchEndedOnSpriteOfTag.setObject(pair, tag);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
LHObserverPair* LHTouchMgr::onTouchBeginObserverForTag(int tag){
    return (LHObserverPair*)onTouchBeginOnSpriteOfTag.objectForKey(tag);    
}
//------------------------------------------------------------------------------
LHObserverPair* LHTouchMgr::onTouchMovedObserverForTag(int tag){
    return (LHObserverPair*)onTouchMovedOnSpriteOfTag.objectForKey(tag);        
}
//------------------------------------------------------------------------------
LHObserverPair* LHTouchMgr::onTouchEndedObserverForTag(int tag){
    return (LHObserverPair*)onTouchEndedOnSpriteOfTag.objectForKey(tag);    
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool LHTouchMgr::shouldTouchesBeSwallowedForTag(int tag){

    std::map<int, bool>::iterator it =swallowTouchesOnTag.find(tag);
    if(it == swallowTouchesOnTag.end()){
        return false;
    }
    return true;
}
//------------------------------------------------------------------------------
int LHTouchMgr::priorityForTag(int tag){

    std::map<int, int>::iterator it =priorityForTouchesOfTag.find(tag);
    if(it == priorityForTouchesOfTag.end()){
        return 0;
    }
    return it->second;

}
//------------------------------------------------------------------------------