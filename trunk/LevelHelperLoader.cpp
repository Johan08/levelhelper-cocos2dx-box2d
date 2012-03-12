//  http://www.levelhelper.org
//
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
//  You do not have permission to use this code or any part of it if you don't
//  own a license to LevelHelper application.

#include "LevelHelperLoader.h"

#include "LHSettings.h"
#include "LHTouchMgr.h"

int intFromString(const std::string& str)
{
    return atoi(str.c_str());
}
bool boolFromString(const std::string& str)
{
    return (bool)atoi(str.c_str());
}
float floatFromString(const std::string& str)
{
    return (float)atof(str.c_str());
}

std::string stringFromInt(const int& i)
{
    std::stringstream st;
    st << i;
    return st.str();    
}

CCPoint LHPointFromString(const std::string& str)
{
    return CCPointFromString(str.c_str());
}

CCRect LHRectFromString(const std::string& str)
{
    return CCRectFromString(str.c_str());
}

////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::initObjects(void)
{
	lhBatchInfo = new LHDictionary();
    
    contactNode = NULL;
    wb = NULL;
    
	addSpritesToLayerWasUsed = false;
	addObjectsToWordWasUsed = false;
    
    pathNotifierId = NULL;
    animNotifierId = NULL;
    
	loadingProgressId= NULL;
	loadingProgressSel= NULL;

    cocosLayer  = NULL;
    box2dWorld = NULL;
    
    LHSettings::sharedInstance()->setLhPtmRatio(32.0f);
	
    notifOnLoopForeverAnim = false;
}
LevelHelperLoader::LevelHelperLoader(const char* levelFile){

    CCAssert(NULL!=levelFile, "Invalid file given to LevelHelperLoader");
	
    initObjects();
    loadLevelHelperSceneFile(levelFile, "", "");
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void LevelHelperLoader::addSpritesToLayer(CCLayer* _cocosLayer)
{	
    
    CCLog("Method addSpritesToLayer is not yet implemented. Please use addObjectsToWorld with all sprites set to NO PHYSICS");
    /*
	CCAssert(addObjectsToWordWasUsed!=true, "You can't use method addSpritesToLayer because you already used addObjectToWorld. Only one of the two can be used."); 
	CCAssert(addSpritesToLayerWasUsed!=true, "You can't use method addSpritesToLayer again. You can only use it once. Create a new LevelHelperLoader object if you want to load the level again."); 
	
	addSpritesToLayerWasUsed = true;
	
	cocosLayer = _cocosLayer;
	
    addBatchNodesToLayer(cocosLayer);
	
    createAllAnimationsInfo();
    
    //we need to first create the path so we can assign the path to sprite on creation
    //    for(NSDictionary* bezierDict in lhBeziers)
    //    {
    //        //NSString* uniqueName = [bezierDict objectForKey:@"UniqueName"];
    //        if([[bezierDict objectForKey:@"IsPath"] boolValue])
    //        {
    //            [self createBezierPath:bezierDict];
    //        }
    //    }
    
    
	for(NSDictionary* dictionary in lhSprites)
	{
		NSDictionary* spriteProp = [dictionary objectForKey:@"GeneralProperties"];
		
		//find the coresponding batch node for this sprite
        //LHBatch* bNode = [batchNodesInLevel objectForKey:[spriteProp objectForKey:@"Image"]];
		//CCSpriteBatchNode *batch = [bNode spriteBatchNode];
		
        LHBatch* bNode = [self batchNodeForFile:[spriteProp objectForKey:@"Image"]];
        
        if(bNode)
        {
            CCSpriteBatchNode *batch = [bNode spriteBatchNode];
            if(nil != batch)
            {
                LHSprite* ccsprite = [self spriteWithBatchFromDictionary:spriteProp batchNode:bNode];
                if(nil != ccsprite)
                {
                    [batch addChild:ccsprite];
                    [spritesInLevel setObject:ccsprite forKey:[spriteProp objectForKey:@"UniqueName"]];
                    
                    [self setCustomAttributesForNonPhysics:spriteProp
                                                    sprite:ccsprite];
                }
                
                if(![[spriteProp objectForKey:@"PathName"] isEqualToString:@"None"])
                {
                    //we have a path we need to follow
                    [self createPathOnSprite:ccsprite
                              withProperties:spriteProp];
                }
                
                [self createAnimationFromDictionary:spriteProp onCCSprite:ccsprite];
            }
        }
	}
    
    for(NSDictionary* parallaxDict in lhParallax)
    {
        //NSMutableDictionary* nodeInfo = [[[NSMutableDictionary alloc] init] autorelease];
        //       CCNode* node = [self parallaxNodeFromDictionary:parallaxDict layer:cocosLayer];
        
        //   if(nil != node)
        // {
        //[nodeInfo setObject:[parallaxDict objectForKey:@"ContinuousScrolling"] forKey:@"ContinuousScrolling"];
        //[//nodeInfo setObject:[parallaxDict objectForKey:@"Speed"] forKey:@"Speed"];
        //[nodeInfo setObject:[parallaxDict objectForKey:@"Direction"] forKey:@"Direction"];
        //[nodeInfo setObject:node forKey:@"Node"];
        //         [ccParallaxInScene setObject:node forKey:[parallaxDict objectForKey:@"UniqueName"]];
        //}
    }
     */
}
////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::addObjectsToWorld(b2World* world, CCLayer* _cocosLayer)
{
	CCAssert(addSpritesToLayerWasUsed!=true, "You can't use method addObjectsToWorld because you already used addSpritesToLayer. Only one of the two can be used."); 
	CCAssert(addObjectsToWordWasUsed!=true, "You can't use method addObjectsToWorld again. You can only use it once. Create a new LevelHelperLoader object if you want to load the level again."); 
	
	addObjectsToWordWasUsed = true;
	
	cocosLayer = _cocosLayer;
    box2dWorld = world;
	
    //order is important
    addBatchNodesToLayer(cocosLayer);
    callLoadingProgressObserverWithValue(0.20f);
    createAllAnimationsInfo();
    callLoadingProgressObserverWithValue(0.30f);
    createAllBeziers();
    callLoadingProgressObserverWithValue(0.50f);
    createSpritesWithPhysics();
    createParallaxes();
    callLoadingProgressObserverWithValue(0.90f);
    createJoints();
    callLoadingProgressObserverWithValue(1.00f);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void LevelHelperLoader::registerLoadingProgressObserver(CCObject* loadingProgressObj, SEL_CallFuncFloat sel){
    loadingProgressId = loadingProgressObj;
    loadingProgressSel = sel;
}
void LevelHelperLoader::callLoadingProgressObserverWithValue(float val)
{
    if(NULL != loadingProgressId && NULL != loadingProgressSel){         
		(loadingProgressId->*loadingProgressSel)(val);
    }
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool LevelHelperLoader::isPaused(void){
    return LHSettings::sharedInstance()->levelPaused();
}
//------------------------------------------------------------------------------
void LevelHelperLoader::setPaused(bool value){
    LHSettings::sharedInstance()->setLevelPaused(value);    
}
//------------------------------------------------------------------------------
void LevelHelperLoader::dontStretchArtOnIpad(){
    LHSettings::sharedInstance()->setStretchArt(false);
}
void LevelHelperLoader::preloadBatchNodes(){
    LHSettings::sharedInstance()->setPreloadBatchNodes(true);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void LevelHelperLoader::useLevelHelperCollisionHandling(void)
{
    if(0 == box2dWorld){
        CCLog("LevelHelper WARNING: Please call useLevelHelperCollisionHandling after addObjectsToWorld");
        return;
    }
    
    contactNode = LHContactNode::contactNodeWithWorld(box2dWorld);    
    if(contactNode)
        cocosLayer->addChild(contactNode);
}
//------------------------------------------------------------------------------
void LevelHelperLoader::registerBeginOrEndCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                                       enum LevelHelper_TAG tagB,
                                                                       CCObject* obj,
                                                                       SEL_CallFuncO selector){
    if(NULL == contactNode){
        CCLog("LevelHelper WARNING: Please call registerBeginOrEndCollisionCallbackBetweenTagA after useLevelHelperCollisionHandling");
        return;
    }
    contactNode->registerBeginOrEndCollisionCallbackBetweenTagA(tagA, tagB, obj, selector);

}
//------------------------------------------------------------------------------
void LevelHelperLoader::cancelBeginOrEndCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                                     enum LevelHelper_TAG tagB){
    if(NULL == contactNode){
        CCLog("LevelHelper WARNING: Please call cancelBeginOrEndCollisionCallbackBetweenTagA after useLevelHelperCollisionHandling");
        return;
    }

    contactNode->cancelBeginOrEndCollisionCallbackBetweenTagA((int)tagA, (int)tagB);
}
//------------------------------------------------------------------------------
void LevelHelperLoader::registerPreCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                               enum LevelHelper_TAG tagB,
                                                               CCObject* obj,
                                                               SEL_CallFuncO selector)
{
    if(NULL == contactNode){
        CCLog("LevelHelper WARNING: Please call registerPreColisionCallbackBetweenTagA after useLevelHelperCollisionHandling");
        return;
    }
    contactNode->registerPreCollisionCallbackBetweenTagA(tagA, tagB, obj, selector);
}
//------------------------------------------------------------------------------
void LevelHelperLoader::cancelPreCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                              enum LevelHelper_TAG tagB)
{
    if(NULL == contactNode){
        CCLog("LevelHelper WARNING: Please call registerPreColisionCallbackBetweenTagA after useLevelHelperCollisionHandling");
        return;
    }
    contactNode->cancelPreCollisionCallbackBetweenTagA((int)tagA, (int)tagB);
}
//------------------------------------------------------------------------------
void LevelHelperLoader::registerPostCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                                enum LevelHelper_TAG tagB,
                                                                CCObject* obj,
                                                                SEL_CallFuncO selector)
{
    if(NULL == contactNode){
        CCLog("LevelHelper WARNING: Please call registerPostColisionCallbackBetweenTagA after useLevelHelperCollisionHandling");
        return;
    }
    contactNode->registerPreCollisionCallbackBetweenTagA(tagA, tagB, obj, selector);    
}
//------------------------------------------------------------------------------
void LevelHelperLoader::cancelPostCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                               enum LevelHelper_TAG tagB)
{
    if(NULL == contactNode){
        CCLog("LevelHelper WARNING: Please call registerPreColisionCallbackBetweenTagA after useLevelHelperCollisionHandling");
        return;
    }
    contactNode->cancelPostCollisionCallbackBetweenTagA((int)tagA,(int)tagB);
}
//------------------------------------------------------------------------------
void LevelHelperLoader::markSpriteForRemoval(LHSprite* ccsprite){
    
    if(NULL == ccsprite)
        return;
    
    markedSprites.setObject(ccsprite, ccsprite->getUniqueName());
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removeMarkedSprites(){
    
    std::vector<std::string> keys = markedSprites.allKeys();

    for(size_t i = 0; i < keys.size(); ++i){
        LHSprite* spr = (LHSprite*)markedSprites.objectForKey(keys[i]);
        removeSprite(spr);
    }
    markedSprites.removeAllObjects();
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CCSize LevelHelperLoader::gameScreenSize(void){
    return CCSizeMake(safeFrame.x, safeFrame.y);
}
//------------------------------------------------------------------------------
CCRect LevelHelperLoader::gameWorldSize(void)
{
    CCPoint  wbConv = LHSettings::sharedInstance()->convertRatio();
	
    CCRect ws = gameWorldRect;
    
    ws.origin.x *= wbConv.x;
    ws.origin.y *= wbConv.y;
    ws.size.width *= wbConv.x;
    ws.size.height *= wbConv.y;
    
    return ws;
}
//------------------------------------------------------------------------------
unsigned int LevelHelperLoader::numberOfBatchNodesUsed(void)
{
	return (int)batchNodesInLevel.count() -1;
}
////////////////////////////////////////////////////////////////////////////////

LevelHelperLoader::~LevelHelperLoader()
{    
    releasePhysicBoundaries();
    removeAllBezierNodes();
    releaseAllJoints();
    releaseAllSprites();
    removeAllParallaxes();
    releaseAllBatchNodes();
    
    delete lhSprites;
    delete lhJoints;
    delete lhParallax;
    delete lhBeziers;
    delete lhAnims;
    delete lhBatchInfo;
    
    delete wb;
    
    if(NULL != contactNode){
        contactNode->removeFromParentAndCleanup(true);
    }
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////PRIVATE METHODS//////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//ANIMATIONS
////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::createAllAnimationsInfo(void)
{
    for(int i = 0; i< lhAnims->count(); ++i)
    {
        LHDictionary* animInfo = lhAnims->objectAtIndex(i)->dictValue();
        std::string uniqueAnimName = animInfo->objectForKey("UniqueName")->stringValue();
        
        LHArray* framesInfo = animInfo->objectForKey("Frames")->arrayValue();
        
        bool loop           = animInfo->objectForKey("LoopForever")->boolValue();
        float animSpeed     = animInfo->objectForKey("Speed")->floatValue();
        int repetitions     = animInfo->objectForKey("Repetitions")->intValue();
        bool startAtLaunch  = animInfo->objectForKey("StartAtLaunch")->boolValue();
        
        std::string image = animInfo->objectForKey("Image")->stringValue();
        
        LHAnimationNode* animNode = LHAnimationNode::animationNodeWithUniqueName(uniqueAnimName.c_str());
        animNode->loop = loop;
        animNode->speed =animSpeed;
        animNode->repetitions = repetitions;
        animNode->startAtLaunch = startAtLaunch;
        animNode->setImageName(image.c_str());
        
        std::vector<CCRect> frmsInfo;
        for(int j = 0; j < framesInfo->count(); ++j)
        {
            LHDictionary* rectDict = framesInfo->objectAtIndex(j)->dictValue();
            CCRect rect = CCRectFromString(rectDict->objectForKey("FrameRect")->stringValue().c_str());
            frmsInfo.push_back(rect);
        }
        animNode->setFramesInfo(frmsInfo);
        animationsInLevel.setObject(animNode, uniqueAnimName);
    }
}

//------------------------------------------------------------------------------
void LevelHelperLoader::createAnimationFromDictionary(LHDictionary* spriteProp,
                                                      LHSprite* ccsprite)
{
    std::string animName = spriteProp->objectForKey("AnimName")->stringValue();
	if(animName != "")
	{
        LHAnimationNode* animNode = (LHAnimationNode*)animationsInLevel.objectForKey(animName);
        if(NULL != animNode)
        {
            if(animNode->startAtLaunch)
            {
                LHBatch* batch = batchNodeForFile(animNode->getImageName());
                
                if(batch)
                {
                    animNode->setBatchNode(batch->getSpriteBatchNode());
                    animNode->computeFrames();
     
                    ccsprite->startAnimationNamed(animName, 0, animNotifierId, animNotifierSel, notifOnLoopForeverAnim);
                }
            }
            else
            {
                ccsprite->prepareAnimationNamed(animName);
            }
        }
	}
}

//------------------------------------------------------------------------------
void LevelHelperLoader::startAnimationWithUniqueName(const std::string& animName,
                                                     LHSprite* ccsprite,
                                                     CCObject* customAnimNotifierId,
                                                     SEL_CallFuncND customAnimNotifierSel)
{    
    
    CCLog("startAnimationWithUniqueName is deprecated. Please use the method provide in LHSprite class");
    ccsprite->startAnimationNamed(animName, 0, customAnimNotifierId, customAnimNotifierSel, notifOnLoopForeverAnim);    
}

//------------------------------------------------------------------------------
void LevelHelperLoader::stopAnimationOnSprite(LHSprite* ccsprite)
{
    if(NULL != ccsprite){
        ccsprite->stopActionByTag(LH_ANIM_ACTION_TAG);
        ccsprite->setAnimation(NULL);
    }    
}
//------------------------------------------------------------------------------
void LevelHelperLoader::prepareAnimationWithUniqueName(const std::string& animName,
                                                       LHSprite* sprite)
{
    LHAnimationNode* animNode = (LHAnimationNode*)animationsInLevel.objectForKey(animName);
    if(animNode == NULL)
        return;
    
    LHBatch* batch = batchNodeForFile(animNode->getImageName());
    
    if(batch)
    {
        animNode->setBatchNode(batch->getSpriteBatchNode());
        animNode->computeFrames();
        sprite->setAnimation(animNode);
    }
}

//------------------------------------------------------------------------------
void LevelHelperLoader::registerNotifierOnAllAnimationEnds(CCObject* obj, SEL_CallFuncND sel)
{
    animNotifierId = obj;
    animNotifierSel = sel;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::enableNotifOnLoopForeverAnimations(void)
{
    notifOnLoopForeverAnim = true;
}
////////////////////////////////////////////////////////////////////////////////
//GRAVITY
////////////////////////////////////////////////////////////////////////////////
bool LevelHelperLoader::isGravityZero(void){
    if(gravity.x == 0 && gravity.y == 0)
        return true;
    return false;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createGravity(b2World* world)
{
	if(isGravityZero())
		CCLog("LevelHelper Warning: Gravity is not defined in the level. Are you sure you want to set a zero gravity?");
    world->SetGravity(b2Vec2(gravity.x, gravity.y));
}
////////////////////////////////////////////////////////////////////////////////
//PHYSIC BOUNDARIES
////////////////////////////////////////////////////////////////////////////////
b2Body* LevelHelperLoader::physicBoundarieForKey(const std::string& key)
{
    LHSprite* spr = (LHSprite*)physicBoundariesInLevel.objectForKey(key);
    if(NULL == spr)
        return 0;
    return spr->getBody();
}
//------------------------------------------------------------------------------
b2Body* LevelHelperLoader::leftPhysicBoundary(void){
    return physicBoundarieForKey("LHPhysicBoundarieLeft");
}
LHSprite* LevelHelperLoader::leftPhysicBoundarySprite(void){
    return (LHSprite*)physicBoundariesInLevel.objectForKey("LHPhysicBoundarieLeft");
}
//------------------------------------------------------------------------------
b2Body* LevelHelperLoader::rightPhysicBoundary(void){
	return physicBoundarieForKey("LHPhysicBoundarieRight");
}
LHSprite* LevelHelperLoader::rightPhysicBoundarySprite(void){
    return (LHSprite*)physicBoundariesInLevel.objectForKey("LHPhysicBoundarieRight");
}
//------------------------------------------------------------------------------
b2Body* LevelHelperLoader::topPhysicBoundary(void){
    return physicBoundarieForKey("LHPhysicBoundarieTop");
}
LHSprite* LevelHelperLoader::topPhysicBoundarySprite(void){
    return (LHSprite*)physicBoundariesInLevel.objectForKey("LHPhysicBoundarieTop");
}
//------------------------------------------------------------------------------
b2Body* LevelHelperLoader::bottomPhysicBoundary(void){
    return physicBoundarieForKey("LHPhysicBoundarieBottom");
}
LHSprite* LevelHelperLoader::bottomPhysicBoundarySprite(void){
    return (LHSprite*)physicBoundariesInLevel.objectForKey("LHPhysicBoundarieBottom");
}
//------------------------------------------------------------------------------
bool LevelHelperLoader::hasPhysicBoundaries(void){
	if(wb == NULL){
		return false;
	}
    CCRect rect = LHRectFromString(wb->objectForKey("WBRect")->stringValue());    
    if(rect.size.width == 0 || rect.size.height == 0)
        return false;
	return true;
}
//------------------------------------------------------------------------------
CCRect LevelHelperLoader::physicBoundariesRect(void)
{
    CCPoint  wbConv = LHSettings::sharedInstance()->convertRatio();
    CCRect rect = LHRectFromString(wb->objectForKey("WBRect")->stringValue());    
    rect.origin.x = rect.origin.x*wbConv.x,
    rect.origin.y = rect.origin.y*wbConv.y;
    rect.size.width = rect.size.width*wbConv.x;
    rect.size.height= rect.size.height*wbConv.y;
    return rect;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createPhysicBoundariesNoStretching(b2World * _world){
    
    CCPoint pos_offset = LHSettings::sharedInstance()->possitionOffset();
    CCPoint  wbConv = LHSettings::sharedInstance()->convertRatio();
    
    createPhysicBoundariesHelper(_world, 
                                 wbConv, 
                                 CCPointMake(pos_offset.x/2.0f, 
                                            pos_offset.y/2.0f));
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createPhysicBoundaries(b2World* _world)
{
    CCPoint  wbConv = LHSettings::sharedInstance()->realConvertRatio();
    createPhysicBoundariesHelper(_world,
                                 wbConv,
                                 CCPointMake(0.0f, 0.0f));
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createPhysicBoundariesHelper(b2World* _world,
                                                     const CCPoint& wbConv,
                                                     const CCPoint& pos_offset)
{
	if(!hasPhysicBoundaries()){
        CCLog("LevelHelper WARNING - Please create physic boundaries in LevelHelper in order to call method \"createPhysicBoundaries\"");
        return;
    }	
    
    b2BodyDef bodyDef;		
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(0.0f, 0.0f);
    b2Body* wbBodyT = _world->CreateBody(&bodyDef);
	b2Body* wbBodyL = _world->CreateBody(&bodyDef);
	b2Body* wbBodyB = _world->CreateBody(&bodyDef);
	b2Body* wbBodyR = _world->CreateBody(&bodyDef);
	
	{
        LHSprite* spr = LHSprite::sprite();
		spr->setTag(wb->objectForKey("TagLeft")->intValue()); 
		spr->setIsVisible(false);
		spr->setUniqueName("LHPhysicBoundarieLeft");
        spr->setBody(wbBodyL);    
        wbBodyL->SetUserData(spr);
        physicBoundariesInLevel.setObject(spr, "LHPhysicBoundarieLeft");
	}
	
	{
        LHSprite* spr = LHSprite::sprite();
		spr->setTag(wb->objectForKey("TagRight")->intValue()); 
		spr->setIsVisible(false);
		spr->setUniqueName("LHPhysicBoundarieRight");
        spr->setBody(wbBodyR);  
        wbBodyR->SetUserData(spr);
        physicBoundariesInLevel.setObject(spr,"LHPhysicBoundarieRight");
	}
	
	{
        LHSprite* spr = LHSprite::sprite();
		spr->setTag(wb->objectForKey("TagTop")->intValue()); 
		spr->setIsVisible(false);
		spr->setUniqueName("LHPhysicBoundarieTop");
        spr->setBody(wbBodyT);  
        wbBodyT->SetUserData(spr);        
        physicBoundariesInLevel.setObject(spr,"LHPhysicBoundarieTop");
	}
	
	{
        LHSprite* spr = LHSprite::sprite();
		spr->setTag(wb->objectForKey("TagBottom")->intValue()); 
		spr->setIsVisible(false);
		spr->setUniqueName("LHPhysicBoundarieBottom");
        spr->setBody(wbBodyB);  
        wbBodyB->SetUserData(spr);        
        physicBoundariesInLevel.setObject(spr, "LHPhysicBoundarieBottom");
	}
	
	wbBodyT->SetSleepingAllowed(wb->objectForKey("CanSleep")->boolValue());  
	wbBodyL->SetSleepingAllowed(wb->objectForKey("CanSleep")->boolValue());  
	wbBodyB->SetSleepingAllowed(wb->objectForKey("CanSleep")->boolValue());  
	wbBodyR->SetSleepingAllowed(wb->objectForKey("CanSleep")->boolValue());  
	
	
    CCRect rect = LHRectFromString(wb->objectForKey("WBRect")->stringValue());    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	
    
#ifndef LH_SCENE_TESTER
    rect.origin.x += pos_offset.x;
    rect.origin.y += pos_offset.y;
#else
    rect.origin.x += pos_offset.x*2.0f;
    rect.origin.y += pos_offset.y*2.0f;
#endif
    
    float ptm = LHSettings::sharedInstance()->lhPtmRatio();
    
    {//TOP
        b2EdgeShape shape;
		
        b2Vec2 pos1 = b2Vec2(rect.origin.x/ptm*wbConv.x,
							 (winSize.height - rect.origin.y*wbConv.y)/ptm);
        
        b2Vec2 pos2 = b2Vec2((rect.origin.x + rect.size.width)*wbConv.x/ptm, 
							 (winSize.height - rect.origin.y*wbConv.y)/ptm);		
		shape.Set(pos1, pos2);
		
        b2FixtureDef fixture;
        setFixtureDefPropertiesFromDictionary(wb, &fixture);
        fixture.shape = &shape;
        wbBodyT->CreateFixture(&fixture);
    }
	
    {//LEFT
        b2EdgeShape shape;
		
		b2Vec2 pos1 = b2Vec2(rect.origin.x*wbConv.x/ptm,
							 (winSize.height - rect.origin.y*wbConv.y)/ptm);
        
		b2Vec2 pos2 = b2Vec2((rect.origin.x*wbConv.x)/ptm, 
							 (winSize.height - (rect.origin.y + rect.size.height)*wbConv.y)/ptm);
        shape.Set(pos1, pos2);
		
        b2FixtureDef fixture;
        setFixtureDefPropertiesFromDictionary(wb, &fixture);
        fixture.shape = &shape;
        wbBodyL->CreateFixture(&fixture);
    }
	
    {//RIGHT
        b2EdgeShape shape;
        
        b2Vec2 pos1 = b2Vec2((rect.origin.x + rect.size.width)*wbConv.x/ptm,
							 (winSize.height - rect.origin.y*wbConv.y)/ptm);
        
        b2Vec2 pos2 = b2Vec2((rect.origin.x+ rect.size.width)*wbConv.x/ptm, 
							 (winSize.height - (rect.origin.y + rect.size.height)*wbConv.y)/ptm);
        shape.Set(pos1, pos2);
		
        b2FixtureDef fixture;
        setFixtureDefPropertiesFromDictionary(wb, &fixture);
        fixture.shape = &shape;
        wbBodyR->CreateFixture(&fixture);
    }
	
    {//BOTTOM
        b2EdgeShape shape;
        
        b2Vec2 pos1 = b2Vec2(rect.origin.x*wbConv.x/ptm,
							 (winSize.height - (rect.origin.y + rect.size.height)*wbConv.y)/ptm);
        
        b2Vec2 pos2 = b2Vec2((rect.origin.x+ rect.size.width)*wbConv.x/ptm, 
							 (winSize.height - (rect.origin.y + rect.size.height)*wbConv.y)/ptm);
        shape.Set(pos1, pos2);
		
        b2FixtureDef fixture;
        setFixtureDefPropertiesFromDictionary(wb, &fixture);
        fixture.shape = &shape;
        wbBodyB->CreateFixture(&fixture);
    }
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removePhysicBoundaries()
{
    physicBoundariesInLevel.removeAllObjects();
}
//------------------------------------------------------------------------------
void LevelHelperLoader::releasePhysicBoundaries(void)
{
    removePhysicBoundaries();
}
////////////////////////////////////////////////////////////////////////////////
//PHYSICS
////////////////////////////////////////////////////////////////////////////////

void LevelHelperLoader::setMeterRatio(float ratio){
	LHSettings::sharedInstance()->setLhPtmRatio(ratio);
}
//------------------------------------------------------------------------------
float LevelHelperLoader::meterRatio(){
	return LHSettings::sharedInstance()->lhPtmRatio();
}
//------------------------------------------------------------------------------
float LevelHelperLoader::pixelsToMeterRatio(){
    return LHSettings::sharedInstance()->lhPtmRatio()*LHSettings::sharedInstance()->convertRatio().x;
}
//------------------------------------------------------------------------------
float LevelHelperLoader::pointsToMeterRatio(){
    return LHSettings::sharedInstance()->lhPtmRatio();
}
//------------------------------------------------------------------------------
b2Vec2 LevelHelperLoader::pixelToMeters(CCPoint point){
    return b2Vec2(point.x / LevelHelperLoader::pixelsToMeterRatio(), 
                  point.y / LevelHelperLoader:: pixelsToMeterRatio());
}
//------------------------------------------------------------------------------
b2Vec2 LevelHelperLoader::pointsToMeters(CCPoint point){
    return b2Vec2(point.x / LHSettings::sharedInstance()->lhPtmRatio(), 
                  point.y / LHSettings::sharedInstance()->lhPtmRatio());
}
//------------------------------------------------------------------------------
CCPoint LevelHelperLoader::metersToPoints(b2Vec2 vec){
    return CCPointMake(vec.x*LHSettings::sharedInstance()->lhPtmRatio(), 
                       vec.y*LHSettings::sharedInstance()->lhPtmRatio());
}
//------------------------------------------------------------------------------
CCPoint LevelHelperLoader::metersToPixels(b2Vec2 vec){
    return ccpMult(CCPointMake(vec.x, vec.y), LevelHelperLoader::pixelsToMeterRatio());
}

////////////////////////////////////////////////////////////////////////////////
//BEZIERS
////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::createAllBeziers(void)
{
    for(int i = 0; i< lhBeziers->count(); ++i)
    {
        LHDictionary* bezierDict = lhBeziers->objectAtIndex(i)->dictValue();

		LHBezierNode* node = LHBezierNode::nodeWithDictionary(bezierDict,
                                                              cocosLayer,
                                                              box2dWorld);
		
        std::string uniqueName = bezierDict->objectForKey("UniqueName")->stringValue();
		if(NULL != node){
			beziersInLevel.setObject(node, uniqueName);
            int tag = bezierDict->objectForKey("Tag")->intValue();
            setTouchDispatcherForBezierWithTag(node, tag);
			int z = bezierDict->objectForKey("ZOrder")->intValue();
			cocosLayer->addChild(node, z);
		}		
    }
}
//------------------------------------------------------------------------------
LHBezierNode* LevelHelperLoader::bezierNodeWithUniqueName(const std::string& name){
	return (LHBezierNode*)beziersInLevel.objectForKey(name);
}
//------------------------------------------------------------------------------
CCArray* LevelHelperLoader::bezierNodesWithTag(enum LevelHelper_TAG tag){
    std::vector<std::string> keys = beziersInLevel.allKeys();
    CCArray* array = CCArray::array();
    for(size_t i = 0; i < keys.size(); ++i){        
        LHBezierNode* node = (LHBezierNode*)beziersInLevel.objectForKey(keys[i]);
        if(node && node->getTag() == (int)tag){
            array->addObject(node);
        }
	}
    return array;
}
//------------------------------------------------------------------------------
CCArray* LevelHelperLoader::allBeziers(void){
    
    std::vector<std::string> keys = beziersInLevel.allKeys();
    CCArray* array = CCArray::array();
    for(size_t i = 0; i < keys.size(); ++i){        
        LHBezierNode* node = (LHBezierNode*)beziersInLevel.objectForKey(keys[i]);
        if(node){
            array->addObject(node);
        }
	}
    return array;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removeBezier(LHBezierNode* node){

    if(NULL != node){
        removeTouchDispatcherFromBezier(node);
        beziersInLevel.removeObjectForKey(node->getUniqueName());
        markedBeziers.removeObjectForKey(node->getUniqueName());
        node->removeFromParentAndCleanup(true);
    }
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removeAllBezierNodes(void)
{
    std::vector<std::string> keys = beziersInLevel.allKeys();
    
    for(size_t i = 0; i < keys.size(); ++i){
        std::string key = keys[i];
        LHBezierNode* node = (LHBezierNode*)beziersInLevel.objectForKey(key);
        
        removeBezier(node);
//        if(NULL != node){
//            node->removeFromParentAndCleanup(true);
//        }
    }
    beziersInLevel.removeAllObjects(); 
    markedBeziers.removeAllObjects();
}
//------------------------------------------------------------------------------
void LevelHelperLoader::markBezierForRemoval(LHBezierNode* node){
    
    if(NULL == node)
        return;
    
    markedBeziers.setObject(node, node->getUniqueName());
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removeMarkedBeziers(void){

    std::vector<std::string> keys = markedBeziers.allKeys();
    for(size_t i = 0; i< keys.size(); ++i){
        LHBezierNode* node = (LHBezierNode*)markedBeziers.objectForKey(keys[i]);
        removeBezier(node);
    }
    markedBeziers.removeAllObjects();
}
////////////////////////////////////////////////////////////////////////////////
//PATH
////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::createPathOnSprite(LHSprite* ccsprite, LHDictionary* spriteProp)
{
    if(NULL == ccsprite || NULL == spriteProp)
        return;
    
    std::string uniqueName = spriteProp->objectForKey("PathName")->stringValue();
    bool isCyclic = spriteProp->objectForKey("PathIsCyclic")->boolValue();
    float pathSpeed = spriteProp->objectForKey("PathSpeed")->floatValue();
    int startPoint =  spriteProp->objectForKey("PathStartPoint")->intValue(); //0 is first 1 is end
    bool pathOtherEnd = spriteProp->objectForKey("PathOtherEnd")->boolValue(); //false means will restart where it finishes
    int axisOrientation = spriteProp->objectForKey("PathOrientation")->intValue(); //false means will restart where it finishes
    
    bool flipX = spriteProp->objectForKey("PathFlipX")->boolValue();
    bool flipY = spriteProp->objectForKey("PathFlipY")->boolValue();
	
    ccsprite->moveOnPathWithUniqueName(uniqueName, pathSpeed, startPoint,
                                       isCyclic, pathOtherEnd, axisOrientation, 
                                       flipX, flipY, true, 
                                       pathNotifierId, pathNotifierSel);
//    moveSpriteOnPathWithUniqueName(ccsprite, 
//                                   uniqueName, 
//                                   pathSpeed, 
//                                   startPoint, 
//                                   isCyclic,
//                                   pathOtherEnd,
//                                   axisOrientation,
//                                   flipX,
//                                   flipY,
//                                   true);
}

//------------------------------------------------------------------------------
void LevelHelperLoader::moveSpriteOnPathWithUniqueName(LHSprite * ccsprite, 
                                                       const   std::string& pathUniqueName,
                                                       float   time, 
                                                       bool    startAtEndPoint,
                                                       bool    isCyclic,
                                                       bool    restartOtherEnd,
                                                       int     axis,
                                                       bool    flipx,
                                                       bool    flipy,
                                                       bool    deltaMove)
{
    if(NULL == ccsprite)
        return;
	
    CCLog("moveSpriteOnPathWithUniqueName is deprecated. Please use the method provided in LHSprite.h");
    
    ccsprite->moveOnPathWithUniqueName(pathUniqueName, 
                                       time,
                                       startAtEndPoint, 
                                       isCyclic, 
                                       restartOtherEnd, 
                                       axis, 
                                       flipx, 
                                       flipy, 
                                       deltaMove, 
                                       pathNotifierId, 
                                       pathNotifierSel);
    
//	LHBezierNode* node = bezierNodeWithUniqueName(pathUniqueName);
//	
//	if(NULL != node)
//	{
//		LHPathNode* pathNode = node->addSpriteOnPath(ccsprite,
//                                                     time,
//                                                     startAtEndPoint,
//                                                     isCyclic,
//                                                     restartOtherEnd,
//                                                     axis,
//                                                     flipx,
//                                                     flipy,
//                                                     deltaMove);
//        
//        if(NULL != pathNode)
//        {
//            pathNode->setPathNotifierObject(pathNotifierId);
//            pathNode->setPathNotifierSelector(pathNotifierSel);
//        }
//	}
}
//------------------------------------------------------------------------------
void LevelHelperLoader::registerNotifierOnAllPathEndPoints(CCObject* obj, SEL_CallFuncN sel)
{
    pathNotifierId = obj;
    pathNotifierSel = sel;
}
////////////////////////////////////////////////////////////////////////////////
//SPRITES
////////////////////////////////////////////////////////////////////////////////
LHSprite* LevelHelperLoader::spriteWithUniqueName(const std::string& name)
{
    return (LHSprite*)spritesInLevel.objectForKey(name);
}
//------------------------------------------------------------------------------
CCArray* LevelHelperLoader::spritesWithTag(enum LevelHelper_TAG tag)
{
    CCArray* array = CCArray::array();
    std::vector<std::string> keys = spritesInLevel.allKeys();
    for(size_t i = 0; i< keys.size(); ++i)
    {
		LHSprite* ccSprite = (LHSprite*)spritesInLevel.objectForKey(keys[i]);
		if(NULL != ccSprite && ccSprite->getTag() == (int)tag){
            array->addObject(ccSprite);
		}
	}
	return array;
}
//------------------------------------------------------------------------------
CCArray* LevelHelperLoader::allSprites(void){
    CCArray* array = CCArray::array();
    std::vector<std::string> keys = spritesInLevel.allKeys();
    for(size_t i = 0; i< keys.size(); ++i){
		LHSprite* ccSprite = (LHSprite*)spritesInLevel.objectForKey(keys[i]);
        if(ccSprite)
            array->addObject(ccSprite);
	}
	return array;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createSpritesWithPhysics()
{
 
    int sprCount = lhSprites->count();
    for(int i = 0; i< sprCount; ++i)
    {
        LHDictionary* dictionary = lhSprites->objectAtIndex(i)->dictValue();

		LHDictionary* spriteProp = dictionary->dictForKey("GeneralProperties");
		LHDictionary* physicProp = dictionary->dictForKey("PhysicProperties");
		
        LHBatch* bNode = batchNodeForFile(spriteProp->objectForKey("Image")->stringValue());
        
        if(NULL != bNode)
        {
            CCSpriteBatchNode *batch = bNode->getSpriteBatchNode();
            if(NULL != batch)
            {
                LHSprite* ccsprite = spriteWithBatchFromDictionary(spriteProp, bNode);
                
                if(!LHSettings::sharedInstance()->isCoronaUser()){
                    batch->addChild(ccsprite, spriteProp->objectForKey("ZOrder")->intValue());
                }
                else
                    cocosLayer->addChild(ccsprite);
                
                std::string uniqueName = spriteProp->objectForKey("UniqueName")->stringValue();
                if(physicProp->objectForKey("Type")->intValue() != 3) //3 means no physic
                {
                    b2Body* body = b2BodyFromDictionary(physicProp,spriteProp,ccsprite ,box2dWorld);
                    
                    if(0 != body)
                        ccsprite->setBody(body);
                    
                    spritesInLevel.setObject(ccsprite, uniqueName);
                }
                else {
                    spritesInLevel.setObject(ccsprite, uniqueName);

                    setCustomAttributesForNonPhysics(spriteProp,ccsprite);
                }
                
                if(//![[spriteProp objectForKey:@"IsInParallax"] boolValue] &&
                   spriteProp->objectForKey("PathName")->stringValue() != "None")
                {
                    createPathOnSprite(ccsprite,spriteProp);
                }
                
                createAnimationFromDictionary(spriteProp, ccsprite);
                ccsprite->postInit();
            }
        }
        
        //calculate progress loading
        float val = ((float)(i+1.0f)/(float)sprCount)*0.30f;
        callLoadingProgressObserverWithValue(0.50f+val);
	}
}
//------------------------------------------------------------------------------
void LevelHelperLoader::setFixtureDefPropertiesFromDictionary(LHDictionary* spritePhysic,
                                                              b2FixtureDef* shapeDef)
{
	shapeDef->density = spritePhysic->objectForKey("Density")->floatValue();
	shapeDef->friction = spritePhysic->objectForKey("Friction")->floatValue();
	shapeDef->restitution = spritePhysic->objectForKey("Restitution")->floatValue();
	
	shapeDef->filter.categoryBits = spritePhysic->objectForKey("Category")->intValue();
	shapeDef->filter.maskBits = spritePhysic->objectForKey("Mask")->intValue();
	shapeDef->filter.groupIndex = spritePhysic->objectForKey("Group")->intValue();
    
    if(NULL != spritePhysic->objectForKey("IsSensor"))
        shapeDef->isSensor = spritePhysic->objectForKey("IsSensor")->boolValue();
    
    if(NULL != spritePhysic->objectForKey("IsSenzor"))
    {//in case we load a 1.3 level
	    shapeDef->isSensor = spritePhysic->objectForKey("IsSenzor")->boolValue();
    }
}
//------------------------------------------------------------------------------
b2Body* LevelHelperLoader::b2BodyFromDictionary(LHDictionary* spritePhysic,
                                                LHDictionary* spriteProp,
                                                LHSprite* ccsprite,
                                                b2World* _world)
{
	b2BodyDef bodyDef;	
	
	int bodyType = spritePhysic->objectForKey("Type")->intValue();
	if(bodyType == 3) //in case the user wants to create a body with a sprite that has type as "NO_PHYSIC"
		bodyType = 2;
        bodyDef.type = (b2BodyType)bodyType;
        
        CCPoint pos = ccsprite->getPosition();	
        bodyDef.position.Set(pos.x/LHSettings::sharedInstance()->lhPtmRatio(),
                             pos.y/LHSettings::sharedInstance()->lhPtmRatio());
        
        bodyDef.angle = CC_DEGREES_TO_RADIANS(-1*spriteProp->objectForKey("Angle")->intValue());
        bodyDef.userData = ccsprite;
        
        b2Body* body = _world->CreateBody(&bodyDef);
        
        body->SetFixedRotation(spritePhysic->objectForKey("FixedRot")->boolValue());
        
        CCPoint linearVelocity = LHPointFromString(spritePhysic->objectForKey("LinearVelocity")->stringValue());
        
        float linearDamping = spritePhysic->objectForKey("LinearDamping")->floatValue(); 
        float angularVelocity = spritePhysic->objectForKey("AngularVelocity")->floatValue();
        float angularDamping = spritePhysic->objectForKey("AngularDamping")->floatValue();   
        
        bool isBullet = spritePhysic->objectForKey("IsBullet")->boolValue();
        bool canSleep = spritePhysic->objectForKey("CanSleep")->boolValue();
        
        LHArray* fixtures = spritePhysic->arrayForKey("ShapeFixtures");
        CCPoint scale = LHPointFromString(spriteProp->objectForKey("Scale")->stringValue()); 
        
        CCPoint size = LHPointFromString(spriteProp->objectForKey("Size")->stringValue());
        
        CCPoint border = LHPointFromString(spritePhysic->objectForKey("ShapeBorder")->stringValue());
        
        CCPoint offset = LHPointFromString(spritePhysic->objectForKey("ShapePositionOffset")->stringValue());
        
        float gravityScale = spritePhysic->objectForKey("GravityScale")->floatValue();
        
        scale.x *= LHSettings::sharedInstance()->convertRatio().x;
        scale.y *= LHSettings::sharedInstance()->convertRatio().y;        
        
        //	if(scale.x == 0)
        //		scale.x = 0.01;
        //	if(scale.y == 0)
        //		scale.y = 0.01;
        
        float ptm = LHSettings::sharedInstance()->lhPtmRatio();
    
        if(fixtures == NULL || 
           fixtures->count() == 0 || 
           fixtures->objectAtIndex(0)->arrayValue()->count() == 0)
        {
            b2PolygonShape shape;
            b2FixtureDef fixture;
            b2CircleShape circle;
            setFixtureDefPropertiesFromDictionary(spritePhysic, &fixture);
            
            if(spritePhysic->objectForKey("IsCircle")->boolValue())
            {
                if(LHSettings::sharedInstance()->convertLevel())
                {
                    //    NSLog(@"convert circle");
                    //this is for the ipad scale on circle look weird if we dont do this
                    float scaleSpr = ccsprite->getScaleX();
                    ccsprite->setScaleY(scaleSpr);
                }
                
                float circleScale = scale.x; //if we dont do this we dont have collision
                if(circleScale < 0)
                    circleScale = -circleScale;
                    
                    //float radius = (size.x*circleScale/2.0f - border.x/2.0f*circleScale)/LHSettings::sharedInstance()->lhPtmRatio();
                    
                    float radius = ((size.x - border.x/2.0f)*circleScale/2.0f)/LHSettings::sharedInstance()->lhPtmRatio();
                
                
                
                    if(radius < 0)
                        radius *= -1;
                        circle.m_radius = radius; 
                        
                        circle.m_p.Set(offset.x/2.0f/LHSettings::sharedInstance()->lhPtmRatio(), 
                                       -offset.y/2.0f/LHSettings::sharedInstance()->lhPtmRatio());
                        
                        fixture.shape = &circle;
                        body->CreateFixture(&fixture);
                        }
            else
            {
                //THIS WAS ADDED BECAUSE I DISCOVER A BUG IN BOX2d
                //that makes linearImpulse to not work the body is in contact with
                //a box object
                int vsize = 4;
                b2Vec2 *verts = new b2Vec2[vsize];
                b2PolygonShape shape;
                
                
                if(scale.x * scale.y < 0.0f)
                {
                    verts[3].x = ( (-1* size.x + border.x/2.0f)*scale.x/2.0f+offset.x/2.0f)/ptm;
                    verts[3].y = ( (-1* size.y + border.y/2.0f)*scale.y/2.0f-offset.y/2.0f)/ptm;
                    
                    verts[2].x = ( (+ size.x - border.x/2.0f)*scale.x/2.0f+offset.x/2.0f)/ptm;
                    verts[2].y = ( (-1* size.y + border.y/2.0f)*scale.y/2.0f-offset.y/2.0f)/ptm;
                    
                    verts[1].x = ( (+ size.x - border.x/2.0f)*scale.x/2.0f+offset.x/2.0f)/ptm;
                    verts[1].y = ( (+ size.y - border.y/2.0f)*scale.y/2.0f-offset.y/2.0f)/ptm;
                    
                    verts[0].x = ( (-1* size.x + border.x/2.0f)*scale.x/2.0f+offset.x/2.0f)/ptm;
                    verts[0].y = ( (+ size.y - border.y/2.0f)*scale.y/2.0f-offset.y/2.0f)/ptm;
                }
                else
                {
                    verts[0].x = ( (-1* size.x + border.x/2.0f)*scale.x/2.0f+offset.x/2.0f)/ptm;
                    verts[0].y = ( (-1* size.y + border.y/2.0f)*scale.y/2.0f-offset.y/2.0f)/ptm;
                    
                    verts[1].x = ( (+ size.x - border.x/2.0f)*scale.x/2.0f+offset.x/2.0f)/ptm;
                    verts[1].y = ( (-1* size.y + border.y/2.0f)*scale.y/2.0f-offset.y/2.0f)/ptm;
                    
                    verts[2].x = ( (+ size.x - border.x/2.0f)*scale.x/2.0f+offset.x/2.0f)/ptm;
                    verts[2].y = ( (+ size.y - border.y/2.0f)*scale.y/2.0f-offset.y/2.0f)/ptm;
                    
                    verts[3].x = ( (-1* size.x + border.x/2.0f)*scale.x/2.0f+offset.x/2.0f)/ptm;
                    verts[3].y = ( (+ size.y - border.y/2.0f)*scale.y/2.0f-offset.y/2.0f)/ptm;
                    
                }
                
                shape.Set(verts, vsize);		
                
                fixture.shape = &shape;
                body->CreateFixture(&fixture);
                delete verts;
            }
        }
        else
        {
            for(int k = 0; k < fixtures->count(); ++k)
            {
                LHArray* curFixture = fixtures->objectAtIndex(k)->arrayValue();

                int size = (int)curFixture->count();
                b2Vec2 *verts = new b2Vec2[size];
                b2PolygonShape shape;
                int i = 0;
                
                for(int j = 0; j < curFixture->count(); ++j)
                {
                    std::string pointStr = curFixture->objectAtIndex(j)->stringValue();

                    CCPoint point = LHPointFromString(pointStr);
                    verts[i] = b2Vec2((point.x*(scale.x)+offset.x/2.0f)/ptm, 
                                      (point.y*(scale.y)-offset.y/2.0f)/ptm);
                    ++i;
                }
                shape.Set(verts, size);		
                b2FixtureDef fixture;
                setFixtureDefPropertiesFromDictionary(spritePhysic, &fixture);
                fixture.shape = &shape;
                body->CreateFixture(&fixture);
                delete[] verts;
            }
        }
	
    setCustomAttributesForPhysics(spriteProp, body, ccsprite);
	
	body->SetGravityScale(gravityScale);
	body->SetSleepingAllowed(canSleep);    
    body->SetBullet(isBullet);
    body->SetLinearVelocity(b2Vec2(linearVelocity.x, linearVelocity.y));
    body->SetAngularVelocity(angularVelocity);
    body->SetLinearDamping(linearDamping);
    body->SetAngularDamping(angularDamping);
	
	return body;
}

//------------------------------------------------------------------------------
void LevelHelperLoader::releaseAllSprites()
{
    removeAllSprites();
    spritesInLevel.removeAllObjects();
}
//------------------------------------------------------------------------------
bool LevelHelperLoader::removeSprite(LHSprite* ccsprite)
{
	if(NULL == ccsprite)
		return false;
    
    if(LHSprite::isLHSprite(ccsprite))
    {
        removeTouchDispatcherFromSprite(ccsprite);
        markedSprites.removeObjectForKey(ccsprite->getUniqueName());
        spritesInLevel.removeObjectForKey(ccsprite->getUniqueName());
    }    
    ccsprite->removeFromParentAndCleanup(true);
        
	return true;
}
//------------------------------------------------------------------------------
bool LevelHelperLoader::removeSpritesWithTag(enum LevelHelper_TAG tag)
{
    std::vector<std::string> keys = spritesInLevel.allKeys();
    
    for(size_t i = 0; i < keys.size(); ++i)
    {
        std::string key = keys[i];
        
        LHSprite* spr = (LHSprite*)spritesInLevel.objectForKey(key);
        
        if(NULL != spr){
            if(tag == spr->getTag()){
                removeSprite(spr);
            }
        }
    }
    return true;
}
//------------------------------------------------------------------------------
bool LevelHelperLoader::removeAllSprites(void)
{	
    std::vector<std::string> keys = spritesInLevel.allKeys();
    
    for(size_t i = 0; i < keys.size(); ++i)
    {
        LHSprite* spr = (LHSprite*)spritesInLevel.objectForKey(keys[i]);
        
        if(NULL != spr){
            removeSprite(spr);
        }
    }

    //spritesInLevel.removeAllObjects();    
    markedSprites.removeAllObjects();
	return true;	
}

//------------------------------------------------------------------------------
LHSprite* LevelHelperLoader::newSpriteWithUniqueName(const std::string& name)
{
    for(int i = 0; i< lhSprites->count(); ++i)
    {
        LHDictionary* dictionary = lhSprites->objectAtIndex(i)->dictValue();        
		LHDictionary* spriteProp = dictionary->dictForKey("GeneralProperties");
        
		if(spriteProp->objectForKey("UniqueName")->stringValue()  == name)
        {            
            LHSprite* ccsprite =  spriteFromDictionary(spriteProp);
            std::string uName = name + "_LH_NEW_SPRITE_" + stringFromInt(LHSettings::sharedInstance()->newBodyId());

            ccsprite->setUniqueName(uName.c_str());
            spritesInLevel.setObject(ccsprite, uName);
            ccsprite->postInit();
            return ccsprite;
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------
LHSprite* LevelHelperLoader::newPhysicalSpriteWithUniqueName(const std::string& name)
{
    for(int i = 0; i< lhSprites->count(); ++i)
    {
        LHDictionary* dictionary = lhSprites->objectAtIndex(i)->dictValue();        
		LHDictionary* spriteProp = dictionary->dictForKey("GeneralProperties");

		if(spriteProp->objectForKey("UniqueName")->stringValue()  == name)
        {
            LHDictionary* physicProp = dictionary->dictForKey("PhysicProperties");
            LHSprite* ccsprite = spriteFromDictionary(spriteProp);
            
            b2Body* body =  b2BodyFromDictionary(physicProp,
                                                 spriteProp,
                                                 ccsprite,
                                                 box2dWorld);
            
            if(0 != body)
                ccsprite->setBody(body);
            
            std::string uName = name + "_LH_NEW_BODY_" + stringFromInt(LHSettings::sharedInstance()->newBodyId());
            ccsprite->setUniqueName(uName.c_str());
            spritesInLevel.setObject(ccsprite, uName);
            ccsprite->postInit();
            return ccsprite;
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------
LHSprite* LevelHelperLoader::newBatchSpriteWithUniqueName(const std::string& name)
{
    for(int i = 0; i< lhSprites->count(); ++i)
    {
        LHDictionary* dictionary = lhSprites->objectAtIndex(i)->dictValue();        
		LHDictionary* spriteProp = dictionary->dictForKey("GeneralProperties");
        
		if(spriteProp->objectForKey("UniqueName")->stringValue()  == name)
        {
            //find the coresponding batch node for this sprite
            LHBatch* bNode = batchNodeForFile(spriteProp->objectForKey("Image")->stringValue());

            if(NULL != bNode){
                CCSpriteBatchNode *batch = bNode->getSpriteBatchNode();
                if(NULL != batch){
                    LHSprite* ccsprite = spriteWithBatchFromDictionary(spriteProp, bNode);
                    batch->addChild(ccsprite, spriteProp->objectForKey("ZOrder")->intValue());
            
                    std::string uName = name + "_LH_NEW_BATCH_SPRITE_" + stringFromInt(LHSettings::sharedInstance()->newBodyId());
                    ccsprite->setUniqueName(uName.c_str());
                    spritesInLevel.setObject(ccsprite, uName);
                    ccsprite->postInit();
                    return ccsprite;
                }
            }
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------
LHSprite* LevelHelperLoader::newPhysicalBatchSpriteWithUniqueName(const std::string& name)
{
    for(int i = 0; i< lhSprites->count(); ++i)
    {
        LHDictionary* dictionary = lhSprites->objectAtIndex(i)->dictValue();        
		LHDictionary* spriteProp = dictionary->dictForKey("GeneralProperties");
        
		if(spriteProp->objectForKey("UniqueName")->stringValue()  == name)
        {
            //find the coresponding batch node for this sprite
            
            LHBatch* bNode = batchNodeForFile(spriteProp->objectForKey("Image")->stringValue());
            if(NULL != bNode){
                CCSpriteBatchNode *batch = bNode->getSpriteBatchNode();
                if(NULL != batch){
                    LHSprite* ccsprite = spriteWithBatchFromDictionary(spriteProp, bNode);
                    batch->addChild(ccsprite, spriteProp->objectForKey("ZOrder")->intValue());
                    
                    LHDictionary* physicProp = dictionary->dictForKey("PhysicProperties");
                    b2Body* body =  b2BodyFromDictionary(physicProp,
                                                         spriteProp,
                                                         ccsprite,
                                                         box2dWorld);
                    
                    if(0 != body)
                        ccsprite->setBody(body);
                    
                    std::string uName = name + "_LH_NEW_BATCH_BODY_" + stringFromInt(LHSettings::sharedInstance()->newBodyId());
                
                    ccsprite->setUniqueName(uName.c_str());
                    spritesInLevel.setObject(ccsprite, uName);
                    ccsprite->postInit();
                    return ccsprite;
                }
            }
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------
LHSprite* LevelHelperLoader::spriteFromDictionary(LHDictionary* spriteProp)
{
    CCRect uv = LHRectFromString(spriteProp->objectForKey("UV")->stringValue());
    
    std::string img = LHSettings::sharedInstance()->imagePath(spriteProp->objectForKey("Image")->stringValue());
    
    if(LHSettings::sharedInstance()->shouldScaleImageOnRetina(img))
    {
        uv.origin.x *=2.0f;
        uv.origin.y *=2.0f;
        uv.size.width *=2.0f;
        uv.size.height *=2.0f;
    }
    
    int tag = spriteProp->objectForKey("Tag")->intValue();
    
    lh_spriteCreationMethods methods = LHCustomSpriteMgr::sharedInstance()->customSpriteClassForTag(tag);
    
	LHSprite *ccsprite =  (*methods.first)(img.c_str(), uv);
        
    //LHSprite::spriteWithFile(img.c_str(),uv);
    
	setSpriteProperties(ccsprite, spriteProp);
	return ccsprite;
}
//------------------------------------------------------------------------------
LHSprite* LevelHelperLoader::spriteWithBatchFromDictionary(LHDictionary* spriteProp,
                                                           LHBatch* lhBatch)
{
    CCRect uv = LHRectFromString(spriteProp->objectForKey("UV")->stringValue());
    
    if(lhBatch == NULL)
        return NULL;
    
    CCSpriteBatchNode* batch = lhBatch->getSpriteBatchNode();
    
    if(batch == NULL)
        return NULL;
    
    std::string img = LHSettings::sharedInstance()->imagePath(lhBatch->getUniqueName().c_str());
    
    if(LHSettings::sharedInstance()->shouldScaleImageOnRetina(img.c_str()))
    {
        uv.origin.x *=2.0f;
        uv.origin.y *=2.0f;
        uv.size.width *=2.0f;
        uv.size.height *=2.0f;
    }
    
    LHSprite *ccsprite = NULL;
    
    
    int tag = spriteProp->objectForKey("Tag")->intValue();
    lh_spriteCreationMethods methods = LHCustomSpriteMgr::sharedInstance()->customSpriteClassForTag(tag);
    
    if(!LHSettings::sharedInstance()->isCoronaUser())
        //ccsprite = LHSprite::spriteWithBatchNode(batch, uv);
        ccsprite =  (*methods.second)(batch, uv);
    else
        ccsprite =  (*methods.first)(img.c_str(), uv);
        //ccsprite = LHSprite::spriteWithFile(img.c_str(), uv);
            
    setSpriteProperties(ccsprite, spriteProp);
	
    return ccsprite;	
}
//------------------------------------------------------------------------------
void LevelHelperLoader::setSpriteProperties(LHSprite* ccsprite, LHDictionary* spriteProp)
{
	//convert position from LH to Cocos2d coordinates
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint position = LHPointFromString(spriteProp->objectForKey("Position")->stringValue().c_str());
	
	position.x *= LHSettings::sharedInstance()->convertRatio().x;
	position.y *= LHSettings::sharedInstance()->convertRatio().y;
    
    position.y = winSize.height - position.y;
    
    CCPoint pos_offset = LHSettings::sharedInstance()->possitionOffset();
    
    position.x += pos_offset.x;
    position.y -= pos_offset.y;
    
    ccsprite->setPosition(position);
	ccsprite->setRotation((float)spriteProp->objectForKey("Angle")->intValue());
    ccsprite->setOpacity(255*spriteProp->objectForKey("Opacity")->floatValue()*LHSettings::sharedInstance()->customAlpha());
	CCRect color = LHRectFromString(spriteProp->objectForKey("Color")->stringValue().c_str());
	ccsprite->setColor(ccc3(255*color.origin.x, 255*color.origin.y, 255*color.size.width));
	CCPoint scale = LHPointFromString(spriteProp->objectForKey("Scale")->stringValue().c_str());
	ccsprite->setIsVisible(spriteProp->objectForKey("IsDrawable")->boolValue());
    
    int tag = spriteProp->objectForKey("Tag")->intValue();
    ccsprite->setTag(tag);
    
	scale.x *= LHSettings::sharedInstance()->convertRatio().x;
	scale.y *= LHSettings::sharedInstance()->convertRatio().y;
    
    std::string img = LHSettings::sharedInstance()->imagePath(spriteProp->objectForKey("Image")->stringValue().c_str());
    
    ccsprite->setRealScale(CCSizeMake(scale.x, scale.y));
    
    if(LHSettings::sharedInstance()->shouldScaleImageOnRetina(img.c_str()))
    {
        scale.x /=2.0f;
        scale.y /=2.0f;        
    }
    
    //this is to fix a noise issue on cocos2d.
    // scale.x += 0.0005f*scale.x;
    // scale.y += 0.0005f*scale.y;
    
    setTouchDispatcherForSpriteWithTag(ccsprite, tag);
	ccsprite->setScaleX(scale.x);
	ccsprite->setScaleY(scale.y);
    ccsprite->setUniqueName(spriteProp->objectForKey("UniqueName")->stringValue().c_str());
    ccsprite->parentLoader = this;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::setTouchDispatcherForBezierWithTag(LHBezierNode* object, int tag){

    object->setTagTouchBeginObserver(LHTouchMgr::sharedInstance()->onTouchBeginObserverForTag(tag));
    object->setTagTouchMovedObserver(LHTouchMgr::sharedInstance()->onTouchMovedObserverForTag(tag));
    object->setTagTouchEndedObserver(LHTouchMgr::sharedInstance()->onTouchEndedObserverForTag(tag));
    
    bool swallow = LHTouchMgr::sharedInstance()->shouldTouchesBeSwallowedForTag(tag);
    int priority = LHTouchMgr::sharedInstance()->priorityForTag(tag);

    object->swallowTouches = swallow;
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(object, priority, swallow);       
}
//------------------------------------------------------------------------------
void LevelHelperLoader::setTouchDispatcherForSpriteWithTag(LHSprite* object, int tag)
{
    object->setTagTouchBeginObserver(LHTouchMgr::sharedInstance()->onTouchBeginObserverForTag(tag));
    object->setTagTouchMovedObserver(LHTouchMgr::sharedInstance()->onTouchMovedObserverForTag(tag));
    object->setTagTouchEndedObserver(LHTouchMgr::sharedInstance()->onTouchEndedObserverForTag(tag));
    
    bool swallow = LHTouchMgr::sharedInstance()->shouldTouchesBeSwallowedForTag(tag);
    int priority = LHTouchMgr::sharedInstance()->priorityForTag(tag);

    object->swallowTouches = swallow;
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(object, priority, swallow);        
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removeTouchDispatcherFromSprite(LHSprite* object){
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(object);    
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removeTouchDispatcherFromBezier(LHBezierNode* object){
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(object);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//PARALLAX
////////////////////////////////////////////////////////////////////////////////
LHParallaxNode* LevelHelperLoader::paralaxNodeWithUniqueName(const std::string& uniqueName){
    CCLog("paralaxNodeWithUniqueName is deprecated. Please use parallaxNodeWithUniqueName. (2 ll's)");
    return parallaxNodeWithUniqueName(uniqueName);
}
//------------------------------------------------------------------------------
LHParallaxNode* LevelHelperLoader::parallaxNodeWithUniqueName(const std::string& uniqueName)
{
    return (LHParallaxNode*)parallaxesInLevel.objectForKey(uniqueName);
}
//------------------------------------------------------------------------------
CCArray* LevelHelperLoader::allParallaxes(void){
    std::vector<std::string> keys = parallaxesInLevel.allKeys();
    CCArray* allNodes = CCArray::array();
    for(size_t i = 0; i < keys.size(); ++i){        
        LHParallaxNode* node = (LHParallaxNode*)parallaxesInLevel.objectForKey(keys[i]);
        if(node)
            allNodes->addObject(node);
	}
    return allNodes;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createParallaxes(void)
{
    for(int i = 0; i< lhParallax->count(); ++i){
        
        LHDictionary* parallaxDict = lhParallax->objectAtIndex(i)->dictValue();

		LHParallaxNode* node = parallaxNodeFromDictionary(parallaxDict, cocosLayer);
        if(NULL != node){
            node->parentLoader = this;
			parallaxesInLevel.setObject(node, 
                                        parallaxDict->objectForKey("UniqueName")->stringValue());
		}
    }
}
//------------------------------------------------------------------------------
LHParallaxNode*  LevelHelperLoader::parallaxNodeFromDictionary(LHDictionary* parallaxDict,CCLayer* layer)
{
	LHParallaxNode* node = LHParallaxNode::nodeWithDictionary(parallaxDict);
    
    if(layer != NULL && node != NULL){
        int z = parallaxDict->objectForKey("ZOrder")->intValue();
        layer->addChild(node, z);
    }
    
    LHArray* spritesInfo = parallaxDict->objectForKey("Sprites")->arrayValue();

    for(int i = 0; i < spritesInfo->count(); ++i)
    {
        LHDictionary* sprInf = spritesInfo->objectAtIndex(i)->dictValue();

        float ratioX = sprInf->objectForKey("RatioX")->floatValue();
        float ratioY = sprInf->objectForKey("RatioY")->floatValue();
        std::string sprName = sprInf->objectForKey("SpriteName")->stringValue();
        
		LHSprite* spr = spriteWithUniqueName(sprName);
		if(NULL != node && spr != NULL){
			node->addSprite(spr, ccp(ratioX, ratioY));
		}
    }
    return node;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removeAllParallaxes(bool removeSprites)
{
    std::vector<std::string> keys = parallaxesInLevel.allKeys();
    
    for(size_t i = 0; i < keys.size(); ++i)
    {
        std::string key = keys[i];
        
        LHParallaxNode* par = (LHParallaxNode*)parallaxesInLevel.objectForKey(key);
        
        if(NULL != par){
            par->removeSpritesOnDelete = removeSprites;
            par->removeFromParentAndCleanup(true);
        }
    }
    parallaxesInLevel.removeAllObjects();
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removeParallaxNode(LHParallaxNode* node, bool removeSprites){
    
    if(NULL == node)
        return;    
    
    node->removeSpritesOnDelete = removeSprites;
    parallaxesInLevel.removeObjectForKey(node->getUniqueName());
    
    node->removeFromParentAndCleanup(true);
}
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
//JOINTS
////////////////////////////////////////////////////////////////////////////////
LHJoint* LevelHelperLoader::jointFromDictionary(LHDictionary* joint, b2World* world)
{
    b2Joint* boxJoint = 0;
    
	if(NULL == joint)
		return 0;
	
	if(world == 0)
		return 0;
    
    LHSprite* sprA = (LHSprite*)spritesInLevel.objectForKey(joint->objectForKey("ObjectA")->stringValue());
    b2Body* bodyA = sprA->getBody();
	
    LHSprite* sprB = (LHSprite*)spritesInLevel.objectForKey(joint->objectForKey("ObjectB")->stringValue());
    b2Body* bodyB = sprB->getBody();
	
    CCPoint sprPosA = sprA->getPosition();
    CCPoint sprPosB = sprB->getPosition();
    
    CCSize scaleA = sprA->getRealScale();
    CCSize scaleB = sprB->getRealScale();
    
	if(NULL == bodyA || 
       NULL == bodyB )
		return NULL;
	
	CCPoint anchorA = LHPointFromString(joint->objectForKey("AnchorA")->stringValue());
	CCPoint anchorB = LHPointFromString(joint->objectForKey("AnchorB")->stringValue());
    
	bool collideConnected = joint->objectForKey("CollideConnected")->boolValue();
	
    int tag = joint->objectForKey("Tag")->intValue();
    int type = joint->objectForKey("Type")->intValue();
    
	b2Vec2 posA, posB;
	
	float convertX = LHSettings::sharedInstance()->convertRatio().x;
	float convertY = LHSettings::sharedInstance()->convertRatio().y;
    
    float ptm = LHSettings::sharedInstance()->lhPtmRatio();
    
    if(!joint->objectForKey("CenterOfMass")->boolValue())
    {        
        posA = b2Vec2((sprPosA.x + anchorA.x*scaleA.width)/ptm, 
                      (sprPosA.y - anchorA.y*scaleA.height)/ptm);
        
        posB = b2Vec2((sprPosB.x + anchorB.x*scaleB.width)/ptm, 
                      (sprPosB.y - anchorB.y*scaleB.height)/ptm);
        
    }
    else {		
        posA = bodyA->GetWorldCenter();
        posB = bodyB->GetWorldCenter();
    }
	
	if(0 != bodyA && 0 != bodyB)
	{
		switch (type)
		{
			case LH_DISTANCE_JOINT:
			{
				b2DistanceJointDef jointDef;
				
				jointDef.Initialize(bodyA, 
									bodyB, 
									posA,
									posB);
				
				jointDef.collideConnected = collideConnected;
				
				jointDef.frequencyHz = joint->objectForKey("Frequency")->floatValue();
				jointDef.dampingRatio = joint->objectForKey("Damping")->floatValue();
				
				if(0 != world)
				{
					boxJoint = (b2DistanceJoint*)world->CreateJoint(&jointDef);
				}
			}	
				break;
				
			case LH_REVOLUTE_JOINT:
			{
				b2RevoluteJointDef jointDef;
				
				jointDef.lowerAngle = CC_DEGREES_TO_RADIANS(joint->objectForKey("LowerAngle")->floatValue());
				jointDef.upperAngle = CC_DEGREES_TO_RADIANS(joint->objectForKey("UpperAngle")->floatValue());
				jointDef.motorSpeed = joint->objectForKey("MotorSpeed")->floatValue(); //Usually in radians per second. ?????
				jointDef.maxMotorTorque = joint->objectForKey("MaxTorque")->floatValue(); //Usually in N-m.  ?????
				jointDef.enableLimit = joint->objectForKey("EnableLimit")->boolValue();
				jointDef.enableMotor = joint->objectForKey("EnableMotor")->boolValue();
				jointDef.collideConnected = collideConnected;    
				
				jointDef.Initialize(bodyA, bodyB, posA);
				
				if(0 != world)
				{
					boxJoint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
				}
			}
				break;
				
			case LH_PRISMATIC_JOINT:
			{
				b2PrismaticJointDef jointDef;
				
				// Bouncy limit
				CCPoint axisPt = LHPointFromString(joint->objectForKey("Axis")->stringValue());
				
				b2Vec2 axis(axisPt.x, axisPt.y);
				axis.Normalize();
				
				jointDef.Initialize(bodyA, bodyB, posA, axis);
				
				jointDef.motorSpeed = joint->objectForKey("MotorSpeed")->floatValue();
				jointDef.maxMotorForce = joint->objectForKey("MaxMotorForce")->floatValue();
				
				
				jointDef.lowerTranslation =  CC_DEGREES_TO_RADIANS(joint->objectForKey("LowerTranslation")->floatValue());
				jointDef.upperTranslation = CC_DEGREES_TO_RADIANS(joint->objectForKey("UpperTranslation")->floatValue());
				
				jointDef.enableMotor = joint->objectForKey("EnableMotor")->boolValue();
				jointDef.enableLimit = joint->objectForKey("EnableLimit")->boolValue();
				jointDef.collideConnected = collideConnected;   
				if(0 != world)
				{
					boxJoint = (b2PrismaticJoint*)world->CreateJoint(&jointDef);
				}
			}	
				break;
				
			case LH_PULLEY_JOINT:
			{
				b2PulleyJointDef jointDef;
				
				CCPoint grAnchorA = LHPointFromString(joint->objectForKey("GroundAnchorA")->stringValue());
				CCPoint grAnchorB = LHPointFromString(joint->objectForKey("GroundAnchorB")->stringValue());
				
				CCSize winSize = CCDirector::sharedDirector()->getDisplaySizeInPixels();
				
				grAnchorA.y = winSize.height - convertY*grAnchorA.y;
				grAnchorB.y = winSize.height - convertY*grAnchorB.y;
				
				b2Vec2 groundAnchorA = b2Vec2(convertX*grAnchorA.x/ptm, 
											  grAnchorA.y/ptm);
				
				b2Vec2 groundAnchorB = b2Vec2(convertX*grAnchorB.x/ptm, 
											  grAnchorB.y/ptm);
				
				float ratio = joint->objectForKey("Ratio")->floatValue();
				jointDef.Initialize(bodyA, bodyB, groundAnchorA, groundAnchorB, posA, posB, ratio);				
				jointDef.collideConnected = collideConnected;   
				
				if(0 != world)
				{
					boxJoint = (b2PulleyJoint*)world->CreateJoint(&jointDef);
				}
			}
				break;
				
			case LH_GEAR_JOINT:
			{
				b2GearJointDef jointDef;
				
				jointDef.bodyA = bodyB;
				jointDef.bodyB = bodyA;
				
				if(bodyA == 0)
					return 0;
				if(bodyB == 0)
					return 0;
				
                LHJoint* jointAObj = jointWithUniqueName(joint->objectForKey("JointA")->stringValue());
                b2Joint* jointA = jointAObj->getJoint();
                
                LHJoint* jointBObj = jointWithUniqueName(joint->objectForKey("JointB")->stringValue());
                b2Joint* jointB = jointBObj->getJoint();
                
				if(jointA == 0)
					return 0;
				if(jointB == 0)
					return 0;
				
				
				jointDef.joint1 = jointA;
				jointDef.joint2 = jointB;
				
				jointDef.ratio = joint->objectForKey("Ratio")->floatValue();
				jointDef.collideConnected = collideConnected;
				if(0 != world)
				{
					boxJoint = (b2GearJoint*)world->CreateJoint(&jointDef);
				}
			}	
				break;
				
				
			case LH_WHEEL_JOINT: //aka line joint
			{
				b2WheelJointDef jointDef;
				
				CCPoint axisPt = LHPointFromString(joint->objectForKey("Axis")->stringValue());
				b2Vec2 axis(axisPt.x, axisPt.y);
				axis.Normalize();
				
				jointDef.motorSpeed = joint->objectForKey("MotorSpeed")->floatValue(); //Usually in radians per second. ?????
				jointDef.maxMotorTorque = joint->objectForKey("MaxTorque")->floatValue(); //Usually in N-m.  ?????
				jointDef.enableMotor = joint->objectForKey("EnableMotor")->boolValue();
				jointDef.frequencyHz = joint->objectForKey("Frequency")->floatValue();
				jointDef.dampingRatio = joint->objectForKey("Damping")->floatValue();
				
				jointDef.Initialize(bodyA, bodyB, posA, axis);
				jointDef.collideConnected = collideConnected; 
				
				if(0 != world)
				{
					boxJoint = (b2WheelJoint*)world->CreateJoint(&jointDef);
				}
			}
				break;				
			case LH_WELD_JOINT:
			{
				b2WeldJointDef jointDef;
				
				jointDef.frequencyHz = joint->objectForKey("Frequency")->floatValue();
				jointDef.dampingRatio = joint->objectForKey("Damping")->floatValue();
				
				jointDef.Initialize(bodyA, bodyB, posA);
				jointDef.collideConnected = collideConnected; 
				
				if(0 != world)
				{
					boxJoint = (b2WheelJoint*)world->CreateJoint(&jointDef);
				}
			}
				break;
				
			case LH_ROPE_JOINT: //NOT WORKING YET AS THE BOX2D JOINT FOR THIS TYPE IS A TEST JOINT
			{
				
				b2RopeJointDef jointDef;
				
				jointDef.localAnchorA = bodyA->GetPosition();
				jointDef.localAnchorB = bodyB->GetPosition();
				jointDef.bodyA = bodyA;
				jointDef.bodyB = bodyB;
				jointDef.maxLength = joint->objectForKey("MaxLength")->floatValue();
				jointDef.collideConnected = collideConnected; 
				
				if(0 != world)
				{
					boxJoint = (b2RopeJoint*)world->CreateJoint(&jointDef);
				}
			}
				break;
				
			case LH_FRICTION_JOINT:
			{
				b2FrictionJointDef jointDef;
				
				jointDef.maxForce   = joint->objectForKey("MaxForce")->floatValue();
				jointDef.maxTorque  = joint->objectForKey("MaxTorque")->floatValue();
				
				jointDef.Initialize(bodyA, bodyB, posA);
				jointDef.collideConnected = collideConnected; 
				
				if(0 != world)
				{
					boxJoint = (b2FrictionJoint*)world->CreateJoint(&jointDef);
				}
				
			}
				break;
				
			default:
				CCLog("Unknown joint type in LevelHelper file.");
				break;
		}
	}
    
    LHJoint* levelJoint = LHJoint::jointWithUniqueName(joint->objectForKey("UniqueName")->stringValue().c_str(), 
                                                       tag, 
                                                       (LH_JOINT_TYPE)type, 
                                                       boxJoint);
    //levelJoint->getTag() = tag;
    //levelJoint->type = (LH_JOINT_TYPE)type;
    //levelJoint->joint = boxJoint;
    boxJoint->SetUserData(levelJoint);
    
	return levelJoint;
}
//------------------------------------------------------------------------------
LHJoint* LevelHelperLoader::jointWithUniqueName(const std::string& name)
{
    return (LHJoint*)jointsInLevel.objectForKey(name);
}
//------------------------------------------------------------------------------
CCArray* LevelHelperLoader::jointsWithTag(enum LevelHelper_TAG tag){
    std::vector<std::string> keys = jointsInLevel.allKeys();
    CCArray* jointsWithTag = CCArray::array();
    for(size_t i = 0; i < keys.size(); ++i){        
        LHJoint* levelJoint = (LHJoint*)jointsInLevel.objectForKey(keys[i]);
        if(levelJoint->getTag() == (int)tag){
            jointsWithTag->addObject(levelJoint);
        }
	}
    return jointsWithTag;
}
//------------------------------------------------------------------------------
CCArray* LevelHelperLoader::allJoints(void){
    std::vector<std::string> keys = jointsInLevel.allKeys();
    CCArray* allJoints = CCArray::array();
    for(size_t i = 0; i < keys.size(); ++i){        
        LHJoint* levelJoint = (LHJoint*)jointsInLevel.objectForKey(keys[i]);
        if(levelJoint)
            allJoints->addObject(levelJoint);
	}
    return allJoints;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createJoints(void)
{
    for(int i = 0; i < lhJoints->count(); ++i)
    {
        LHDictionary* jointDict = lhJoints->objectAtIndex(i)->dictValue();

		LHJoint* boxJoint = jointFromDictionary(jointDict, box2dWorld);
		
		if(NULL != boxJoint){
			jointsInLevel.setObject(boxJoint,
                                    jointDict->objectForKey("UniqueName")->stringValue());	
		}
	}	
}
//------------------------------------------------------------------------------
bool LevelHelperLoader::removeAllJoints(void){    
    jointsInLevel.removeAllObjects();
    markedJoints.removeAllObjects();
    return true;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::releaseAllJoints(void)
{
    removeAllJoints();
}
//------------------------------------------------------------------------------
void LevelHelperLoader::markJointForRemoval(LHJoint* jt){
    
    if(NULL == jt)
        return;
    
    markedJoints.setObject(jt, jt->getUniqueName());
}
//------------------------------------------------------------------------------
void LevelHelperLoader::markJointsAttachedToSpriteForRemoval(LHSprite* spr){
    
    if(NULL == spr)
        return;
    
    CCArray* joints = spr->jointList();
    
    if(joints == NULL)
        return;
    
    for(int i = 0; i < (int)joints->count(); ++i)
    {
        LHJoint* jt = (LHJoint*)joints->objectAtIndex(i);
        markJointForRemoval(jt);
    }
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removeMarkedJoints(void){

    std::vector<std::string> keys = markedJoints.allKeys();
    for(size_t i = 0; i< keys.size(); ++i){
        LHJoint* jt = (LHJoint*)markedJoints.objectForKey(keys[i]);
        removeJoint(jt);
    }
    markedJoints.removeAllObjects();
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removeJointsWithTag(enum LevelHelper_TAG tag)
{
    std::vector<std::string> keys = jointsInLevel.allKeys();
    
    for(size_t i = 0; i< keys.size(); ++i)
    {
        std::string key = keys[i];
        LHJoint* jt = (LHJoint*)jointsInLevel.objectForKey(key);
        
        if(NULL != jt)
        {
            if(jt->getTag() == tag)
            {
                jointsInLevel.removeObjectForKey(key);
            }
        }
    }
}
//------------------------------------------------------------------------------
bool LevelHelperLoader::removeJoint(LHJoint* joint)
{
	if(0 == joint)
		return false;
    
    markedJoints.removeObjectForKey(joint->getUniqueName());
    jointsInLevel.removeObjectForKey(joint->getUniqueName());
	return true;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::setCustomAttributesForPhysics(LHDictionary* spriteProp, b2Body* body, LHSprite* sprite)
{
    
}
void LevelHelperLoader::setCustomAttributesForNonPhysics(LHDictionary* spriteProp, LHSprite* sprite)
{
    
}

void LevelHelperLoader::setCustomAttributesForBezierBodies(LHDictionary* bezierProp, CCNode* sprite, b2Body* body)
{
  
}

////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::loadLevelHelperSceneFile(const char* levelFile,
                                                 const char* subfolder, 
                                                 const char* imgFolder)
{
    
    unsigned char* levelFileBuffer = NULL;
    unsigned long bufferSize = 0;
    std::string fullPath = CCFileUtils::fullPathFromRelativePath(levelFile);
    levelFileBuffer = CCFileUtils::getFileData(fullPath.c_str(), "r", &bufferSize);
    
	CCAssert(bufferSize > 0, "Invalid level file. Please add the LevelHelper scene file to Resource folder.");
    
    std::string filecontents((const char*) levelFileBuffer, bufferSize);
    std::stringstream infile(filecontents, stringstream::in);
    
    
    LHDictionary* dictionary = new LHDictionary(infile);
    
    processLevelFileFromDictionary(dictionary);
    
    delete dictionary;  
    
    delete [] levelFileBuffer;
}
//------------------------------------------------------------------------------
LHBatch* LevelHelperLoader::loadBatchNodeWithImage(const std::string& image)
{
    if("" == image)
        return 0;

    LHDictionary* imageInfo = lhBatchInfo->dictForKey(image.c_str());
    
    if(0 == imageInfo)
        return 0;
    
    CCSpriteBatchNode *batch = CCSpriteBatchNode::batchNodeWithFile(LHSettings::sharedInstance()->imagePath(image.c_str()).c_str());
    
    LHBatch* bNode = LHBatch::batchWithUniqueName(image);
    bNode->setSpriteBatchNode(batch);
    
    bNode->setZ(imageInfo->objectForKey("OrderZ")->intValue());
    batchNodesInLevel.setObject(bNode, image);		
    return bNode;
}

void LevelHelperLoader::addBatchNodesToLayer(CCLayer* _cocosLayer)
{
    if(!LHSettings::sharedInstance()->preloadBatchNodes())
        return;
    
    std::vector<std::string> keys = batchNodesInLevel.allKeys();

	int tag = 0;
    for(size_t i = 0; i < keys.size(); ++i)
    {
        std::string key = keys[i];
        
        LHBatch* info = (LHBatch*)batchNodesInLevel.objectForKey(key);
        
        _cocosLayer->addChild(info->getSpriteBatchNode(), info->getZ(), tag);
    }
}

void LevelHelperLoader::addBatchNodeToLayer(CCLayer* _cocosLayer, LHBatch* info)
{
    if(info!= 0 && _cocosLayer != 0){
        _cocosLayer->addChild(info->getSpriteBatchNode(), info->getZ());
    }
}

//------------------------------------------------------------------------------
void LevelHelperLoader::releaseAllBatchNodes(void)
{
    batchNodesInLevel.removeAllObjects();
    //delete batchNodesInLevel;
    //batchNodesInLevel = 0;
}

//this will load the batch if its not loaded
LHBatch* LevelHelperLoader::batchNodeForFile(const std::string& image)
{
    LHBatch* bNode = (LHBatch*)batchNodesInLevel.objectForKey(image);
    if(0 != bNode){
        return bNode;
    }
    else{
        bNode = loadBatchNodeWithImage(image);
        addBatchNodeToLayer(cocosLayer, bNode);
        return bNode;
    }
    return 0;
}

void LevelHelperLoader::removeUnusedBatchesFromMemory(void)
{
    std::vector<std::string> keys = batchNodesInLevel.allKeys();
	for(size_t i = 0; i < keys.size(); ++i)
    {
        std::string key = keys[i];
        
        LHBatch* bNode = (LHBatch*)batchNodesInLevel.objectForKey(key);
        
        if(bNode)
        {
            CCSpriteBatchNode* cNode = bNode->getSpriteBatchNode();
            
            if(0 == (int)(cNode->getDescendants()->count()))
            {
                //delete bNode;
                batchNodesInLevel.removeObjectForKey(key);
            }
        }
    }    
}
 
void LevelHelperLoader::processLevelFileFromDictionary(LHDictionary* dictionary)
{
	if(0 == dictionary)
		return;
    
	bool fileInCorrectFormat =	dictionary->objectForKey("Author")->stringValue() == "Bogdan Vladu" && 
                                dictionary->objectForKey("CreatedWith")->stringValue() == "LevelHelper";
	
	if(fileInCorrectFormat == false)
		CCLog("This file was not created with LevelHelper or file is damaged.");
        
        LHDictionary* scenePref = dictionary->dictForKey("ScenePreference");
        safeFrame = LHPointFromString(scenePref->objectForKey("SafeFrame")->stringValue());
        gameWorldRect = LHRectFromString(scenePref->objectForKey("GameWorld")->stringValue());
        
        
        CCRect color = LHRectFromString(scenePref->objectForKey("BackgroundColor")->stringValue());
        glClearColor(color.origin.x, color.origin.y, color.size.width, 1);
        
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    LHSettings::sharedInstance()->setConvertRatio(CCPointMake(winSize.width/safeFrame.x,
                                                                 winSize.height/safeFrame.y));
    
    float safeFrameDiagonal = sqrtf(safeFrame.x* safeFrame.x + safeFrame.y* safeFrame.y);
    float winDiagonal = sqrtf(winSize.width* winSize.width + winSize.height*winSize.height);
    float PTM_conversion = winDiagonal/safeFrameDiagonal;
    
    LevelHelperLoader::setMeterRatio(LHSettings::sharedInstance()->lhPtmRatio()*PTM_conversion);
    
	////////////////////////LOAD WORLD BOUNDARIES///////////////////////////////
	if(NULL != dictionary->objectForKey("WBInfo"))
	{
		wb = new LHDictionary(dictionary->dictForKey("WBInfo"));
	}
	
	////////////////////////LOAD SPRITES////////////////////////////////////////
    lhSprites = new LHArray(dictionary->objectForKey("SPRITES_INFO")->arrayValue());
	
    //load batch nodes only if asked
	///////////////////////////LOAD BATCH IMAGES////////////////////////////////
    LHArray* batchInfo = dictionary->objectForKey("LoadedImages")->arrayValue();

    for(int i = 0; i < batchInfo->count(); ++i)
    {
        LHDictionary* imageInfo = batchInfo->objectAtIndex(i)->dictValue();
    
        std::string image = imageInfo->objectForKey("Image")->stringValue();	    
        lhBatchInfo->setObjectForKey(new LHObject(new LHDictionary(imageInfo)), image);
        
        if(LHSettings::sharedInstance()->preloadBatchNodes())
        {
            loadBatchNodeWithImage(image);
        }
    }
	
	///////////////////////LOAD JOINTS//////////////////////////////////////////
	lhJoints = new LHArray(dictionary->objectForKey("JOINTS_INFO")->arrayValue());	
	
    //////////////////////LOAD PARALLAX/////////////////////////////////////////
    lhParallax = new LHArray(dictionary->objectForKey("PARALLAX_INFO")->arrayValue());
    
    ////////////////////LOAD BEZIER/////////////////////////////////////////////
    lhBeziers = new LHArray(dictionary->objectForKey("BEZIER_INFO")->arrayValue());
    
    ////////////////////LOAD ANIMS//////////////////////////////////////////////
    lhAnims = new LHArray(dictionary->objectForKey("ANIMS_INFO")->arrayValue());
    
    gravity = LHPointFromString(dictionary->objectForKey("Gravity")->stringValue());
}
////////////////////////////////////////////////////////////////////////////////