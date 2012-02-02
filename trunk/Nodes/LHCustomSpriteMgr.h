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

#ifndef __LH_CUSTOM_SPRITE_MGR_SINGLETON
#define __LH_CUSTOM_SPRITE_MGR_SINGLETON

#include "cocos2d.h"
#include "LHSprite.h"

typedef LHSprite* (*pt2BatchSprite)(CCSpriteBatchNode *batchNode, const CCRect& rect);
typedef LHSprite* (*pt2FileSprite)(const char *pszFileName, const CCRect& rect);
typedef std::pair<pt2FileSprite, pt2BatchSprite> lh_spriteCreationMethods;

using namespace cocos2d;

class LHCustomSpriteMgr : CCObject
{
private:
    
    std::map<int, lh_spriteCreationMethods > classesDictionary;    

public:
    
    void registerCustomSpriteClassForTag(pt2FileSprite fSprite, 
                                         pt2BatchSprite bSprite, 
                                         int tag);
    
    lh_spriteCreationMethods customSpriteClassForTag(int tag);


    static LHCustomSpriteMgr* sharedInstance(void);
    
    virtual ~LHCustomSpriteMgr(void);
    
private:
    LHCustomSpriteMgr();
    static LHCustomSpriteMgr *m_sharedInstance;

public:

   };

#endif
