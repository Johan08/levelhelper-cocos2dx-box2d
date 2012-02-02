//
//  LHInfoObjects.h
//  plistReaderProject
//
//  Created by Bogdan Vladu on 15.12.2011.
//  Copyright (c) 2011 Bogdan Vladu. All rights reserved.
//

#ifndef __LH_ARRAY_TYPE__
#define __LH_ARRAY_TYPE__

#include <iostream>
#include "assert.h"
#include "sstream"
#include "fstream"
#include <string>
#include <vector>
#include <map>

using namespace std;

class LHObject;

class LHArray
{
public:
//------------------------------------------------------------------------------
    LHArray(std::stringstream& fileIN);
    LHArray();
    LHArray(LHArray* other);
    virtual ~LHArray();
//------------------------------------------------------------------------------
    LHObject* objectAtIndex(const int& idx);
    void addObject(LHObject* obj);
    int count(void);
    
    void print(void);
private:
//------------------------------------------------------------------------------    
    std::vector<LHObject*> objects;
    
    static int numberOfArrays;
    
    int intFromString(const std::string& str);
    float floatFromString(const std::string& str);
    std::string valueForField(const std::string& field);
};

typedef LHArray LHMutableArray;

#endif
