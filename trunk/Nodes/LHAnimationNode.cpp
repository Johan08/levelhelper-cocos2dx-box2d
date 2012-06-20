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
#include "LHAnimationNode.h"
#include "LHSprite.h"
#include "../LevelHelperLoader.h"
#include "LHSettings.h"

//int LHAnimationNode::numberOfAnimNodes;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool LHAnimationNode::init(void){

    return true;
}
LHAnimationNode::~LHAnimationNode(void){
 
//    CCLog("LH_ANIMATION_NODE dealloc %d", --numberOfAnimNodes);
    frames.removeAllObjects();
}
LHAnimationNode::LHAnimationNode(void){
        
  //  ++numberOfAnimNodes;
    
    batchNode = 0;
    loop = false;
    speed = 0.2f;
    
    repetitions = 1;
    startAtLaunch = true;
}
////////////////////////////////////////////////////////////////////////////////
bool LHAnimationNode::initWithUniqueName(const char* name){
    CCAssert(name != 0, "Name must not be NULL");
    setUniqueName(name);
    init();
    return true;
}
LHAnimationNode* LHAnimationNode::animationNodeWithUniqueName(const char* name){

    LHAnimationNode *anim = new LHAnimationNode();
	if (anim && anim->initWithUniqueName(name))
    {
	    anim->autorelease();
        return anim;
    }
    CC_SAFE_DELETE(anim);
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void LHAnimationNode::setUniqueName(const char* name){
    CCAssert(name!=0, "UniqueName must not be nil");
    uniqueName = std::string(name);
}
const std::string& LHAnimationNode::getUniqueName(void){
    return uniqueName;
}
////////////////////////////////////////////////////////////////////////////////
//void LHAnimationNode::setFrames(CCMutableArray<CCSpriteFrame*>* frms){
//    frames.removeAllObjects();
//    frames.addObjectsFromArray(frms);
//}
void LHAnimationNode::setImageName(const char* img){
    imageName = std::string(img);
}
std::string& LHAnimationNode::getImageName(void){
    return imageName;
}
////////////////////////////////////////////////////////////////////////////////
void LHAnimationNode::setFramesInfo(const std::vector<CCRect>& frmInfo)
{
    framesInfo.clear();
    
    for(int i = 0; i< (int)frmInfo.size(); ++i)
    {
        framesInfo.push_back(frmInfo[i]);
    }
}
////////////////////////////////////////////////////////////////////////////////
void LHAnimationNode::computeFrames(void)
{
    if(batchNode == 0)
        return;
    
    CCMutableArray<CCSpriteFrame*> ccframes;
    
    for(int i = 0; i < (int)framesInfo.size(); ++i)
    {
        CCRect rect =  framesInfo[i];//LHRectFromString([frm objectForKey:@"FrameRect"]);
        
        std::string image = LHSettings::sharedInstance()->imagePath(imageName.c_str());
        
        if(LHSettings::sharedInstance()->shouldScaleImageOnRetina(image.c_str()))
        {
            rect.origin.x *=2.0f;
            rect.origin.y *=2.0f;
            rect.size.width *=2.0f;
            rect.size.height *=2.0f;
        }
        
        CCSpriteFrame* frame = CCSpriteFrame::frameWithTexture(batchNode->getTexture(), 
                                                               rect);
        ccframes.addObject(frame);
    }
    
    frames.removeAllObjects();
    frames.addObjectsFromArray(&ccframes);
}



CCMutableArray<CCSpriteFrame*>* LHAnimationNode::getFrames(void){
    return &frames;
}
void LHAnimationNode::setBatchNode(CCSpriteBatchNode* node){
    batchNode = node;
}
////////////////////////////////////////////////////////////////////////////////
void LHAnimationNode::runAnimationOnSprite(LHSprite* ccsprite,
                                           int startFrame,
                                           CCObject* animNotifierId,
                                           SEL_CallFuncND animNotifierSel,
                                           const bool& notifOnLoop)
{
    CCAnimate* animSeqFromStartAction = NULL;
    CCMutableArray<CCSpriteFrame*> framesSeq;
    
    if(startFrame > 0 && startFrame < frames.count())
    {        
        for(int i = startFrame; i < frames.count(); ++i){
            framesSeq.addObject(frames.getObjectAtIndex(i));
        }
        
        CCAnimation *animSeqFromStartFrame = CCAnimation::animationWithFrames(&framesSeq, speed);
        
        animSeqFromStartAction = CCAnimate::actionWithAnimation(animSeqFromStartFrame,false);
    }

    CCAnimation *anim = CCAnimation::animationWithFrames(&frames, speed);
    
    CCFiniteTimeAction *seq = 0;
    if(!loop)
    {
        CCRepeat* animAct = CCRepeat::actionWithAction(CCAnimate::actionWithAnimation(anim, false), 
                                                       repetitions);
        CCFiniteTimeAction* seq1 = animAct;
        if(animSeqFromStartAction != NULL)
            seq1 = CCSequence::actionOneTwo(animSeqFromStartAction, animAct);
            
        if(0 != animNotifierId)
        {
            CCCallFuncND* actionRestart = CCCallFuncND::actionWithTarget(animNotifierId,
                                                                         animNotifierSel,
                                                                         (void*)&uniqueName);
            seq = CCSequence::actionOneTwo(seq1,actionRestart);
        }
        else{
            seq = seq1;
        }
    }
    else
    {
        if(notifOnLoop && 0 != animNotifierId)
        {
            CCCallFuncND* actionRestart = CCCallFuncND::actionWithTarget(animNotifierId, 
                                                                         animNotifierSel,
                                                                         (void*)&uniqueName);
            
            CCSequence* animAct = CCSequence::actionOneTwo(CCAnimate::actionWithAnimation(anim, false), 
                                                           actionRestart);
            
            seq = CCRepeatForever::actionWithAction(animAct);
        }
        else
        {
            seq = CCRepeatForever::actionWithAction(CCAnimate::actionWithAnimation(anim , false));
        }

        if(animSeqFromStartAction != NULL)
        {
            CCFiniteTimeAction* repeatActionCantBeAddedInSequence = CCCallFuncO::actionWithTarget(ccsprite, callfuncO_selector(LHSprite::setAnimationSequence), seq);
            
            seq = CCSequence::actionOneTwo(animSeqFromStartAction, repeatActionCantBeAddedInSequence);
        }
        //////////<<<<<<<<<<<<-------
    }
    
    if(0 != seq)
    {
        seq->setTag(LH_ANIM_ACTION_TAG);
        
        ccsprite->stopActionByTag(LH_ANIM_ACTION_TAG);
        ccsprite->setAnimation(this);
        setAnimationTexturePropertiesOnSprite(ccsprite);
        ccsprite->runAction(seq);    
    }    
}

/*
void LHAnimationNode::runAnimationOnSprite(LHSprite* ccsprite,
                                           SelectorProtocol* animNotifierId,
                                           SEL_CallFuncND animNotifierSel,
                                           const bool& notifOnLoop)
{
    CCAnimation *anim = CCAnimation::animationWithFrames(&frames, speed);
    
    CCFiniteTimeAction *seq = 0;
    if(!loop)
    {
        CCRepeat* animAct = CCRepeat::actionWithAction(CCAnimate::actionWithAnimation(anim, false), 
                                                       repetitions);
        
        if(0 != animNotifierId)
        {
            CCCallFuncND* actionRestart = CCCallFuncND::actionWithTarget(animNotifierId,
                                                                         animNotifierSel,
                                                                         (void*)&uniqueName);
            seq = CCSequence::actionOneTwo(animAct,actionRestart);
        }
        else{
            seq = animAct;
        }
    }
    else
    {
        if(notifOnLoop && 0 != animNotifierId)
        {
            CCCallFuncND* actionRestart = CCCallFuncND::actionWithTarget(animNotifierId, 
                                                                         animNotifierSel,
                                                                         (void*)&uniqueName);
            
            CCSequence* animAct = CCSequence::actionOneTwo(CCAnimate::actionWithAnimation(anim, false), 
                                                           actionRestart);
            
            seq = CCRepeatForever::actionWithAction(animAct);
        }
        else
        {
            seq = CCRepeatForever::actionWithAction(CCAnimate::actionWithAnimation(anim , false));
        }
    }
    
    if(0 != seq)
    {
        seq->setTag(LH_ANIM_ACTION_TAG);

        printf ("set anim--------------------------\n");
        ccsprite->setAnimation(this);
        //setAnimationTexturePropertiesOnSprite(ccsprite);
        ccsprite->runAction(seq);    
    }
}
*/
////////////////////////////////////////////////////////////////////////////////
int LHAnimationNode::getNumberOfFrames(void){
    return frames.count();
}
////////////////////////////////////////////////////////////////////////////////
void LHAnimationNode::setAnimationTexturePropertiesOnSprite(LHSprite* ccsprite){
    
    if(!LHSettings::sharedInstance()->isCoronaUser())
        ccsprite->removeFromParentAndCleanup(true);
    
    ccsprite->setTexture(batchNode->getTexture());
    
    if(!LHSettings::sharedInstance()->isCoronaUser())
    {
        ccsprite->setSpriteBatchNode(batchNode);
        batchNode->addChild(ccsprite);
    }
}
void LHAnimationNode::setFrame(int frameNo, LHSprite* spr){

    if(0 == spr)
        return;
    
    if(frameNo >= 0 && frameNo < frames.count())
    {
        CCSpriteFrame* frame = frames.getObjectAtIndex(frameNo);
        
        if(0 != frame)
        {
            spr->setTextureRect(frame->getRect());
        }
    }
}
////////////////////////////////////////////////////////////////////////////////