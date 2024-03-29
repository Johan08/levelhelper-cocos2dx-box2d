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

#ifndef __LH_JOINT__
#define __LH_JOINT__

#include "cocos2d.h"
#include "Box2d/Box2D.h"

using namespace cocos2d;

class LHSprite;

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
private:
    std::string uniqueName;
    std::map<std::string, void*> customUserValues;

    bool removeJointFromWorld(void);

    //static int numberOfJoints;
    
    b2Joint* joint; //week ptr
    int tag;
    LH_JOINT_TYPE type;
    
    bool shouldDestroyJointOnDealloc;
    friend class LHSprite;
public:
    b2Joint* getJoint(void){return joint;}//box2d joint 
    int getTag(void){return tag;}
    LH_JOINT_TYPE getType(void){return type;}//type of the joint in order to know how to convert the b2Joint
    
    std::string& getUniqueName(void){return uniqueName;}
    
    LHJoint(int _tag, LH_JOINT_TYPE _type, b2Joint* _boxJoint);
    virtual ~LHJoint(void);
    
    bool initWithUniqueName(const char* name);
    static LHJoint* jointWithUniqueName(const char* name, int _tag, LH_JOINT_TYPE _type, b2Joint* _boxJoint);

    //setting a custom value will require that you release the value 
    void setCustomValueWithKey(void* value, const std::string& key);
    void* customValueWithKey(const char* key);
    
    LHSprite* getSpriteA(void);
    LHSprite* getSpriteB(void);
    
    static LHJoint* jointFromBox2dJoint(b2Joint* jt);    
    static bool isLHJoint(CCObject* obj);
};

////////////////////////////////////////////////////////////////////////////////
#endif
