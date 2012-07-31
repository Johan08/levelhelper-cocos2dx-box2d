//  This file is part of LevelHelper
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
////////////////////////////////////////////////////////////////////////////////
//
//  Version history
//  ...............
//  v0.1 First version for LevelHelper 1.4.9x
////////////////////////////////////////////////////////////////////////////////

#ifndef __LEVEL_HELPER_LOADER__
#define __LEVEL_HELPER_LOADER__

#include "cocos2d.h"
#include "cocoa/CCNS.h"
#include "Box2D/Box2D.h"

#include "Utilities/LHDictionary.h"
#include "Utilities/LHArray.h"
#include "Utilities/LHObject.h"

#include "Nodes/LHCustomSpriteMgr.h"
#include "Nodes/LHContactNode.h"
#include "Nodes/LHAnimationNode.h"
#include "Nodes/LHParallaxNode.h"
#include "Nodes/LHContactInfo.h"

#include "Nodes/LHNode.h"
#include "Nodes/LHLayer.h"
#include "Nodes/LHBatch.h"
#include "Nodes/LHSprite.h"
#include "Nodes/LHBezier.h"
#include "Nodes/LHJoint.h"
#include "Nodes/LHPathNode.h"

#include "CustomClasses/LHCustomClasses.h"
using namespace cocos2d;

enum LevelHelper_TAG;

std::string stringFromInt(const int& i);

typedef void (CCObject::*SEL_CallFuncFloat)(float);
#define callfuncFloat_selector(_SELECTOR) (SEL_CallFuncFloat)(&_SELECTOR)


class LevelHelperLoader : public CCObject {

private:
	LHArray* lhNodes;	//array of NSDictionary //includes LHSprite, LHBezier, LHBatch, LHLayer
    LHArray* lhJoints;	//array of NSDictionary
    LHArray* lhParallax;//array of NSDictionary 
    LHDictionary*  wb;//world boundaries info;
    
    LHLayer* mainLHLayer;
    
#if COCOS2D_VERSION >= 0x00020000
    
    CCDictionary jointsInLevel;
    CCDictionary parallaxesInLevel;    
    CCDictionary physicBoundariesInLevel;
#else
    CCMutableDictionary<std::string>  jointsInLevel;        //key name - value LHJoint*
    CCMutableDictionary<std::string>  parallaxesInLevel;    //key name - value LHParallaxNode*
    CCMutableDictionary<std::string>  physicBoundariesInLevel;
#endif
    
    CCPoint safeFrame;
    CCRect  gameWorldRect;
    CCPoint gravity;
	    
	CCLayer* cocosLayer; //weak ptr
    b2World* box2dWorld; //weak ptr
    
    LHContactNode* contactNode;
        
        
    CCObject* loadingProgressId;
    SEL_CallFuncFloat loadingProgressSel;
    
public:

    friend class LHSprite;
    
    //------------------------------------------------------------------------------
    LevelHelperLoader(const char* levelFile);
    virtual ~LevelHelperLoader();
    //------------------------------------------------------------------------------
    
    //will call this selector during loading the level (addObjectsToWorld or addSpritesToLayer)
    //the registered method needs to have this signature " void HelloWorld::loadingProgress(float percentage) "
    //registration should be done like this loader->registerLoadingProgressObserver(this, callfuncFloat_selector(HelloWorld::loadingProgress));
    //percentage will return a value from 0.0f to 1.0f
    void registerLoadingProgressObserver(CCObject* loadingProgressObj, SEL_CallFuncFloat sel);
    
    //LOADING
    void addObjectsToWorld(b2World* world, CCLayer* cocosLayer);
    
    
    LHLayer*  layerWithUniqueName(const std::string& name);
    LHBatch*  batchWithUniqueName(const std::string& name);
    LHSprite* spriteWithUniqueName(const std::string& name);
    LHBezier* bezierWithUniqueName(const std::string& name);
    LHJoint*  jointWithUniqueName(const std::string& name);
    LHParallaxNode* parallaxNodeWithUniqueName(const std::string& uniqueName);
    
    CCArray* allLayers();
    CCArray* allBatches();
    CCArray* allSprites();
    CCArray* allBeziers();
    CCArray* allJoints();
    CCArray* allParallaxes();
    
    CCArray* layersWithTag(enum LevelHelper_TAG tag);
    CCArray* batchesWithTag(enum LevelHelper_TAG tag);
    CCArray* spritesWithTag(enum LevelHelper_TAG tag);
    CCArray* beziersWithTag(enum LevelHelper_TAG tag);
    CCArray* jointsWithTag(enum LevelHelper_TAG tag);
    
    
    /*
     to remove any of the LHLayer, LHBatch, LHSprite, LHBezier, LHJoint objects call
     object->removeSelf();
     
     if you retain it somewhere the object will not be release - so make sure you dont retain
     any of the objects
     */

    
    
    
    
    
    
    
    
//    void addSpritesToLayer(CCLayer* cocosLayer); //NO PHYSICS
    //------------------------------------------------------------------------------
    static void dontStretchArtOnIpad(void);

    //------------------------------------------------------------------------------
    //PAUSING THE GAME
    //this will pause all path movement and all parallaxes
    //use  [[CCDirector sharedDirector] pause]; for everything else
    static bool isPaused(void);
    static void setPaused(bool value); //pass true to pause, false to unpause

    
    //------------------------------------------------------------------------------
    //COLLISION HANDLING
    //see API Documentation on the website to see how to use this
    void useLevelHelperCollisionHandling(void);
    
    void registerBeginOrEndCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                        enum LevelHelper_TAG tagB,
                                                        CCObject* obj,
                                                        SEL_CallFuncO selector);
    
    void cancelBeginOrEndCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                      enum LevelHelper_TAG tagB);

    
    void registerPreCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                enum LevelHelper_TAG tagB,
                                                CCObject* obj,
                                                SEL_CallFuncO selector);
    
    void cancelPreCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                               enum LevelHelper_TAG tagB);
    
    void registerPostCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                 enum LevelHelper_TAG tagB,
                                                 CCObject* obj,
                                                 SEL_CallFuncO selector);
    
    void cancelPostCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                enum LevelHelper_TAG tagB);

    
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
//    //CREATION
//    
//    //New sprite and associated body will be released automatically
//    //or you can use removeFromParentAndCleanup(true), CCSprite method, to do it at a specific time
//    //you must set the desired position after creation
//    
//    //sprites returned needs to be added in the layer by you
//    //new sprite unique name for the returned sprite will be
//    //[OLDNAME]_LH_NEW__SPRITE_XX and [OLDNAME]_LH_NEW_BODY_XX
//    LHSprite* newSpriteWithUniqueName(const std::string& name); //no physic body
//    LHSprite* newPhysicalSpriteWithUniqueName(const std::string& name);//with physic body
//    
//    //sprites are added in the coresponding batch node automatically
//    //new sprite unique name for the returned sprite will be
//    //[OLDNAME]_LH_NEW_BATCH_SPRITE_XX and [OLDNAME]_LH_NEW_BATCH_BODY_XX
//    LHSprite* newBatchSpriteWithUniqueName(const std::string& name); //no physic body
//    LHSprite* newPhysicalBatchSpriteWithUniqueName(const std::string& name); //with physic body
//    
//    //------------------------------------------------------------------------------

//    //------------------------------------------------------------------------------
//    //PARALLAX
    void removeParallaxNode(LHParallaxNode* node, bool removeSprites = false);
    void removeAllParallaxes(bool removeSprites = false); //does not remove the sprites
//    //------------------------------------------------------------------------------
//    //------------------------------------------------------------------------------

    //GRAVITY
    bool isGravityZero(void);
    void createGravity(b2World* world);
    //------------------------------------------------------------------------------
//    //PHYSIC BOUNDARIES
    void createPhysicBoundaries(b2World* _world);

    //this method should be used when using dontStretchArtOnIpad
    //see api documentatin for more info
    void createPhysicBoundariesNoStretching(b2World * _world);

    CCRect physicBoundariesRect(void);
    bool hasPhysicBoundaries(void);

    b2Body* leftPhysicBoundary(void);
    LHNode* leftPhysicBoundaryNode(void);
    b2Body* rightPhysicBoundary(void);
    LHNode* rightPhysicBoundaryNode(void);
    b2Body* topPhysicBoundary(void);
    LHNode* topPhysicBoundaryNode(void);
    b2Body* bottomPhysicBoundary(void);
    LHNode* bottomPhysicBoundaryNode(void);
    void removePhysicBoundaries(void);
    
//    //------------------------------------------------------------------------------
//    //LEVEL INFO
    CCSize gameScreenSize(void); //the device size set in loaded level
    CCRect gameWorldSize(void); //the size of the game world
//    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    //PHYSICS
    static void setMeterRatio(float ratio); //default is 32.0f
    static float meterRatio(void); //same as pointsToMeterRatio - provided for simplicity as static method
    
    static float pixelsToMeterRatio(void);
    static float pointsToMeterRatio(void);
    
    static b2Vec2 pixelToMeters(CCPoint point); //Cocos2d point to Box2d point
    static b2Vec2 pointsToMeters(CCPoint point); //Cocos2d point to Box2d point
    
    static CCPoint metersToPoints(b2Vec2 vec); //Box2d point to Cocos2d point
    static CCPoint metersToPixels(b2Vec2 vec); //Box2d point to Cocos2d pixels
    
private:
    
    void initObjects(void);
    
    friend class LHJoint;
    friend class LHSprite;
    friend class LHBezier;
    friend class LHLayer;
    

    
    void createAllNodes();
    void createAllJoints();
    
    LHParallaxNode*  parallaxNodeFromDictionary(LHDictionary* parallaxDict, CCLayer*layer);
    void createParallaxes();
    void startAllPaths();

    b2World* getPhysicsWorld();
    void removeMainLayer();
    
    void removeJoint(LHJoint* jt);
    void callLoadingProgressObserverWithValue(float val);
    
    void loadLevelHelperSceneFile(const char* levelFile,
                                  const char* subfolder, 
                                  const char* imgFolder);

    void loadLevelHelperSceneFromDictionary(const LHDictionary& levelDictionary,
                                            const std::string& imgFolder);
        
    void processLevelFileFromDictionary(LHDictionary* dictionary);

    static void setTouchDispatcherForBezierWithTag(LHBezier* object, int tag);
    static void setTouchDispatcherForSpriteWithTag(LHSprite* object, int tag);
    static void removeTouchDispatcherFromSprite(LHSprite* object);
    static void removeTouchDispatcherFromBezier(LHBezier* object);
    
    b2Body* physicBoundarieForKey(const std::string& key);
    void setFixtureDefPropertiesFromDictionary(LHDictionary* spritePhysic, b2FixtureDef* shapeDef);
    void createPhysicBoundariesHelper(b2World* _world,
                                      const CCPoint& wbConv,
                                      const CCPoint& pos_offset);

};

#endif

