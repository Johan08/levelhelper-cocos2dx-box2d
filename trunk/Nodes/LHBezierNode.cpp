//  This file was generated by LevelHelper
//  http://www.levelhelper.org
//
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
#include "LHBezierNode.h"
#include "LevelHelperLoader.h"
#include "LHPathNode.h"
#include "LHSettings.h"
#include "LHSprite.h"

//int LHBezierNode::numberOfBezierNodes = 0;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void LHBezierNode::init(void){
    
}
////////////////////////////////////////////////////////////////////////////////
LHBezierNode::~LHBezierNode(void){
    
   // CCLog("LHBezierNode destructor %d", --numberOfBezierNodes);
    
//    for(int i = 0; i < pathNodes.count(); ++i)
//    {
//        LHPathNode* pNode = pathNodes.getObjectAtIndex(i);
//        pNode->removeFromParentAndCleanup(true);
//	}
//	pathNodes.removeAllObjects();
	
	if(0 != body)
	{
		b2World* _world = body->GetWorld();
		if(0 != _world)
		{
			_world->DestroyBody(body);
			body = NULL;
		}
	}
    
    pathPoints.clear();
	linesHolder.clear();
    trianglesHolder.clear();
}
////////////////////////////////////////////////////////////////////////////////
LHBezierNode::LHBezierNode(void){
 
    texture = NULL;
    body = NULL;
//    numberOfBezierNodes++;
}
////////////////////////////////////////////////////////////////////////////////
bool LHBezierNode::initWithDictionary(LHDictionary* bezierDict,
                                      CCLayer* ccLayer, 
                                      b2World* world){
    
    tagTouchBeginObserver = NULL;
    tagTouchMovedObserver = NULL;
    tagTouchEndedObserver = NULL;
    swallowTouches = false;

    
    isClosed	= bezierDict->objectForKey("IsClosed")->boolValue();
    isTile		= bezierDict->objectForKey("IsTile")->boolValue();
    isVisible	= bezierDict->objectForKey("IsDrawable")->boolValue();
    isLine		= bezierDict->objectForKey("IsSimpleLine")->boolValue();
    isPath		= bezierDict->objectForKey("IsPath")->boolValue();
    
    uniqueName  = bezierDict->objectForKey("UniqueName")->stringValue();
    
    setTag(bezierDict->objectForKey("Tag")->intValue());
    setVertexZ(bezierDict->objectForKey("ZOrder")->intValue());
    
    std::string img = bezierDict->objectForKey("Image")->stringValue();
    imageSize = CCSizeZero;
    if(img != "")
    {
        std::string path = LHSettings::sharedInstance()->imagePath(img.c_str());
        texture = CCTextureCache::sharedTextureCache()->addImage(path.c_str());
        if( texture ) {
            imageSize = texture->getContentSize();
        }
    }
    
    winSize = CCDirector::sharedDirector()->getWinSize();		
    float scale = CCDirector::sharedDirector()->getContentScaleFactor();

    
    color = CCRectFromString(bezierDict->objectForKey("Color")->stringValue().c_str());
    lineColor = CCRectFromString(bezierDict->objectForKey("LineColor")->stringValue().c_str());
    lineWidth = bezierDict->objectForKey("LineWidth")->floatValue()*scale;
    
    initTileVerticesFromDictionary(bezierDict);
    initPathPointsFromDictionary(bezierDict);	
    createBodyFromDictionary(bezierDict,world);
    
    return true;
}
////////////////////////////////////////////////////////////////////////////////
LHBezierNode* LHBezierNode::nodeWithDictionary(LHDictionary* properties,
                                               CCLayer* ccLayer, 
                                               b2World* world){
    
    LHBezierNode *pobBNode = new LHBezierNode();
	if (pobBNode && pobBNode->initWithDictionary(properties, ccLayer, world))
    {
	    pobBNode->autorelease();
        return pobBNode;
    }
    CC_SAFE_DELETE(pobBNode);
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
LHPathNode* LHBezierNode::addSpriteOnPath(LHSprite* spr, 
                                          float   pathSpeed, 
                                          bool    startAtEndPoint,
                                          bool    isCyclic,
                                          bool    restartOtherEnd,
                                          int     axis,
                                          bool    flipx,
                                          bool    flipy,
                                          bool    deltaMove){
    
    
    
	LHPathNode* node = LHPathNode::nodePathWithPoints(pathPoints);	
    node->setStartAtEndPoint(startAtEndPoint);
	node->setSprite(spr);
	node->setBody(spr->getBody());
    
    if(!deltaMove){
        if((int)pathPoints.size() > 0)
        {
            CCPoint pathPos = pathPoints[0];
            spr->transformPosition(pathPos);
        }
    }
    
	node->setSpeed(pathSpeed);
    node->setRestartOtherEnd(restartOtherEnd);
	node->setIsCyclic(isCyclic);
	node->setAxisOrientation(axis);
	node->setIsLine(isLine);
    node->setFlipX(flipx);
    node->setFlipY(flipy);
    node->setUniqueName(uniqueName.c_str());
    //pathNodes.addObject(node);
	    
    this->getParent()->addChild(node);
    
    return  node;

}
////////////////////////////////////////////////////////////////////////////////
CCPoint LHBezierNode::pointOnCurve(CCPoint p1, CCPoint p2, CCPoint p3, CCPoint p4, float t){    
	float var1, var2, var3;
    CCPoint vPoint(0.0f, 0.0f);
    
    var1 = 1 - t;
    var2 = var1 * var1 * var1;
    var3 = t * t * t;
    vPoint.x = var2*p1.x + 3*t*var1*var1*p2.x + 3*t*t*var1*p3.x + var3*p4.x;
    vPoint.y = var2*p1.y + 3*t*var1*var1*p2.y + 3*t*t*var1*p3.y + var3*p4.y;
    return(vPoint);				
}
////////////////////////////////////////////////////////////////////////////////
void LHBezierNode::initTileVerticesFromDictionary(LHDictionary* bezierDict)
{
    float scale = CCDirector::sharedDirector()->getContentScaleFactor();
    
	CCPoint convert = LHSettings::sharedInstance()->convertRatio();
	LHArray* fixtures = bezierDict->arrayForKey("TileVertices");
    
    if(NULL != fixtures)
    {
        for(int i = 0; i < fixtures->count(); ++i)
        {
            LHArray* fix = fixtures->objectAtIndex(i)->arrayValue();
        
            std::vector<CCPoint> triagle;
        
            for(int j = 0; j < fix->count(); ++j)
            {
                CCPoint point = LHPointFromString(fix->objectAtIndex(j)->stringValue());
			
                CCPoint pos_offset = LHSettings::sharedInstance()->possitionOffset();
            
                point.x = point.x* convert.x;
                point.y = winSize.height - point.y*convert.y;
			
                //point.x*=scale;
                //point.y*=scale;

                point.x += pos_offset.x;
                point.y -= pos_offset.y;

                triagle.push_back(point);
            }
            trianglesHolder.push_back(triagle);
        }	
	}
	
	if(isVisible)
	{
		LHArray* curvesInShape = bezierDict->objectForKey("Curves")->arrayValue();
		
		int MAX_STEPS = 25;
		
        for(int i = 0; i < curvesInShape->count(); ++i)
		{
            LHDictionary* curvDict = curvesInShape->objectAtIndex(i)->dictValue();
            
			CCPoint endCtrlPt   = LHPointFromString(curvDict->objectForKey("EndControlPoint")->stringValue());
			CCPoint startCtrlPt = LHPointFromString(curvDict->objectForKey("StartControlPoint")->stringValue());
			CCPoint endPt       = LHPointFromString(curvDict->objectForKey("EndPoint")->stringValue());
			CCPoint startPt     = LHPointFromString(curvDict->objectForKey("StartPoint")->stringValue());
			
            CCPoint pos_offset = LHSettings::sharedInstance()->possitionOffset();
                        
			if(!isLine)
			{
				CCPoint prevPoint;
				bool firstPt = true;
				
				for(float t = 0; t <= (1 + (1.0f / MAX_STEPS)); t += 1.0f / MAX_STEPS)
				{
					CCPoint vPoint = LHBezierNode::pointOnCurve(startPt,
                                                                startCtrlPt,
                                                                endCtrlPt,
                                                                endPt,
                                                                t);
					
					if(!firstPt)
					{
						CCPoint pt1 = CCPointMake(prevPoint.x*convert.x, 
												  winSize.height - prevPoint.y*convert.y);
						CCPoint pt2 = CCPointMake(vPoint.x*convert.x, 
												  winSize.height - vPoint.y*convert.y);
						
                        pt1.x *=scale;
                        pt1.y *=scale;
                        
                        pt2.x *=scale;
                        pt2.y *=scale;
                        
                        pt1.x += pos_offset.x;
                        pt1.y -= pos_offset.y;
                        
                        pt2.x += pos_offset.x;
                        pt2.y -= pos_offset.y;
                        
                        linesHolder.push_back(pt1);
                        linesHolder.push_back(pt2);
					}
					prevPoint = vPoint;
					firstPt = false;					
				}
			}
			else
			{
				
				CCPoint pos1 = CCPointMake(startPt.x*convert.x, 
										   winSize.height - startPt.y*convert.y);
				CCPoint pos2 = CCPointMake(endPt.x*convert.x, 
										   winSize.height - endPt.y*convert.y);
				
                pos1.x*=scale;
                pos1.y*=scale;
                
                pos2.x*=scale;
                pos2.y*=scale;
                
                pos1.x += pos_offset.x;
                pos1.y -= pos_offset.y;
                
                pos2.x += pos_offset.x;
                pos2.y -= pos_offset.y;
                
                linesHolder.push_back(pos1);
                linesHolder.push_back(pos2);				
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void LHBezierNode::initPathPointsFromDictionary(LHDictionary* bezierDict)
{	
    LHArray* curvesInShape = bezierDict->objectForKey("Curves")->arrayValue();    
    int MAX_STEPS = 25;    
	CCPoint conv = LHSettings::sharedInstance()->convertRatio();
	
	int i = 0;
    for(int j = 0; j < curvesInShape->count(); ++j)
    {
        LHDictionary* curvDict = curvesInShape->objectAtIndex(j)->dictValue();
        
        CCPoint endCtrlPt   = LHPointFromString(curvDict->objectForKey("EndControlPoint")->stringValue());
        CCPoint startCtrlPt = LHPointFromString(curvDict->objectForKey("StartControlPoint")->stringValue());
        CCPoint endPt       = LHPointFromString(curvDict->objectForKey("EndPoint")->stringValue());
        CCPoint startPt     = LHPointFromString(curvDict->objectForKey("StartPoint")->stringValue());
		
		CCPoint pos_offset = LHSettings::sharedInstance()->possitionOffset();
        
		if(!isLine)
        {
            for(float t = 0; t <= (1 + (1.0f / MAX_STEPS)); t += 1.0f / MAX_STEPS)
            {
                CCPoint vPoint = LHBezierNode::pointOnCurve(startPt,
                                                            startCtrlPt,
                                                            endCtrlPt,
                                                            endPt,
                                                            t);
				
                vPoint = ccp(vPoint.x*conv.x, winSize.height - vPoint.y*conv.y);
                
                vPoint.x += pos_offset.x;
                vPoint.y -= pos_offset.y;
                                
                pathPoints.push_back(vPoint);
            }
			
            pathPoints.pop_back();
        }
        else
        {
            CCPoint sPoint =  ccp(startPt.x*conv.x, 
                                  winSize.height - startPt.y*conv.y);
            
            sPoint.x += pos_offset.x;
            sPoint.y -= pos_offset.y;
            
            pathPoints.push_back(sPoint);

            
            if(i == curvesInShape->count()-1)
            {
                CCPoint ePoint = ccp(endPt.x*conv.x,
                                     winSize.height - endPt.y*conv.y);
                ePoint.x += pos_offset.x;
                ePoint.y -= pos_offset.y;
                
                pathPoints.push_back(ePoint);
            }
            ++i;            
        }
	}		
}
////////////////////////////////////////////////////////////////////////////////
void LHBezierNode::createBodyFromDictionary(LHDictionary* bezierDict, b2World* world)
{
	if(isPath)
		return;
	
	if((int)pathPoints.size() < 2)
		return;
	
	b2BodyDef bodyDef;	
	
	int bodyType = bezierDict->objectForKey("PhysicType")->intValue();
	if(bodyType > 2)
        return;
        
	bodyDef.type = (b2BodyType)bodyType;
    
	bodyDef.position.Set(0.0f, 0.0f);
	bodyDef.angle = 0.0f;
	
	bodyDef.userData = this;
	
	body = world->CreateBody(&bodyDef);
	
	float ptm = LHSettings::sharedInstance()->lhPtmRatio();
    
    if(b2_version.major <= 2)
        if(b2_version.minor <=2)
            if(b2_version.revision <2)
                CCLog("Please update to Box2d 2.2.2 or above or else you may experience asserts");
    
    for(int k =0; k< (int)trianglesHolder.size();++k)
    {
        std::vector<CCPoint> fix = trianglesHolder[k];
        
        int size = fix.size();
        b2Vec2 *verts = new b2Vec2[size];
        int i = 0;
        for(int j = 0; j < size; ++j)
        {
            CCPoint pt = fix[j];
            
            verts[i].x =pt.x/ptm;
            verts[i].y =pt.y/ptm;
            ++i;
        }

        b2PolygonShape shape;
        shape.Set(verts, size);		
        
        b2FixtureDef fixture;
        
        fixture.density = bezierDict->objectForKey("Density")->floatValue();
		fixture.friction = bezierDict->objectForKey("Friction")->floatValue();
		fixture.restitution = bezierDict->objectForKey("Restitution")->floatValue();
		
		fixture.filter.categoryBits = bezierDict->objectForKey("Category")->intValue();
		fixture.filter.maskBits = bezierDict->objectForKey("Mask")->intValue();
		fixture.filter.groupIndex = bezierDict->objectForKey("Group")->intValue();
		
		fixture.isSensor = bezierDict->objectForKey("IsSenzor")->boolValue();
        
        fixture.shape = &shape;
        body->CreateFixture(&fixture);
        delete[] verts;
    }		
    
    
    b2Vec2 * verts = new b2Vec2 [(int) pathPoints.size ()];
    for (int i = 0; i <(int) pathPoints.size ();++ i)
    {
        verts [i]. x = pathPoints [i] .x / ptm;
        verts [i]. y = pathPoints [i] .y / ptm;
    }
    b2ChainShape shape;
    shape.CreateChain (verts, (int) pathPoints.size ());
    
    b2FixtureDef fixture;
    
    fixture.density = bezierDict-> objectForKey ("Density") -> floatValue ();
    fixture.friction = bezierDict-> objectForKey ("Friction") -> floatValue ();
    fixture.restitution = bezierDict-> objectForKey ("Restitution") -> floatValue ();
    
    fixture.filter.categoryBits = bezierDict-> objectForKey ("Category") -> intValue ();
    fixture.filter.maskBits = bezierDict-> objectForKey ("Mask") -> intValue ();
    fixture.filter.groupIndex = bezierDict-> objectForKey ("Group") -> intValue ();
    
    fixture.isSensor = bezierDict-> objectForKey ("IsSenzor") -> boolValue ();
    
    fixture.shape = &shape;
    body-> CreateFixture (& fixture);
    delete [] verts;
}
////////////////////////////////////////////////////////////////////////////////
void LHBezierNode::pushBlendingTextureNamed(const std::string& texName, 
                                            bool tile,
                                            GLenum blendSource, 
                                            GLenum blendDestination){

    if(!isTile)
        return;
    
    CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage(texName.c_str());
    if(NULL != tex){
        LHBezierBlendingInfo info = {tex, blendSource, blendDestination, tile};
        blendingTextures.push_back(info);
    }
}
////////////////////////////////////////////////////////////////////////////////
void LHBezierNode::draw(void)
{
    
    float scale = CCDirector::sharedDirector()->getContentScaleFactor();
    
	if(0.0f != LHSettings::sharedInstance()->customAlpha())
	{
		glColor4f(color.origin.x, 
				  color.origin.y, 
				  color.size.width, 
				  color.size.height*LHSettings::sharedInstance()->customAlpha());
		glPushMatrix();
		
        glDisableClientState(GL_COLOR_ARRAY);
        
        if(NULL != texture)
        {
            glEnable(GL_TEXTURE_2D);		            
            glBindTexture(GL_TEXTURE_2D, texture->getName());
		
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            
            int size = (int)trianglesHolder.size();
            CCPoint* verts = new CCPoint[size* 3];
            CCPoint* texcoords = new CCPoint[size* 3];
            
            for(int k = 0; k < (int)trianglesHolder.size();++k)
            {
                std::vector<CCPoint> fix = trianglesHolder[k];
                
                for(int j = 0; j < 3; ++j)
                {
                    CCPoint pt = fix[j];
                
                    pt.x *= scale;
                    pt.y *= scale;
                    
                    verts[k*3 + j] = pt;
                    texcoords[k*3 + j].x = pt.x/imageSize.width;
                    texcoords[k*3 + j].y = (winSize.height - pt.y)/imageSize.height;
                }
            }
                                
            glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
            glVertexPointer(2, GL_FLOAT, 0, verts);
            glDrawArrays(GL_TRIANGLES, 0, 3*size);
            
            bool wasBlend = glIsEnabled(GL_BLEND);
            glEnable(GL_BLEND);            
            int oldBlendDST = 0;
            glGetIntegerv(GL_BLEND_DST, &oldBlendDST);
            int oldBlendSRC = 0;
            glGetIntegerv(GL_BLEND_SRC, &oldBlendSRC);

            for(int i = 0; i < blendingTextures.size(); ++i)
            {
                CCTexture2D* tex = blendingTextures[i].texture;
                if(NULL != tex)
                {
                    glBlendFunc(blendingTextures[i].blendSource, blendingTextures[i].blendDestination);
                    glBindTexture(GL_TEXTURE_2D, tex->getName());
                    
                    if(blendingTextures[i].tile){
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    }

                    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
                    glVertexPointer(2, GL_FLOAT, 0, verts);
                    glDrawArrays(GL_TRIANGLES, 0, 3*size);                
                }
            }
            
            glBlendFunc(oldBlendSRC, oldBlendDST);
            if(!wasBlend)
                glDisable(GL_BLEND);
            
            delete [] verts;
            delete [] texcoords;
		}
        
		float oldLineWidth = 1.0f;
		glGetFloatv(GL_LINE_WIDTH, &oldLineWidth); 
		
		glLineWidth(lineWidth);
		
		glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glColor4f(lineColor.origin.x, 
				  lineColor.origin.y, 
				  lineColor.size.width, 
				  lineColor.size.height*LHSettings::sharedInstance()->customAlpha());
		
        CCPoint* line_verts = new CCPoint[linesHolder.size()];
        for(int i = 0; i < (int)linesHolder.size(); i+=2)
		{
			CCPoint pt1 = linesHolder[i];
			CCPoint pt2 = linesHolder[i+1];
            line_verts[i] = pt1;
            line_verts[i+1] = pt2;            
		}
        
        glVertexPointer(2, GL_FLOAT, 0, line_verts);
        glDrawArrays(GL_LINES, 0, linesHolder.size());
        delete[] line_verts;
        
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
		glLineWidth(oldLineWidth);
		glEnable(GL_TEXTURE_2D);	
		glPopMatrix();
	}	
}
////////////////////////////////////////////////////////////////////////////////
bool LHBezierNode::isLHBezierNode(CCNode* obj){
    
    if( 0 != dynamic_cast<LHBezierNode*>(obj))
        return true;
    
    return false;    
}


bool LHBezierNode::isTouchedAtPoint(CCPoint point){
    
    if(body != NULL){
        b2Fixture* stFix = body->GetFixtureList();
        while(stFix != 0){
            if(stFix->TestPoint(b2Vec2(point.x/LHSettings::sharedInstance()->lhPtmRatio(), 
                                       point.y/LHSettings::sharedInstance()->lhPtmRatio()))){
                return true;
            }
            stFix = stFix->GetNext();
        }
    }
    return false;    
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void LHBezierNode::registerTouchBeginObserver(CCObject* observer, 
                                              SEL_CallFuncO selector){
    touchBeginObserver.object = observer;
    touchBeginObserver.selector = selector;
}
//------------------------------------------------------------------------------
void LHBezierNode::registerTouchMovedObserver(CCObject* observer, 
                                              SEL_CallFuncO selector){
    touchMovedObserver.object = observer;
    touchMovedObserver.selector = selector;
}
//------------------------------------------------------------------------------
void LHBezierNode::registerTouchEndedObserver(CCObject* observer, 
                                              SEL_CallFuncO selector){
    touchEndedObserver.object = observer;
    touchEndedObserver.selector = selector;    
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool LHBezierNode::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) 
{    
    CCPoint touchPoint =     pTouch->locationInView();
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
    
    if(isTouchedAtPoint(touchPoint))
    {
        LHTouchInfo* info = LHTouchInfo::touchInfo();
        info->relativePoint = CCPointMake(touchPoint.x - getPosition().x,
                                          touchPoint.y - getPosition().y);
        info->glPoint = touchPoint;
        info->event = pEvent;
        info->touch = pTouch;
        info->bezier = this;
        info->delta = CCPointZero;
        
        if(touchBeginObserver.object){
            (touchBeginObserver.object->*touchBeginObserver.selector)(info);
        }        
        
        if(tagTouchBeginObserver && tagTouchBeginObserver->object){
            (tagTouchBeginObserver->object->*tagTouchBeginObserver->selector)(info);
        }
        return swallowTouches;
    }
    return false;
}
//------------------------------------------------------------------------------
void LHBezierNode::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
    CCPoint touchPoint =     pTouch->locationInView();
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
    
    CCPoint prevLocation = pTouch->previousLocationInView();
    prevLocation =  CCDirector::sharedDirector()->convertToGL(prevLocation);
    
    LHTouchInfo* info = LHTouchInfo::touchInfo();
    info->relativePoint = CCPointMake(touchPoint.x - getPosition().x,
                                      touchPoint.y - getPosition().y);
    info->glPoint = touchPoint;
    info->event = pEvent;
    info->touch = pTouch;
    info->bezier = this;
    info->delta = CCPointMake(touchPoint.x - prevLocation.x,
                              touchPoint.y - prevLocation.y);
    
    
    if(touchMovedObserver.object){
        (touchMovedObserver.object->*touchMovedObserver.selector)(info);
    }  
    
    if(tagTouchMovedObserver && tagTouchMovedObserver->object){
        (tagTouchMovedObserver->object->*tagTouchMovedObserver->selector)(info);
    }
}
//------------------------------------------------------------------------------
void LHBezierNode::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
    CCPoint touchPoint =     pTouch->locationInView();
    touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
    
    CCPoint prevLocation = pTouch->previousLocationInView();
    prevLocation =  CCDirector::sharedDirector()->convertToGL(prevLocation);
    
    LHTouchInfo* info = LHTouchInfo::touchInfo();
    info->relativePoint = CCPointMake(touchPoint.x - getPosition().x,
                                      touchPoint.y - getPosition().y);
    info->glPoint = touchPoint;
    info->event = pEvent;
    info->touch = pTouch;
    info->bezier = this;
    info->delta = CCPointMake(touchPoint.x - prevLocation.x,
                              touchPoint.y - prevLocation.y);
    
    if(touchEndedObserver.object){
        (touchEndedObserver.object->*touchEndedObserver.selector)(info);
    }
    if(tagTouchEndedObserver && tagTouchEndedObserver->object){
        (tagTouchEndedObserver->object->*tagTouchEndedObserver->selector)(info);
    }
}
//------------------------------------------------------------------------------
void LHBezierNode::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
    
}

void LHBezierNode::setTagTouchBeginObserver(LHObserverPair* pair){
    tagTouchBeginObserver = pair;
}
void LHBezierNode::setTagTouchMovedObserver(LHObserverPair* pair){
    tagTouchMovedObserver = pair;
}
void LHBezierNode::setTagTouchEndedObserver(LHObserverPair* pair){
    tagTouchEndedObserver = pair;
}

