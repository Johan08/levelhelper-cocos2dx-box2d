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

#ifndef __LH_PATH_NODE__
#define __LH_PATH_NODE__

#include "cocos2d.h"
#include "Box2d/Box2D.h"

#include "time.h"

using namespace cocos2d;

class LHSprite;
class LHPathNode : public CCNode
{
private:
	LHSprite* ccsprite; //week ptr
	b2Body* body; //week ptr
    std::string uniqueName;
    std::vector<CCPoint> pathPoints;
    
	float speed;
	float interval;
	bool startAtEndPoint;
	bool isCyclic;
	bool restartOtherEnd;
	int axisOrientation; //0 NO ORIENTATION 1 X 2 Y
    bool flipX;
    bool flipY;
    
	int currentPoint;
	double elapsed;

	bool paused;
	float initialAngle;
    CCPoint prevPathPosition;
	double m_time;
	bool isLine;
	
    CCObject* pathNotifierId;
    SEL_CallFuncN pathNotifierSel;
    
  //  static int numberOfPathNodes;
    
public:
    
    bool getIsCyclic(void){return isCyclic;}
    void setIsCyclic(const bool& b){isCyclic = b;}
    
    bool getRestartOtherEnd(void){return restartOtherEnd;}
    void setRestartOtherEnd(const bool& r){restartOtherEnd = r;}
    
    int getAxisOrientation(void){return axisOrientation;}
    void setAxisOrientation(const int& a){axisOrientation = a;}
    

    bool getPaused(void){return paused;}
    void setPaused(const bool& p){paused = p;}

    bool getIsLine(void){return isLine;}
    void setIsLine(const bool& l){isLine = l;}
    
    bool getFlipX(void){return flipX;}
    void setFlipX(const bool& x){flipX = x;}
    
    bool getFlipY(void){return flipY;}
    void setFlipY(const bool& y){flipY = y;}

    LHPathNode(void);
    virtual ~LHPathNode(void);
    
    bool initPathWithPoints(std::vector<CCPoint> points);
    static LHPathNode* nodePathWithPoints(std::vector<CCPoint> points);
    
    void setUniqueName(const char* name){uniqueName = std::string(name);}
    std::string& getUniqueName(void) { return uniqueName;}
    
    void setSprite(LHSprite* spr);
    LHSprite* getSprite(void){return ccsprite;}
    void setBody(b2Body* bd){body = bd;}

    float getSpeed(void){return speed;}
    void setSpeed(float value);

    void setStartAtEndPoint(bool val);
    bool getStartAtEndPoint(void){return startAtEndPoint;}
    
    void setPathNotifierObject(CCObject* notifierId){pathNotifierId = notifierId;}
    void setPathNotifierSelector(SEL_CallFuncN notifierSel){pathNotifierSel = notifierSel;}
    
    virtual void visit(void);
    
private:
    static float rotationDegreeFromPoint(CCPoint endPoint, CCPoint startPoint);

};

#endif
