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
#include "LHSprite.h"

#include "LHAnimationNode.h"
#include "../LevelHelperLoader.h"
#include "LHPathNode.h"
#include "LHParallaxNode.h"
#include "LHSettings.h"
#include "LHJoint.h"
//int LHSprite::numberOfSprites = 0;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
LHSprite::~LHSprite(void){

    removeBodyFromWorld();
    
    if(NULL != parallaxFollowingThisSprite)
        parallaxFollowingThisSprite->setFollowSprite(NULL);

    //CCLog("LHSPRITE Destructor %d", --numberOfSprites);
    stopAllActions();
           
    cancelPathMovement();
    
    if(NULL != parallaxNode){
        parallaxNode->removeChild(this);
    }
    parallaxNode = NULL;
}
////////////////////////////////////////////////////////////////////////////////
bool LHSprite::init(void){
    return CCSprite::init();
}
////////////////////////////////////////////////////////////////////////////////
LHSprite::LHSprite(){

    //CCLog("LHSprite init");
    body = NULL;
    parentLoader = NULL;
    currentFrame = 0;
    pathNode = NULL;
    parallaxNode = NULL;
    parallaxFollowingThisSprite = NULL;
    
    tagTouchBeginObserver = NULL;
    tagTouchMovedObserver = NULL;
    tagTouchEndedObserver = NULL;
    swallowTouches = false;

   // ++numberOfSprites;
   // CCLog("LHSprite Constructor %d", numberOfSprites);
}
////////////////////////////////////////////////////////////////////////////////
void LHSprite::setUniqueName(const char* name){
    uniqueName = std::string(name);
}
//------------------------------------------------------------------------------
const std::string& LHSprite::getUniqueName(void){
    return uniqueName;
}
////////////////////////////////////////////////////////////////////////////////
void  LHSprite::setBody(b2Body* bd){
    CCAssert(bd!=0, "b2Body must not be nil");
    body = bd;
}
//------------------------------------------------------------------------------
b2Body*  LHSprite::getBody(void){
    return body;
}
//------------------------------------------------------------------------------
bool  LHSprite::removeBodyFromWorld(void){
    
    //CCLog("remove body from world");
    if(NULL != body)
	{
		b2World* _world = body->GetWorld();
		if(0 != _world)
		{
            CCArray* list = jointList();
            if(list && parentLoader){
                for(int i = 0; i < (int)list->count(); ++i){
                    LHJoint* jt = (LHJoint*)list->objectAtIndex(i);
                    if(jt){
                        jt->shouldDestroyJointOnDealloc = false;
                        parentLoader->removeJoint(jt);
                    }
                }
                list->removeAllObjects();
            }
            
			_world->DestroyBody(body);
			body = NULL;
            return true;
		}
	}
    return false;    
}
//------------------------------------------------------------------------------
bool LHSprite::removeAllAttachedJoints(void){

    CCArray* list = jointList();
    if(list && parentLoader){
        for(int i = 0; i < (int)list->count(); ++i){
            LHJoint* jt = (LHJoint*)list->objectAtIndex(i);
            if(jt){
                if(jt->getJoint()->GetBodyA()->GetContactList() ||
                   jt->getJoint()->GetBodyB()->GetContactList())
                    parentLoader->markJointForRemoval(jt);
                else
                    parentLoader->removeJoint(jt);
            }
        }
        list->removeAllObjects();
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool LHSprite::removeJoint(LHJoint* jt){
 
    if(jt == NULL)
        return false;
    
    if(body != NULL){
        b2JointEdge* jtList = body->GetJointList();
        while (jtList) {
            LHJoint* lhJt = LHJoint::jointFromBox2dJoint(jtList->joint);
            if(lhJt == jt){
                if(parentLoader){
                    if(jt->getJoint()->GetBodyA()->GetContactList() ||
                       jt->getJoint()->GetBodyB()->GetContactList())
                        parentLoader->markJointForRemoval(jt);
                    else
                        parentLoader->removeJoint(jt);
                    
                    return true;
                }
            }
            jtList = jtList->next;
        }
    }
    CCLog("WARNING: Trying to remove joint %s from the sprite %s but the joint does not belong to that sprite. Removal of joint was not performed.", jt->getUniqueName().c_str(), uniqueName.c_str());
    return false;
}
////////////////////////////////////////////////////////////////////////////////
void LHSprite::setAnimation(LHAnimationNode* anim){
    animation = anim;

    if(0 != anim){
        anim->setAnimationTexturePropertiesOnSprite(this);
        setFrame(0);
    }
}
//------------------------------------------------------------------------------
LHAnimationNode* LHSprite::getAnimation(void){
    return animation;
}
//------------------------------------------------------------------------------
std::string LHSprite::getAnimationName(void){

    if(0 != animation)
        return animation->getUniqueName();
    
    return std::string("");
}
//------------------------------------------------------------------------------
int LHSprite::getNumberOfFrames(void){

    if(0 != animation)
        return animation->getNumberOfFrames();
    return -1;
}
//------------------------------------------------------------------------------
void LHSprite::setFrame(int frmNo){

    if(animation == 0)
        return;
    
    animation->setFrame(frmNo, this);
    currentFrame = frmNo;
}
//------------------------------------------------------------------------------
void LHSprite::nextFrame(){
    
    int curFrame = getCurrentFrame();
    curFrame +=1;
    
    if(curFrame >= 0 && curFrame < getNumberOfFrames()){
        setFrame(curFrame);
    }   
    
}
//------------------------------------------------------------------------------
void LHSprite::prevFrame(){
    
    int curFrame = getCurrentFrame();
    curFrame -=1;
    
    if(curFrame >= 0 && curFrame < (int)getNumberOfFrames()){
         setFrame(curFrame);
    }    
}
//------------------------------------------------------------------------------
void LHSprite::nextFrameAndRepeat(){
    
    int curFrame = getCurrentFrame();

    curFrame +=1;
    
    if(curFrame >= getNumberOfFrames()){
        curFrame = 0;
    }
    
    if(curFrame >= 0 && curFrame < getNumberOfFrames()){
        setFrame(curFrame);
    }    
}
//------------------------------------------------------------------------------
void LHSprite::prevFrameAndRepeat(){
    
    int curFrame =  getCurrentFrame();
    curFrame -=1;
    
    if(curFrame < 0){
        curFrame = getNumberOfFrames() - 1;        
    }
    
    if(curFrame >= 0 && curFrame < (int)getNumberOfFrames()){
        setFrame(curFrame);
    }    
}
//------------------------------------------------------------------------------
bool LHSprite::isAtLastFrame(){
    return (getNumberOfFrames()-1 == getCurrentFrame());
}
//------------------------------------------------------------------------------
int LHSprite::getCurrentFrame(void){
   
    if(0 != animation){
        CCMutableArray<CCSpriteFrame*> *frames = animation->getFrames();
        if(0 != frames)
        {
            for(int i = 0; i < (int)frames->count(); ++i){
                CCSpriteFrame* frame = (CCSpriteFrame*)frames->getObjectAtIndex(i);
                
                if(CCRect::CCRectEqualToRect(frame->getRect(), getTextureRect())){
                    return i;
                }
            }
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
void LHSprite::startAnimationNamed(const std::string& animName,                         
                                   int startFrame,
                                   CCObject* customAnimNotifierId,
                                   SEL_CallFuncND customAnimNotifierSel,
                                   bool observeLooping){
    
    if(parentLoader == NULL)
        return;
    
    
    LHAnimationNode* animNode = (LHAnimationNode*)parentLoader->animationsInLevel.objectForKey(animName);
    if(NULL != animNode){
        
        LHBatch* batch = parentLoader->batchNodeForFile(animNode->getImageName());
        
        if(batch)
        {
            animNode->setBatchNode(batch->getSpriteBatchNode());
            animNode->computeFrames();
            
            if(customAnimNotifierId == NULL){
                animNode->runAnimationOnSprite(this, 
                                               startFrame,
                                               parentLoader->animNotifierId,
                                               parentLoader->animNotifierSel, 
                                               parentLoader->notifOnLoopForeverAnim); 
            }
            else
            {
                animNode->runAnimationOnSprite(this, 
                                               startFrame,
                                               customAnimNotifierId,
                                               customAnimNotifierSel, 
                                               observeLooping);    
            }
        }
    }
}
//------------------------------------------------------------------------------
void LHSprite::setAnimationSequence(CCObject* seqObj)
{        
	CCAction* seq = (CCAction*)seqObj;//visual studio compatibility
    stopActionByTag(LH_ANIM_ACTION_TAG);
    runAction(seq);
}
//------------------------------------------------------------------------------
//does not start the animation - just changed the texture. 
//use this when you want to use nextFrame / prevFrame methods
void LHSprite::prepareAnimationNamed(const std::string& animName){
 
    if(parentLoader == NULL)
        return;
    
    LHAnimationNode* animNode = (LHAnimationNode*)parentLoader->animationsInLevel.objectForKey(animName);
    if(animNode == NULL)
        return;
    
    LHBatch* batch = parentLoader->batchNodeForFile(animNode->getImageName());
    
    if(batch)
    {
        animNode->setBatchNode(batch->getSpriteBatchNode());
        animNode->computeFrames();
        setAnimation(animNode);
    }    
}
//------------------------------------------------------------------------------
void LHSprite::stopAnimation(){
    stopActionByTag(LH_ANIM_ACTION_TAG);
    setAnimation(NULL);
}
////////////////////////////////////////////////////////////////////////////////
void LHSprite::setPathNode(LHPathNode* node){
    CCAssert(node != NULL, "Argument must not be null");
    pathNode = node;
}
void  LHSprite::moveOnPathWithUniqueName(const std::string& pathUniqueName,
                                         float time,
                                         bool startAtEndPoint,
                                         bool isCyclic,
                                         bool restartOtherEnd,
                                         int axisOrientation,
                                         bool flipx,
                                         bool flipy,
                                         bool dMove,
                                         CCObject* obj,
                                         SEL_CallFuncN sel){
    
    if(NULL == parentLoader)
        return;

	cancelPathMovement();
    
	LHBezierNode* node = parentLoader->bezierNodeWithUniqueName(pathUniqueName);
	
	if(NULL != node)
	{
		LHPathNode* pNode = node->addSpriteOnPath(this,
                                                     time,
                                                     startAtEndPoint,
                                                     isCyclic,
                                                     restartOtherEnd,
                                                     axisOrientation,
                                                     flipx,
                                                     flipy,
                                                     dMove);
        
        if(NULL != pNode)
        {
            pNode->setPathNotifierObject(obj);
            pNode->setPathNotifierSelector(sel);
        }
        
        pathNode = pNode;
	}
}
//------------------------------------------------------------------------------
LHPathNode* LHSprite::getPathNode(void){
    return pathNode;
}
//------------------------------------------------------------------------------
void LHSprite::cancelPathMovement(void){
    if(NULL != pathNode){
        pathNode->removeFromParentAndCleanup(true);
        pathNode = NULL;
    }
}
//------------------------------------------------------------------------------
void LHSprite::pausePathMovement(const bool& pauseStatus){
    if(NULL != pathNode){
        pathNode->setPaused(pauseStatus);
    }
}
//------------------------------------------------------------------------------
void LHSprite::setPathSpeed(float value){
    
    if(pathNode != NULL){
        pathNode->setSpeed(value);
    }
}
//------------------------------------------------------------------------------
float LHSprite::getPathSpeed(void){
    if(pathNode != NULL)
        return pathNode->getSpeed();
    return 0;
}
//------------------------------------------------------------------------------
void LHSprite::registerNotifierOnPathEndPoints(CCObject* obj, 
                                               SEL_CallFuncN sel)
{    
    if(NULL == pathNode)
        return;
    
    pathNode->setPathNotifierObject(obj);
    pathNode->setPathNotifierSelector(sel);   
}
////////////////////////////////////////////////////////////////////////////////
void LHSprite::setCustomValue(void* value, const char* key){
    CCAssert(value!=0, "Custom value object must not be nil");    
    CCAssert(key!=0, "Custom value key must not be nil");    
    
    customUserValues[std::string(key)] = value;
}
void* LHSprite::getCustomValueWithKey(const char* key){
    CCAssert(key!=0, "Custom value key must not be nil");    
    return customUserValues[std::string(key)];
}
////////////////////////////////////////////////////////////////////////////////
void LHSprite::transformPosition(CCPoint pos){
    setPosition(pos);
    if(0 != body)
    {
        b2Vec2 boxPosition = LevelHelperLoader::pointsToMeters(pos);
        float angle = CC_DEGREES_TO_RADIANS(-1*getRotation());
        body->SetTransform(boxPosition, angle);
    }
}
void LHSprite::transformRotation(float rot){
    setRotation(rot);
    if(0 != body)
    {
        b2Vec2 boxPosition = LevelHelperLoader::pointsToMeters(getPosition());
        float angle = CC_DEGREES_TO_RADIANS(-1*rot);
        body->SetTransform(boxPosition, angle);
    }    
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CCArray* LHSprite::jointList(void){
    CCArray* array = CCArray::array();
    if(body != NULL){
        b2JointEdge* jtList = body->GetJointList();
        while (jtList) {
            LHJoint* lhJt = LHJoint::jointFromBox2dJoint(jtList->joint);
            if(lhJt != NULL)
                array->addObject(lhJt);
            jtList = jtList->next;
        }
    }
    return array;
}
LHJoint* LHSprite::jointWithUniqueName(const std::string& name){
    
    if(body != NULL){
        b2JointEdge* jtList = body->GetJointList();
        while (jtList) {
            LHJoint* lhJt = LHJoint::jointFromBox2dJoint(jtList->joint);
            if(lhJt != NULL)
                if(lhJt->getUniqueName() == name)
                    return lhJt;
            jtList = jtList->next;
        }
    }
    return NULL;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
LHSprite* LHSprite::sprite(void){
    LHSprite *pobSprite = new LHSprite();
	if (pobSprite && pobSprite->init())
    {
	    pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
	return NULL;
}
LHSprite* LHSprite::spriteWithTexture(CCTexture2D *pTexture)
{
	LHSprite *pobSprite = new LHSprite();
	if (pobSprite && pobSprite->initWithTexture(pTexture))
    {
	    pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
	return NULL;
}

LHSprite* LHSprite::spriteWithTexture(CCTexture2D *pTexture, const CCRect& rect)
{
    LHSprite *pobSprite = new LHSprite();
	if (pobSprite && pobSprite->initWithTexture(pTexture, rect))
    {
	    pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
	return NULL;
}

LHSprite* LHSprite::spriteWithTexture(CCTexture2D *pTexture, const CCRect& rect, const CCPoint& offset)
{
    CC_UNUSED_PARAM(pTexture);
    CC_UNUSED_PARAM(rect);
    CC_UNUSED_PARAM(offset);
	// not implement
	CCAssert(0, "");
	return NULL;
}

LHSprite* LHSprite::spriteWithFile(const char *pszFileName)
{
    LHSprite *pobSprite = new LHSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
	return NULL;
}

LHSprite* LHSprite::spriteWithFile(const char *pszFileName, const CCRect& rect)
{
    LHSprite *pobSprite = new LHSprite();
	if (pobSprite && pobSprite->initWithFile(pszFileName, rect))
    {
	    pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
	return NULL;
}

LHSprite* LHSprite::spriteWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    LHSprite *pobSprite = new LHSprite();
    if (pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
	    pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
	return NULL;
}

LHSprite* LHSprite::spriteWithSpriteFrameName(const char *pszSpriteFrameName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
    
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
    CCAssert(pFrame != NULL, msg);
	return spriteWithSpriteFrame(pFrame);
}

LHSprite* LHSprite::spriteWithBatchNode(CCSpriteBatchNode *batchNode, const CCRect& rect)
{
	LHSprite *pobSprite = new LHSprite();
	if (pobSprite && pobSprite->initWithBatchNode(batchNode, rect))
	{
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool LHSprite::isTouchedAtPoint(CCPoint point){
    
    if(body == NULL)
    {
        float x = point.x;
        float y = point.y;
        
        float ax = m_sQuad.tl.vertices.x;
        float ay = m_sQuad.tl.vertices.y;
        
        float bx = m_sQuad.tr.vertices.x;
        float by = m_sQuad.tr.vertices.y;
        
        float dx = m_sQuad.bl.vertices.x;
        float dy = m_sQuad.bl.vertices.y;
        
        float bax=bx-ax;
        float bay=by-ay;
        float dax=dx-ax;
        float day=dy-ay;
        
        if ((x-ax)*bax+(y-ay)*bay<0.0) return false;
        if ((x-bx)*bax+(y-by)*bay>0.0) return false;
        if ((x-ax)*dax+(y-ay)*day<0.0) return false;
        if ((x-dx)*dax+(y-dy)*day>0.0) return false;
        
        return true;
        
    }
    else{
        b2Fixture* stFix = body->GetFixtureList();
        while(stFix != 0){
            if(stFix->TestPoint(b2Vec2(point.x/LHSettings::sharedInstance()->lhPtmRatio(), 
                                       point.y/LHSettings::sharedInstance()->lhPtmRatio()))){
                return true;
            }
            stFix = stFix->GetNext();
        }
    }
    return false;    
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void LHSprite::registerTouchBeginObserver(CCObject* observer, SEL_CallFuncO selector){
        
        touchBeginObserver.object = observer;
        touchBeginObserver.selector = selector;
}
//------------------------------------------------------------------------------
void LHSprite::registerTouchMovedObserver(CCObject* observer, SEL_CallFuncO selector){
    touchMovedObserver.object = observer;
    touchMovedObserver.selector = selector;
}
//------------------------------------------------------------------------------
void LHSprite::registerTouchEndedObserver(CCObject* observer, SEL_CallFuncO selector){
    touchEndedObserver.object = observer;
    touchEndedObserver.selector = selector;    
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool LHSprite::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) 
{    
    CCPoint touchPoint =     pTouch->locationInView();
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
    
    if(isTouchedAtPoint(touchPoint))
    {
        LHTouchInfo* info = LHTouchInfo::touchInfo();
        info->relativePoint = CCPointMake(touchPoint.x - getPosition().x,
                                          touchPoint.y - getPosition().y);
        info->glPoint = touchPoint;
        info->event = pEvent;
        info->touch = pTouch;
        info->sprite = this;
        info->delta = CCPointZero;
        
        if(touchBeginObserver.object){
            (touchBeginObserver.object->*touchBeginObserver.selector)(info);
        }        
        
        if(tagTouchBeginObserver && tagTouchBeginObserver->object){
            (tagTouchBeginObserver->object->*tagTouchBeginObserver->selector)(info);
        }
        return swallowTouches;
    }
    return true;
}
//------------------------------------------------------------------------------
void LHSprite::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
        
    CCPoint touchPoint =     pTouch->locationInView();
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
    
    CCPoint prevLocation = pTouch->previousLocationInView();
    prevLocation =  CCDirector::sharedDirector()->convertToGL(prevLocation);
    
    LHTouchInfo* info = LHTouchInfo::touchInfo();
    info->relativePoint = CCPointMake(touchPoint.x - getPosition().x,
                                     touchPoint.y - getPosition().y);
    info->glPoint = touchPoint;
    info->event = pEvent;
    info->touch = pTouch;
    info->sprite = this;
    info->delta = CCPointMake(touchPoint.x - prevLocation.x,
                              touchPoint.y - prevLocation.y);
    
    
    if(touchMovedObserver.object){
        (touchMovedObserver.object->*touchMovedObserver.selector)(info);
    }  
    
    if(tagTouchMovedObserver && tagTouchMovedObserver->object){
        (tagTouchMovedObserver->object->*tagTouchMovedObserver->selector)(info);
    }
}
//------------------------------------------------------------------------------
void LHSprite::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
    CCPoint touchPoint =     pTouch->locationInView();
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
    
    CCPoint prevLocation = pTouch->previousLocationInView();
    prevLocation =  CCDirector::sharedDirector()->convertToGL(prevLocation);
    
    LHTouchInfo* info = LHTouchInfo::touchInfo();
    info->relativePoint = CCPointMake(touchPoint.x - getPosition().x,
                                      touchPoint.y - getPosition().y);
    info->glPoint = touchPoint;
    info->event = pEvent;
    info->touch = pTouch;
    info->sprite = this;
    info->delta = CCPointMake(touchPoint.x - prevLocation.x,
                              touchPoint.y - prevLocation.y);
    
    if(touchEndedObserver.object){
        (touchEndedObserver.object->*touchEndedObserver.selector)(info);
    }
    if(tagTouchEndedObserver && tagTouchEndedObserver->object){
        (tagTouchEndedObserver->object->*tagTouchEndedObserver->selector)(info);
    }
}
//------------------------------------------------------------------------------
void LHSprite::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){

}

void LHSprite::setTagTouchBeginObserver(LHObserverPair* pair){
    tagTouchBeginObserver = pair;
}
void LHSprite::setTagTouchMovedObserver(LHObserverPair* pair){
    tagTouchMovedObserver = pair;
}
void LHSprite::setTagTouchEndedObserver(LHObserverPair* pair){
    tagTouchEndedObserver = pair;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::string LHSprite::uniqueNameForBody(b2Body* body){
    
    CCNode* spr = (CCNode*)body->GetUserData();
    
    if(LHSprite::isLHSprite(spr))
        return ((LHSprite*)spr)->getUniqueName();
    
    if(LHBezierNode::isLHBezierNode(spr))
        return ((LHBezierNode*)spr)->getUniqueName();
    
    return NULL;
}
//------------------------------------------------------------------------------
LHSprite* LHSprite::spriteForBody(b2Body* body){
    
    if(0 == body)
        return 0;
    
    CCNode* spr = (CCNode*)body->GetUserData();
    
    if(LHSprite::isLHSprite(spr))
    {
        return (LHSprite*)spr;
    }
    
    return 0;
}
//------------------------------------------------------------------------------
int LHSprite::tagForBody(b2Body* body){        
    if(0 != body)
    {
        CCNode* spr = (CCNode*)body->GetUserData();
        if(NULL != spr)
        {
            return spr->getTag();
        }
    }
    return -1;
}
//------------------------------------------------------------------------------
bool LHSprite::isLHSprite(CCNode* obj){
    
    if( 0 != dynamic_cast<LHSprite*>(obj))
        return true;
    
    return false;
}

