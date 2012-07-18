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

#ifndef __LHSPRITE_NODE__
#define __LHSPRITE_NODE__

#include "cocos2d.h"
#include "Box2d/Box2D.h"
#include "LHTouchMgr.h"
#include "LHPathNode.h"
using namespace cocos2d;

class LevelHelperLoader;
class LHAnimationNode;
class LHParallaxNode;
class LHJoint;
class LHBatch;
class LHBezier;
class LHDictionary;
class LHArray;
class LHAbstractClass;

class LHSprite : public CCSprite, public CCStandardTouchDelegate
{
    
public:
    virtual ~LHSprite(void);
	virtual void removeSelf();

    LHSprite();    
    
    virtual bool initBatchSpriteWithDictionary(LHDictionary* dictionary, LHBatch* batch);
    virtual bool initWithDictionary(LHDictionary* dictionary);
    
    //functions to be used by the user
    static LHSprite* batchSpriteWithDictionary(LHDictionary* dictionary, LHBatch* batch);//render by batch node
    static LHSprite* spriteWithDictionary(LHDictionary* dictionary);//self render
    
    static LHSprite* spriteWithName(const std::string& spriteName, const std::string& sheetName, const std::string& spriteHelperFile);
    static LHSprite* batchSpriteWithName(const std::string& spriteName, LHBatch* batch);
    virtual void postInit(void){}; 

    
    
    
    
protected:
	friend class LevelHelperLoader;
    
    b2Body* body; //week ptr
    CCArray* fixturesObj;
    LHArray* fixturesInfo;
    
    std::string uniqueName;
    std::string shSceneName;
    std::string shSheetName;
    std::string shSpriteName;
    
    std::string imageFile;
    CCRect originalRect;
    
    LHAnimationNode* animation;
    LHPathNode* pathNode;
    LH_PATH_DEFAULTS pathDefaults;
    
    LevelHelperLoader* parentLoader;
    
    CCSize realScale;
    
    LHParallaxNode* parallaxFollowingThisSprite;
    LHParallaxNode* spriteIsInParallax;
    
        
    LHObserverPair touchBeginObserver;
    LHObserverPair touchMovedObserver;
    LHObserverPair touchEndedObserver;
    
    LHObserverPair* tagTouchBeginObserver;
    LHObserverPair* tagTouchMovedObserver;
    LHObserverPair* tagTouchEndedObserver;

    bool swallowTouches;
    
    
    bool usesOverloadedTransformations;
    bool usePhysicsForTouches;
    
    LHAbstractClass* userCustomInfo;
    
public:
    //USEFUL INFO
    //--------------------------------------------------------------------------    
    CCSize  getRealScale(){return realScale;}
    void    setRealScale(CCSize scl){realScale = scl;}
    
    
    const std::string&  getUniqueName(void){return uniqueName;}
    void                setUniqueName(const std::string& str){uniqueName = str;}
    
    
    void    setBody(b2Body* b){body = b;}
    b2Body* getBody(){return body;}
    bool removeBodyFromWorld();
    
    
    const std::string&  getImageFile(){return imageFile;}
    void                setImageFile(const std::string& str){imageFile = str;}
    
    
    CCRect  getOriginalRect(){return originalRect;}
    void    setOrinalRect(CCRect rc){originalRect = rc;}
    
    const std::string& getShSceneName(){return shSceneName;}
    const std::string& getSheetName(){return shSheetName;}
    const std::string& getSpriteName(){return shSpriteName;}
    
    
    //TRANSFORMATIONS
    //--------------------------------------------------------------------------
    bool getUsesOverloadedTransformations(){return usesOverloadedTransformations;}
    void setUsesOverloadedTransformations(bool v){usesOverloadedTransformations = v;}
    
    void transformPosition(CCPoint pos);
    void transformRotation(float rot);

    void transformScale(float scale);
    void transformScaleX(float sclX);
    void transformScaleY(float sclY);
    
    //ANIMATIONS
    //--------------------------------------------------------------------------    
    void prepareAnimationNamed(const std::string& animName, const std::string& shScene);
    
    void playAnimation();
    void pauseAnimation();
    void restartAnimation();
    void stopAnimation(); //removes the animation entirely
    
    bool isAnimationPaused();
    
    std::string animationName();
    int numberOfFrames();
    int currentFrame();
    
    float animationDelayPerUnit();
    void setAnimationDelayPerUnit(float d);
    
    std::vector<std::string>    getCurrentFrameDataKeys(); //if no data info -returns empty vector
    float                       getCurrentFrameFloatDataForKey(const std::string& key); //if no data or wrong key - returns -1.0f
    std::string                 getCurrentFrameStringDataForKey(const std::string& key); //if no data or wrong key - returns empty string
    bool                        getCurrentFrameBoolDataForKey(const std::string& key); //if no data or wrong key returns false
    bool                        isCurrentFrameValueForKeyFloat(const std::string& key);
    bool                        isCurrentFrameValueForKeyString(const std::string& key);
    bool                        isCurrentFrameValueForKeyBool(const std::string& key);
    
    
    float animationDuration();//return how much time will take for a loop to complete
    
    void setFrame(int frmNo);
    void nextFrame();
    void prevFrame();
    
    void nextFrameAndRepeat(); //will loop when it reaches end
    void prevFrameAndRepeat(); //will loop when it reaches start
    
    bool isAtLastFrame();

    //USER DATA
    //--------------------------------------------------------------------------    
    //will return "No Class" if no class is defined 
    //will return the class name if a class is assigned to this sprite
    std::string userInfoClassName();

    //this will return an instance of the class defined in LH under Custom Class Properties
    //check for NULL to see if you have any info
    //use the class properties to read all your info 
    //e.g MyClass* myInfo = (MyClass*)sprite->userInfo();  if(myInfo){ int life = myInfo.life); } 
    
    //use the class properties to set new (other then the one set in LH) values
    //e.g MyClass* myInfo = (MyClass*)sprite->userInfo(); if(myInfo){ myInfo.life = 40; } )
    void* userInfo(); 
    
    //JOINTS
    //--------------------------------------------------------------------------        
    CCArray* jointList();
    LHJoint* jointWithUniqueName(const std::string& name);
    bool removeAllAttachedJoints();
    
    
    
    //PATH MOVEMENT
    //--------------------------------------------------------------------------            
    void prepareMovementOnPathWithUniqueName(const std::string& pathName);
    
    const std::string& pathUniqueName();
    
    void startPathMovement();
    void pausePathMovement();
    void restartPathMovement();
    void stopPathMovement(); //removes the path movement;
    
    void setPathMovementSpeed(float value);
    float pathMovementSpeed();
    
    void setPathMovementStartPoint(enum LH_PATH_MOVEMENT_START_POINT point);
    enum LH_PATH_MOVEMENT_START_POINT pathMovementStartPoint();
    
    void setPathMovementIsCyclic(bool cyclic);
    bool pathMovementIsCyclic();
    
    void setPathMovementRestartsAtOtherEnd(bool otherEnd);
    bool pathMovementRestartsAtOtherEnd();
    
    void setPathMovementOrientation(enum LH_PATH_MOVEMENT_ORIENTATION point);
    enum LH_PATH_MOVEMENT_ORIENTATION pathMovementOrientation();
    
    void setPathMovementFlipXAtEnd(bool flip);
    bool pathMovementFlipXAtEnd();
    
    void setPathMovementFlipYAtEnd(bool flip);
    bool pathMovementFlipYAtEnd();
    
    void setPathMovementRelative(bool rel);
    bool pathMovementRelative();
    

    
    //PARALLAX
    //--------------------------------------------------------------------------
    LHParallaxNode* getParallaxNode(void){return spriteIsInParallax;}

    

    //TOUCH HANDLING
    //--------------------------------------------------------------------------            
    bool isTouchedAtPoint(CCPoint point);
    
    void setUsePhysicsForTouches(bool val){usePhysicsForTouches = val;}
    bool getUsePhysicsForTouches(){return usePhysicsForTouches;}
        
    void registerTouchBeginObserver(CCObject* observer, SEL_CallFuncO selector);
    void registerTouchMovedObserver(CCObject* observer, SEL_CallFuncO selector);
    void registerTouchEndedObserver(CCObject* observer, SEL_CallFuncO selector);
    
    
    //Box2d helpful methods
    //--------------------------------------------------------------------------            
    static std::string  uniqueNameForBody(b2Body* body);
    static LHSprite*    spriteForBody(b2Body* body);
    static int          tagForBody(b2Body* body);
    static bool         isLHSprite(CCNode* obj);

    
    //Box2d COLLISION FILTER
    //--------------------------------------------------------------------------            
    void setCollisionFilterCategory(int category);
    void setCollisionFilterMask(int mask);
    void setCollisionFilterGroup(int group);

    //Box2d BODY TYPE TRANSFORMATION
    //--------------------------------------------------------------------------                
    void makeDynamic();
    void makeStatic();
    void makeKinematic();

        
    
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void touchDelegateRetain() {} //compatibility with old cocos2d-x version
    virtual void touchDelegateRelease() {}//compatibility with old cocos2d-x version

    virtual void update(float dt);
private:
    
    friend class LHBatch;
    friend class LHLayer;
    friend class LHParallaxNode;

    void createFixturesFromInfoOnBody();
    void loadPhysicalInformationFromDictionary(LHDictionary* dictionary);
    void loadAnimationsInformationFromDictionary(LHDictionary* dictionary);
    void loadPathMovementFromDictionary(LHDictionary* dictionary);
    void loadInformationFromDictionary(LHDictionary* dictionary);
    void loadUserCustomInfoFromDictionary(LHDictionary* dictionary);
    
    void setShSceneName(const std::string& scene){shSceneName = scene;}
    void setSheetName(const std::string& name){shSheetName = name;}
    void setSpriteName(const std::string& name){shSpriteName = name;}

    
        
    void setParallaxNode(LHParallaxNode*node){spriteIsInParallax = node;}
    
    void setTagTouchBeginObserver(LHObserverPair* pair);
    void setTagTouchMovedObserver(LHObserverPair* pair);
    void setTagTouchEndedObserver(LHObserverPair* pair);
    
    void setParentLoader(LevelHelperLoader* p){parentLoader = p;}
};
////////////////////////////////////////////////////////////////////////////////

#endif
