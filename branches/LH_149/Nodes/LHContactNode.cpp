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
#include "LHContactNode.h"
#include "../LevelHelperLoader.h"
#include "LHSettings.h"

class LHContactNodeInfo : public CCObject
{
	int tagB;
    CCObject* listenerId;
    SEL_CallFuncO listenerSel;  
public:
    LHContactNodeInfo(){}

    virtual ~LHContactNodeInfo(void){CCLog("ContactNodeInfo release");}
    
    bool initcontactInfoWithTag(int _tagB,
                                CCObject* listId,
                                SEL_CallFuncO  listSel){
        tagB = _tagB;
        listenerId = listId;
        listenerSel = listSel;
        return true;
    }
    
    static LHContactNodeInfo* contactInfoWithTag(int tagB,
                                                CCObject* listId,
                                                SEL_CallFuncO  listSel){
       LHContactNodeInfo *pobInfo = new LHContactNodeInfo();
       if (pobInfo && pobInfo->initcontactInfoWithTag(tagB, listId, listSel))
       {
           pobInfo->autorelease();
           return pobInfo;
       }
       CC_SAFE_DELETE(pobInfo);
       return NULL;
       
   }
    
    int getTagB(){return tagB;}
    
    void callListenerWithBodyA(b2Body* A, 
                               b2Body* B,
                               b2Fixture* fixA,
                               b2Fixture* fixB,
                               b2Contact* contact,
                               int contactType,
                               const b2Manifold* oldManifold,
                               const b2ContactImpulse* impulse){
        
        
        LHContactInfo* info = LHContactInfo::contactInfo(A, B, fixA,fixB, contact, contactType, oldManifold, impulse);
        
        if (listenerId) {
            (listenerId->*listenerSel)(info);
        }
    }

};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void lhContact_CallBeginEndSolveMethod(void* object, 
                                       b2Contact* contact, bool isBegin){
    ((LHContactNode*)object)->beginEndSolve(contact, isBegin);
}

void lhContact_CallPreSolveMethod(void* object, 
                                  b2Contact* contact, 
                                  const b2Manifold* oldManifold){
    ((LHContactNode*)object)->preSolve(contact, oldManifold);
}
////////////////////////////////////////////////////////////////////////////////
void lhContact_CallPostSolveMethod(void* object, 
                                   b2Contact* contact, 
                                   const b2ContactImpulse* impulse){
    ((LHContactNode*)object)->postSolve(contact, impulse);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
LHContactNode::LHContactNode(/*b2World* world*/){
    
    lhContactListener = NULL;    
}
LHContactNode::~LHContactNode(){

    CCLog("LHContactNode release");
        
    preCollisionMap.removeAllObjects();
    postCollisionMap.removeAllObjects();
    delete lhContactListener;
}
////////////////////////////////////////////////////////////////////////////////
bool LHContactNode::initContactNodeWithWorld(b2World* world){

    if(0 == world)
        return false;

    lhContactListener = new LHContactListener();
    world->SetContactListener(lhContactListener);
    
    lhContactListener->nodeObject = this;
    lhContactListener->beginEndSolveSelector = &lhContact_CallBeginEndSolveMethod;
    lhContactListener->preSolveSelector = &lhContact_CallPreSolveMethod;
    lhContactListener->postSolveSelector = &lhContact_CallPostSolveMethod;

    return true;
}
LHContactNode* LHContactNode::contactNodeWithWorld(b2World* world){
    
    LHContactNode *pobNode = new LHContactNode();
    if (pobNode && pobNode->initContactNodeWithWorld(world))
    {
        pobNode->autorelease();
        return pobNode;
    }
    CC_SAFE_DELETE(pobNode);
    return NULL;
}
////////////////////////////////////////////////////////////////////////////////
void LHContactNode::registerBeginOrEndCollisionCallbackBetweenTagA(int tagA, 
                                                                   int tagB,
                                                                   CCObject* obj, 
                                                                   SEL_CallFuncO sel){
    
#if COCOS2D_VERSION >= 0x00020000
    CCDictionary* tableA = (CCDictionary*)beginEndCollisionMap.objectForKey(tagA);
    
    if(tableA == NULL){
        LHContactNodeInfo* info = LHContactNodeInfo::contactInfoWithTag(tagB, obj, sel);
        
        CCDictionary tempDict;
        CCDictionary* map = CCDictionary::create(&tempDict);
        map->setObject(info, tagB);
        beginEndCollisionMap.setObject(map, tagA);
    }
    
#else
    
    CCMutableDictionary<int>* tableA = (CCMutableDictionary<int>*)beginEndCollisionMap.objectForKey(tagA);
    
    if(tableA == NULL){
        LHContactNodeInfo* info = LHContactNodeInfo::contactInfoWithTag(tagB, obj, sel);
        
        CCMutableDictionary<int> tempDict;
        CCMutableDictionary<int>* map = CCMutableDictionary<int>::dictionaryWithDictionary(&tempDict);
        map->setObject(info, tagB);
        beginEndCollisionMap.setObject(map, tagA);
    }
    
#endif
    
    else{
        LHContactNodeInfo* info = LHContactNodeInfo::contactInfoWithTag(tagB, obj, sel);
        tableA->setObject(info, tagB);
    }
}
//------------------------------------------------------------------------------
void LHContactNode::cancelBeginOrEndCollisionCallbackBetweenTagA(int tagA,
                                                                 int tagB){
    
#if COCOS2D_VERSION >= 0x00020000
    CCDictionary* tableA = (CCDictionary*)beginEndCollisionMap.objectForKey(tagA);
#else
    CCMutableDictionary<int>* tableA = (CCMutableDictionary<int>*)beginEndCollisionMap.objectForKey(tagA);
#endif

    if(NULL != tableA){
        tableA->removeObjectForKey(tagB);
    }
}
//------------------------------------------------------------------------------
void LHContactNode::registerPreCollisionCallbackBetweenTagA(int tagA, 
                                                           int tagB,
                                                           CCObject* obj, 
                                                           SEL_CallFuncO sel){
    
#if COCOS2D_VERSION >= 0x00020000
    CCDictionary* tableA = (CCDictionary*)preCollisionMap.objectForKey(tagA);
    
    if(tableA == NULL){
        LHContactNodeInfo* info = LHContactNodeInfo::contactInfoWithTag(tagB, obj, sel);
        
        CCDictionary tempDict;
        CCDictionary* map = CCDictionary::create(&tempDict);
        map->setObject(info, tagB);
        preCollisionMap.setObject(map, tagA);
    }
    
#else
    CCMutableDictionary<int>* tableA = (CCMutableDictionary<int>*)preCollisionMap.objectForKey(tagA);
    
    if(tableA == NULL){
        LHContactNodeInfo* info = LHContactNodeInfo::contactInfoWithTag(tagB, obj, sel);
        
        CCMutableDictionary<int> tempDict;
        CCMutableDictionary<int>* map = CCMutableDictionary<int>::dictionaryWithDictionary(&tempDict);
        map->setObject(info, tagB);
        preCollisionMap.setObject(map, tagA);
    }
    
#endif
    
    else{
        LHContactNodeInfo* info = LHContactNodeInfo::contactInfoWithTag(tagB, obj, sel);
        tableA->setObject(info, tagB);
    }   
}
////////////////////////////////////////////////////////////////////////////////
void LHContactNode::cancelPreCollisionCallbackBetweenTagA(int tagA,
                                                         int tagB){
#if COCOS2D_VERSION >= 0x00020000
    CCDictionary* tableA = (CCDictionary*)preCollisionMap.objectForKey(tagA);
#else
    CCMutableDictionary<int>* tableA = (CCMutableDictionary<int>*)preCollisionMap.objectForKey(tagA);
#endif
    
    if(NULL != tableA){
        tableA->removeObjectForKey(tagB);
    }
}
////////////////////////////////////////////////////////////////////////////////
void LHContactNode::registerPostCollisionCallbackBetweenTagA(int tagA,
                                                            int tagB,
                                                            CCObject* obj, 
                                                            SEL_CallFuncO sel){
    
#if COCOS2D_VERSION >= 0x00020000
    CCDictionary* tableA = (CCDictionary*)postCollisionMap.objectForKey(tagA);
    
    if(tableA == NULL){
        LHContactNodeInfo* info = LHContactNodeInfo::contactInfoWithTag(tagB, obj, sel);
        
        CCDictionary tempDict;
        CCDictionary* map = CCDictionary::create(&tempDict);
        map->setObject(info, tagB);
        postCollisionMap.setObject(map, tagA);
    }
    
#else
    CCMutableDictionary<int>* tableA = (CCMutableDictionary<int>*)postCollisionMap.objectForKey(tagA);
    
    if(tableA == NULL){
        LHContactNodeInfo* info = LHContactNodeInfo::contactInfoWithTag(tagB, obj, sel);
        
        CCMutableDictionary<int> tempDict;
        CCMutableDictionary<int>* map = CCMutableDictionary<int>::dictionaryWithDictionary(&tempDict);
        map->setObject(info, tagB);
        postCollisionMap.setObject(map, tagA);
    }
    
#endif
    
    else{
        LHContactNodeInfo* info = LHContactNodeInfo::contactInfoWithTag(tagB, obj, sel);
        tableA->setObject(info, tagB);
    }   
}
////////////////////////////////////////////////////////////////////////////////
void LHContactNode::cancelPostCollisionCallbackBetweenTagA(int tagA,
                                                          int tagB){
    
#if COCOS2D_VERSION >= 0x00020000
    CCDictionary* tableA = (CCDictionary*)postCollisionMap.objectForKey(tagA);
#else
    CCMutableDictionary<int>* tableA = (CCMutableDictionary<int>*)postCollisionMap.objectForKey(tagA);
#endif
    if(NULL != tableA){
        tableA->removeObjectForKey(tagB);
    }
}
////////////////////////////////////////////////////////////////////////////////
void LHContactNode::beginEndSolve(b2Contact* contact, bool isBegin){
    
    b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();
    
    CCNode* nodeA = (CCNode*)bodyA->GetUserData();
    CCNode* nodeB = (CCNode*)bodyB->GetUserData();

    if(NULL == nodeA || NULL == nodeB){
        return;
    }
#if COCOS2D_VERSION >= 0x00020000
    
    CCDictionary* info = (CCDictionary*)beginEndCollisionMap.objectForKey(nodeA->getTag());
    bool foundA = false;
    if(info != NULL){
        LHContactNodeInfo* contactInfo = (LHContactNodeInfo*)info->objectForKey(nodeB->getTag());
        if(NULL != contactInfo){
            foundA = true;
            contactInfo->callListenerWithBodyA(bodyA,bodyB, contact->GetFixtureA(), contact->GetFixtureB(), contact, isBegin, 0,0);
        }
    }
    if(!foundA){
        info = (CCDictionary*)beginEndCollisionMap.objectForKey(nodeB->getTag());
        if(NULL != info){
            LHContactNodeInfo* contactInfo = (LHContactNodeInfo*)info->objectForKey(nodeA->getTag());
            if(NULL != contactInfo){
                contactInfo->callListenerWithBodyA(bodyB, bodyA, contact->GetFixtureB(), contact->GetFixtureA(), contact, isBegin, 0, 0);
            }
        }        
    }
    
#else
    
    CCMutableDictionary<int>* info = (CCMutableDictionary<int>*)beginEndCollisionMap.objectForKey(nodeA->getTag());
    bool foundA = false;
    if(info != NULL){
        LHContactNodeInfo* contactInfo = (LHContactNodeInfo*)info->objectForKey(nodeB->getTag());
        if(NULL != contactInfo){
            foundA = true;
            contactInfo->callListenerWithBodyA(bodyA,bodyB, contact->GetFixtureA(), contact->GetFixtureB(), contact, isBegin, 0,0);
        }
    }
    if(!foundA){
        info = (CCMutableDictionary<int>*)beginEndCollisionMap.objectForKey(nodeB->getTag());
        if(NULL != info){
            LHContactNodeInfo* contactInfo = (LHContactNodeInfo*)info->objectForKey(nodeA->getTag());
            if(NULL != contactInfo){
                contactInfo->callListenerWithBodyA(bodyB, bodyA, contact->GetFixtureB(), contact->GetFixtureA(), contact, isBegin, 0, 0);
            }
        }        
    }
    
#endif    
}
////////////////////////////////////////////////////////////////////////////////
void LHContactNode::preSolve(b2Contact* contact,                     
                             const b2Manifold* oldManifold)
{
    b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();
	
    
    CCNode* nodeA = (CCNode*)bodyA->GetUserData();
    CCNode* nodeB = (CCNode*)bodyB->GetUserData();
      
    if(NULL == nodeA || NULL == nodeB)
        return;
    
#if COCOS2D_VERSION >= 0x00020000
    
    CCDictionary* info = (CCDictionary*)preCollisionMap.objectForKey(nodeA->getTag());
    
    bool foundA = false;
    if(info != NULL){
        LHContactNodeInfo* contactInfo = (LHContactNodeInfo*)info->objectForKey(nodeB->getTag());
        if(NULL != contactInfo){
            foundA = true;
            contactInfo->callListenerWithBodyA(bodyA,bodyB, contact->GetFixtureA(), contact->GetFixtureB(), contact,LH_PRE_SOLVE_CONTACT, oldManifold,0);
        }
    }
    if(!foundA){
        info = (CCDictionary*)preCollisionMap.objectForKey(nodeB->getTag());
        if(NULL != info){
            LHContactNodeInfo* contactInfo = (LHContactNodeInfo*)info->objectForKey(nodeA->getTag());
            if(NULL != contactInfo){
                contactInfo->callListenerWithBodyA(bodyB, bodyA, contact->GetFixtureB(), contact->GetFixtureA(), contact, LH_PRE_SOLVE_CONTACT, oldManifold, 0);
            }
        }        
    }
    
#else
    
    
    CCMutableDictionary<int>* info = (CCMutableDictionary<int>*)preCollisionMap.objectForKey(nodeA->getTag());
    
    bool foundA = false;
    if(info != NULL){
        LHContactNodeInfo* contactInfo = (LHContactNodeInfo*)info->objectForKey(nodeB->getTag());
        if(NULL != contactInfo){
            foundA = true;
            contactInfo->callListenerWithBodyA(bodyA,bodyB, contact->GetFixtureA(), contact->GetFixtureB(), contact,LH_PRE_SOLVE_CONTACT, oldManifold,0);
        }
    }
    if(!foundA){
        info = (CCMutableDictionary<int>*)preCollisionMap.objectForKey(nodeB->getTag());
        if(NULL != info){
            LHContactNodeInfo* contactInfo = (LHContactNodeInfo*)info->objectForKey(nodeA->getTag());
            if(NULL != contactInfo){
                contactInfo->callListenerWithBodyA(bodyB, bodyA, contact->GetFixtureB(), contact->GetFixtureA(), contact, LH_PRE_SOLVE_CONTACT, oldManifold, 0);
            }
        }        
    }
    
#endif
}
////////////////////////////////////////////////////////////////////////////////
void LHContactNode::postSolve(b2Contact* contact,
                              const b2ContactImpulse* impulse){
    
    b2Body *bodyA = contact->GetFixtureA()->GetBody();
	b2Body *bodyB = contact->GetFixtureB()->GetBody();
	
    CCNode* nodeA = (CCNode*)bodyA->GetUserData();
    CCNode* nodeB = (CCNode*)bodyB->GetUserData();
    
    if(NULL == nodeA || NULL == nodeB)
        return;
    
#if COCOS2D_VERSION >= 0x00020000

    
    CCDictionary* info = (CCDictionary*)preCollisionMap.objectForKey(nodeA->getTag());
    bool foundA = false;
    if(info != NULL){
        LHContactNodeInfo* contactInfo = (LHContactNodeInfo*)info->objectForKey(nodeB->getTag());
        if(NULL != contactInfo){
            foundA = true;
            contactInfo->callListenerWithBodyA(bodyA,bodyB, contact->GetFixtureA(), contact->GetFixtureB(), contact,LH_POST_SOLVE_CONTACT,  0,impulse);
        }
    }
    if(!foundA){
        info = (CCDictionary*)preCollisionMap.objectForKey(nodeB->getTag());
        if(NULL != info){
            LHContactNodeInfo* contactInfo = (LHContactNodeInfo*)info->objectForKey(nodeA->getTag());
            if(NULL != contactInfo){
                contactInfo->callListenerWithBodyA(bodyB, bodyA, contact->GetFixtureB(), contact->GetFixtureA(), contact, LH_POST_SOLVE_CONTACT, 0, impulse);
            }
        }        
    }    

#else
    
    CCMutableDictionary<int>* info = (CCMutableDictionary<int>*)preCollisionMap.objectForKey(nodeA->getTag());
    bool foundA = false;
    if(info != NULL){
        LHContactNodeInfo* contactInfo = (LHContactNodeInfo*)info->objectForKey(nodeB->getTag());
        if(NULL != contactInfo){
            foundA = true;
            contactInfo->callListenerWithBodyA(bodyA,bodyB, contact->GetFixtureA(), contact->GetFixtureB(), contact,LH_POST_SOLVE_CONTACT,  0,impulse);
        }
    }
    if(!foundA){
        info = (CCMutableDictionary<int>*)preCollisionMap.objectForKey(nodeB->getTag());
        if(NULL != info){
            LHContactNodeInfo* contactInfo = (LHContactNodeInfo*)info->objectForKey(nodeA->getTag());
            if(NULL != contactInfo){
                contactInfo->callListenerWithBodyA(bodyB, bodyA, contact->GetFixtureB(), contact->GetFixtureA(), contact, LH_POST_SOLVE_CONTACT, 0, impulse);
            }
        }        
    }    

#endif
}
////////////////////////////////////////////////////////////////////////////////