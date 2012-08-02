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

#include "LHSpritesExt.h"
#include "../Nodes/LHSettings.h"
#include "../Nodes/LHSprite.h"
#include "../Nodes/LHBezierNode.h"

std::string LHSpritesExt::uniqueNameForSprite(LHSprite* sprite){
    
    CCLog("Method is deprecated. Please use the method available in LHSprite.h");
    if(0 == sprite)
        return "";
    
    return sprite->getUniqueName();
}
std::string LHSpritesExt::uniqueNameForBody(b2Body* body){

    CCLog("Method is deprecated. Please use the method available in LHSprite.h");
    CCNode* spr = (CCNode*)body->GetUserData();
    
    if(LHSpritesExt::isLHSprite(spr))
        return ((LHSprite*)spr)->getUniqueName();
    
    if(LHSpritesExt::isLHBezierNode(spr))
        return ((LHBezierNode*)spr)->getUniqueName();
    
    return NULL;
}

//value must be a NSObject type
//void LHSpritesExt::setCustomValueWithKeyOnSprite(void* value, const std::string& key, LHSprite* sprite){
//
//    if(0 == sprite)
//        return;
//    
//    sprite->setCustomValue(value, key.c_str());
//}
//void* LHSpritesExt::customValueWithKeyForSprite(const std::string& key, LHSprite* sprite){
//
//    if(0 == sprite)
//        return 0;
//    
//    return sprite->getCustomValueWithKey(key.c_str());
//}
//------------------------------------------------------------------------------
bool LHSpritesExt::isLHSprite(CCNode* obj){

    CCLog("Method is deprecated. Please use the method available in LHSprite.h");
    if( 0 != dynamic_cast<LHSprite*>(obj))
        return true;

    return false;
}
bool LHSpritesExt::isLHBezierNode(CCNode* obj){

    CCLog("Method is deprecated. Please use the method available in LHBezierNode.h");
    if( 0 != dynamic_cast<LHBezierNode*>(obj))
        return true;
    
    return false;    
}
//------------------------------------------------------------------------------
//if fail will return -1
int  LHSpritesExt::tagForSprite(CCNode* sprite){
    if(0 != sprite)
        return sprite->getTag();
    
    return -1;
}
//if fail will return -1
int  LHSpritesExt::tagForBody(b2Body* body){

    CCLog("Method is deprecated. Please use the method available in LHSprite.h");
    if(0 != body)
    {
        CCNode* spr = (CCNode*)body->GetUserData();
        if(NULL != spr)
        {
            return spr->getTag();
        }
    }
    
    return -1;
}
//------------------------------------------------------------------------------
//b2Body* LHSpritesExt::bodyForSprite(LHSprite* sprite){
//    return sprite->getBody();
//}
//if fail will return nil
LHSprite* LHSpritesExt::spriteForBody(b2Body* body){

    CCLog("Method is deprecated. Please use the method available in LHSprite.h");
    if(0 == body)
        return 0;
    
    CCNode* spr = (CCNode*)body->GetUserData();
    
    if(LHSpritesExt::isLHSprite(spr))
    {
        return (LHSprite*)spr;
    }

    return 0;
}

//------------------------------------------------------------------------------
std::vector<b2Body*> LHSpritesExt::bodiesWithTag(enum LevelHelper_TAG tag, 
                                                 LevelHelperLoader* loader){
    
    std::vector<b2Body*> bodies;
    CCArray* sprTag = loader->spritesWithTag(tag);
        
    for(int i =0; i< sprTag->count(); ++i)
    {
        LHSprite* curSprite = (LHSprite*)sprTag->objectAtIndex(i);
        
        b2Body* bd = curSprite->getBody();
        if(bd != NULL)
        {
            bodies.push_back(bd);
        }
    }
    return bodies;
}


bool LHSpritesExt::removeSpriteWithUniqueName(const std::string& name, 
                                              LevelHelperLoader* loader){
  
    CCLog("Method is deprecated. Please use removeSprite inside LevelHelperLoader.h");
    LHSprite* spr = loader->spriteWithUniqueName(name);
    if(NULL != spr)
    {
        loader->removeSprite(spr);
        return true;
    }
    return false;
}
 
bool LHSpritesExt::removeBodyWithUniqueName(const std::string& name, 
                                            LevelHelperLoader* loader){
 
    CCLog("Method is deprecated. Please use removeSprite inside LevelHelperLoader.h");
    
    return LHSpritesExt::removeSpriteWithUniqueName(name, loader);
}

bool LHSpritesExt::removeBody(b2Body* body, LevelHelperLoader* loader){
    
    CCLog("Method is deprecated. Please use removeSprite inside LevelHelperLoader.h");
    
    LHSprite* spr = LHSpritesExt::spriteForBody(body);
    
    if(NULL != spr)
    {
        loader->removeSprite(spr);
        return true;
    }
    
    return false;
}

/*

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
-(b2Body*) bodyWithUniqueName:(NSString*)name
{
    LHSprite* spr = [spritesInLevel objectForKey:name];
    
    if(nil == spr)
        return 0;
    
    return [spr body];
}
////////////////////////////////////////////////////////////////////////////////
-(NSArray*) bodiesWithTag:(enum LevelHelper_TAG)tag
{
	NSMutableArray* array = [[[NSMutableArray alloc] init] autorelease];
	
	NSArray *keys = [spritesInLevel allKeys];
    
	for(NSString* key in keys)
	{
        LHSprite* spr = [spritesInLevel objectForKey:key];
        
        if(nil != spr && [spr tag] == (int)tag)
        {
            b2Body* body = [spr body];
            
            if(0 != body)
            {
                [array addObject:[NSValue valueWithPointer:body]];    
            }
        }
	}
	
	return array;
}
////////////////////////////////////////////////////////////////////////////////
-(bool) removeSpriteWithUniqueName:(NSString*)name
{
	LHSprite* ccsprite = [spritesInLevel objectForKey:name];
    return [self removeSprite:ccsprite];
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
-(bool) removeBodyWithUniqueName:(NSString*)name
{	
    LHSprite* spr = [self spriteWithUniqueName:name];
    return [self removeSprite:spr];
}
////////////////////////////////////////////////////////////////////////////////
-(bool) removeBody:(b2Body*)body
{	
	if(0 == body)
		return false;
	
    id ccsprite = (id)body->GetUserData();

    if(nil != ccsprite)
    {
        if([ccsprite isKindOfClass:[LHSprite class]]){
            LHSprite *ccsprite = (LHSprite*)body->GetUserData();
            return [self removeSprite:ccsprite];
        }
    else if([ccsprite isKindOfClass:[CCSprite class]]){
            //remove it old fashion way. this is not a sprite created with LevelHelper
            [ccsprite removeFromParentAndCleanup:YES];
        }
    }
    
    if(NULL != body){
        b2World* _world = body->GetWorld();
        if(0 != _world){
            _world->DestroyBody(body);
            body = NULL;
            return true;
        }
    }
    return false;
}
////////////////////////////////////////////////////////////////////////////////
-(bool) removeBodiesWithTag:(enum LevelHelper_TAG)tag
{
	NSArray *keys = [spritesInLevel allKeys];
	
    if(nil == keys)
        return false;
    
	for(NSString* key in keys)
	{
        LHSprite* spr = [spritesInLevel objectForKey:key];
        if(nil != spr && [spr tag] == (int)tag)
        {
            [self removeSprite:spr];
        }
	}
	return true;		
}
////////////////////////////////////////////////////////////////////////////////
-(bool) removeAllBodies
{
    return [self removeAllSprites];
}
////////////////////////////////////////////////////////////////////////////////

*/