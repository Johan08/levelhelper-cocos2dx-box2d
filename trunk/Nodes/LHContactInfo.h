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
#ifndef __LH_CONTACT_INFO__
#define __LH_CONTACT_INFO__

#include "cocos2d.h"
#include "Box2d/Box2D.h"


class LHSprite;
class LHBezierNode;

enum LH_CONTACT_TYPE
{
    LH_BEGIN_CONTACT = 1,
    LH_END_CONTACT = 0,
    LH_PRE_SOLVE_CONTACT = -1,
    LH_POST_SOLVE_CONTACT = -2
};

using namespace cocos2d;

class LHContactInfo : public CCObject
{
public:
    b2Body* bodyA; //week ptr;
    b2Body* bodyB; //week ptr;
    enum LH_CONTACT_TYPE contactType; //the type of contact that was triggerd
    b2Contact* contact; //available at both pre and post solve
    const b2Manifold* oldManifold;//available at pre solve - else is nil
    const b2ContactImpulse* impulse; //available at post solve - else is nil
    

    LHSprite* spriteA();//may return nil;
    LHSprite* spriteB();//may return nil;

    LHBezierNode* bezierA(void); //may return nil
    LHBezierNode* bezierB(void); //may return nil
    
    LHContactInfo(void);
    virtual ~LHContactInfo(void);
    
    bool initWithInfo(b2Body* bodyA, 
                      b2Body* bodyB,
                      b2Contact* _contact,
                      int contactType,
                      const b2Manifold* _manifold,
                      const b2ContactImpulse* _impulse);
    
    static LHContactInfo* contactInfo(b2Body* bodyA,
                                      b2Body* bodyB,
                                      b2Contact* _contact,
                                      int contactType,
                                      const b2Manifold* _manifold,
                                      const b2ContactImpulse* _impulse);
};

#endif
