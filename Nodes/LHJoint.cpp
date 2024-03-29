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
#include "LHJoint.h"
#include "LHSettings.h"
#include "LHSprite.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//int LHJoint::numberOfJoints = 0;
LHJoint::LHJoint(int _tag, LH_JOINT_TYPE _type, b2Joint* _boxJoint){
    joint = _boxJoint;
    tag = _tag;
    type = _type;   
    shouldDestroyJointOnDealloc = true;
    //numberOfJoints++;
}
LHJoint::~LHJoint(void){
    
    //CCLog("LHJoint dealloc %d", --numberOfJoints);
    if(shouldDestroyJointOnDealloc)
        removeJointFromWorld();
}

bool LHJoint::initWithUniqueName(const char* name){
    uniqueName = std::string(name);
    return true;
}

LHJoint* LHJoint::jointWithUniqueName(const char* name, 
                                      int _tag, 
                                      LH_JOINT_TYPE _type, 
                                      b2Joint* _boxJoint){
    LHJoint *pobJoint = new LHJoint(_tag, _type, _boxJoint);
	if (pobJoint && pobJoint->initWithUniqueName(name))
    {
	    pobJoint->autorelease();
        return pobJoint;
    }
    CC_SAFE_DELETE(pobJoint);
	return NULL;
}

void LHJoint::setCustomValueWithKey(void* value, const std::string& key){
    customUserValues[key] = value;
}
void* LHJoint::customValueWithKey(const char* key){
    return customUserValues[std::string(key)];
}
LHSprite* LHJoint::getSpriteA(void){    
    if(joint)
        return LHSprite::spriteForBody(joint->GetBodyA());
    return NULL;
}
LHSprite* LHJoint::getSpriteB(void){
    if(joint)
        return LHSprite::spriteForBody(joint->GetBodyB());
    return NULL;
}
////////////////////////////////////////////////////////////////////////////////
bool LHJoint::removeJointFromWorld(void){
    
    if(0 != joint)
	{
        b2Body *body = joint->GetBodyA();
        
        if(0 == body)
        {
            body = joint->GetBodyB();
            
            if(0 == body)
                return false;
        }
        
        b2World* _world = body->GetWorld();
        
        if(0 == _world)
            return false;
        
        _world->DestroyJoint(joint);
        return true;
	}
    return false;
}
////////////////////////////////////////////////////////////////////////////////
bool LHJoint::isLHJoint(CCObject* obj){

    if( 0 != dynamic_cast<LHJoint*>(obj))
        return true;
    
    return false;
}
//------------------------------------------------------------------------------
LHJoint* LHJoint::jointFromBox2dJoint(b2Joint* jt){
    
    if(jt == NULL)
        return NULL;
    
    CCObject* lhJt = (CCObject*)jt->GetUserData();
    
    if(LHJoint::isLHJoint(lhJt)){
        return (LHJoint*)lhJt;
    }

    return NULL;    
}