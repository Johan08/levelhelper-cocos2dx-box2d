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

#ifndef __LHANIMATION_NODE__
#define __LHANIMATION_NODE__

#include "cocos2d.h"
using namespace cocos2d;
//notifications
#define LHAnimationHasEndedAllRepetitionsNotification "LHAnimationHasEndedAllRepetitionsNotification"
#define LHAnimationHasEndedNotification "LHAnimationHasEndedNotification"
#define LHAnimationFrameNotification "LHAnimationFrameNotification" //frame notifications are only triggered on frames with user data

//user info keys
#define LHAnimationSpriteObject "LHAnimationSpriteObject"
#define LHAnimationUserInfo "LHAnimationUserInfo"
#define LHAnimationFrameName "LHAnimationFrameName"

class LHSprite;
class LHBatch;
class LHDictionary;


class LHAnimationFrameInfo : public CCObject
{
    
public:
    
    virtual bool initWithDictionary(LHDictionary* dictionary, LHSprite* sprite);
    virtual ~LHAnimationFrameInfo();
    LHAnimationFrameInfo();


    static LHAnimationFrameInfo* frameWithDictionary(LHDictionary* dictionary, LHSprite* sprite);
    
    float   getDelayPerUnit(){return delayPerUnit;}
    CCPoint getOffset(){return offset;}
    
    LHDictionary*       getNotifications(){return notifications;}
    const std::string&  getSpriteFrameName(){return spriteframeName;}

    CCRect  getRect(){return rect;}
    bool    getRectIsRotated(){return rectIsRotated;}
    CCPoint getSpriteFrameOffset(){return spriteFrameOffset;}
    CCSize  getSpriteFrameSize(){return spriteFrameSize;}
    
private:
    float           delayPerUnit;
    CCPoint         offset;
    LHDictionary*   notifications;
    std::string     spriteframeName;
    CCRect          rect;
    bool            rectIsRotated;
    CCPoint         spriteFrameOffset;
    CCSize          spriteFrameSize;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class LHAnimationNode : public CCObject
{
    
public:
    
//    virtual bool initWithDictionary(LHDictionary* dictionary, LHSprite* sprite);
    virtual ~LHAnimationNode();
    LHAnimationNode(LHDictionary* dic, LHSprite* sprite, std::string shScene);
    
//    static LHAnimationNode* animationWithDictionary(LHDictionary* dic, LHSprite* sprite);

    
    const std::string& getSHSceneName(){return shSceneName;}
    const std::string& getUniqueName(){return uniqueName;}
    const std::string& getSheetName(){return sheetName;}

    float   getDelayPerUnit(){return delayPerUnit;}
    void    setDelayPerUnit(const float& f){delayPerUnit = f;}
    
    bool    getLoop(){return loop;}
    void    setLoop(const bool& l){loop = l;}

    int     getRepetitions(){return repetitions;}
    void    setRepetitions(const int& r){repetitions = r;}
    
    bool    getRestoreOriginalFrame(){return restoreOriginalFrame;}
    void    setRestoreOriginalFrame(const bool& b){restoreOriginalFrame = b;}
    
    LHSprite* getSprite(){return sprite;}
    
    bool    getPaused(){return paused;}
    void    setPaused(const bool& p){paused = p;}
    
    LHDictionary* getUserDataForCurrentFrame();
    
    void prepare();//sets first frame of the animation as texture of the sprite
    void play();
    void restart();
    
    int     getNumberOfFrames();
    void    setFrame(int frm);
    int     getCurrentFrame();
    
    void nextFrame();
    void prevFrame();
    void nextFrameAndRepeat();
    void prevFrameAndRepeat();
    bool isAtLastFrame();
    
    void update(float dt);
    
    void restoreFrame();//only restore the sprites frame if restoreOriginaFrame is set
    ////////////////////////////////////////////////////////////////////////////////
    
    void setOldBatch(LHBatch* b);
    void setOldTexture(CCTexture2D* tex);
    void setOldRect(CCRect r);
    
    float totalTime();
    
private:
    
    std::string shSceneName;
    std::string uniqueName;
    std::string sheetName;
    
    float           delayPerUnit;
    CCArray*        frames;//array of LHAnimationFrameInfo;
    bool            loop;
    int             repetitions;
    int             repetitionsPerformed;
    bool            restoreOriginalFrame;
    
    LHSprite*       sprite;//the sprite on which this animation obj is assigned to    
    
    int             currentFrame;
    float           elapsedFrameTime;
    LHAnimationFrameInfo* activeFrame;
    
    bool            paused;
    
    LHBatch*        oldBatch;
    CCTexture2D*    oldTexture;
    CCRect          oldRect;
    
    void setActiveFrameTexture();
};
#endif
