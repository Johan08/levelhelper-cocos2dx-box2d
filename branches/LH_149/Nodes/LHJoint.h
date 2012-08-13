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

#ifndef __LHJOINT_NODE__
#define __LHJOINT_NODE__

#include "cocos2d.h"
#include "Box2d/Box2D.h"

using namespace cocos2d;

class LHSprite;
class LevelHelperLoader;
class LHDictionary;

enum LH_JOINT_TYPE
{
	LH_DISTANCE_JOINT = 0,
	LH_REVOLUTE_JOINT,
	LH_PRISMATIC_JOINT,
	LH_PULLEY_JOINT,
	LH_GEAR_JOINT,
	LH_WHEEL_JOINT,
	LH_WELD_JOINT,
	LH_ROPE_JOINT,
	LH_FRICTION_JOINT,
    LH_UNKNOWN_TYPE
};

class LHJoint : public CCObject
{
public:;
    
    virtual bool initWithDictionary(LHDictionary* dictionary, b2World* box2d, LevelHelperLoader* pLoader);
    virtual ~LHJoint();
    LHJoint();

    static LHJoint* jointWithDictionary(LHDictionary* dictionary, b2World* box2d, LevelHelperLoader* pLoader);
    void removeSelf();
    
    
    const std::string& getUniqueName(){return uniqueName;};
    enum LH_JOINT_TYPE getType(){return type;}
    
    int getTag(){return tag;}
    void setTag(int t){ tag = t;}
    
    bool getShouldDestroyJointOnDealloc(){return shouldDestroyJointOnDealloc;}
    void setShouldDestroyJointOnDealloc(bool val){shouldDestroyJointOnDealloc = val;}

    b2Joint* getJoint(){return joint;}

    LHSprite* getSpriteA();
    LHSprite* getSpriteB();

    
    static bool isLHJoint(CCNode* object);
    static LHJoint* jointFromBox2dJoint(b2Joint* jt);
    static int tagFromBox2dJoint(b2Joint* joint);
    static std::string uniqueNameFromBox2dJoint(b2Joint* joint);
    
    //this method will return LH_UNKNOWN_TYPE if fail
    static enum LH_JOINT_TYPE typeFromBox2dJoint(b2Joint* joint);

private:
    
    friend class LHSprite;
    
	b2Joint*    joint; //week ptr
    b2World*    boxWorld;
    std::string uniqueName;
    int         tag;
    LH_JOINT_TYPE type;
    
    LevelHelperLoader* parentLoader;
    
    bool shouldDestroyJointOnDealloc;
    
    bool removeJointFromWorld();
    
    void removeJoint(LHJoint* jt);
    
    void createBox2dJointFromDictionary(LHDictionary* dictionary);
};

#endif
////////////////////////////////////////////////////////////////////////////////