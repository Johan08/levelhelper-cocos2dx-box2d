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
#include "LHContactInfo.h"
#include "../LevelHelperLoader.h"
#include "LHSettings.h"
#include "LHBezierNode.h"
#include "LHSprite.h"
////////////////////////////////////////////////////////////////////////////////

LHContactInfo::LHContactInfo(void){
    
}
LHContactInfo::~LHContactInfo(void){
   // CCLog("Contact Info dealloc");
}

////////////////////////////////////////////////////////////////////////////////
bool LHContactInfo::initWithInfo(b2Body* _bodyA, 
                                 b2Body* _bodyB,
                                 b2Contact* _contact,
                                 int ctype,
                                 const b2Manifold* _manifold,
                                 const b2ContactImpulse* _impulse){
    
    bodyA = _bodyA;
    bodyB = _bodyB;
    contact = _contact;
    oldManifold = _manifold;
    impulse = _impulse;
    contactType = (LH_CONTACT_TYPE)ctype;
    return true;
}

LHContactInfo* LHContactInfo::contactInfo(b2Body* _bodyA,
                                          b2Body* _bodyB,
                                          b2Contact* _contact,
                                          int ctype,
                                          const b2Manifold* _manifold,
                                          const b2ContactImpulse* _impulse){
    
    LHContactInfo *pobContact = new LHContactInfo();
	if (pobContact && pobContact->initWithInfo(_bodyA, _bodyB, _contact, 
                                               ctype, _manifold, _impulse)){
	    pobContact->autorelease();
        return pobContact;
    }
    CC_SAFE_DELETE(pobContact);
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
LHSprite* LHContactInfo::spriteA(void){
    
    CCNode* spr = (CCNode*)bodyA->GetUserData();
    if(0 != spr){
        if(LHSprite::isLHSprite(spr)){
            return (LHSprite*)spr;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
LHSprite* LHContactInfo::spriteB(void){
    CCNode* spr = (CCNode*)bodyB->GetUserData();
    if(0 != spr){
        if(LHSprite::isLHSprite(spr)){
            return (LHSprite*)spr;
        }
    }
    return 0;    
}
////////////////////////////////////////////////////////////////////////////////
LHBezierNode* LHContactInfo::bezierA(void){
    
    CCNode* node = (CCNode*)bodyA->GetUserData();
    if(0 != node){
        if(LHBezierNode::isLHBezierNode(node)){
            return (LHBezierNode*)node;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
LHBezierNode* LHContactInfo::bezierB(void){
    
    CCNode* node = (CCNode*)bodyB->GetUserData();
    if(0 != node){
        if(LHBezierNode::isLHBezierNode(node)){
            return (LHBezierNode*)node;
        }
    }
    return 0;
}