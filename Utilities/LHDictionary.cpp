//
//  LHInfoObjects.h
//  plistReaderProject
//
//  Created by Bogdan Vladu on 15.12.2011.
//  Copyright (c) 2011 Bogdan Vladu. All rights reserved.
//

#include "LHDictionary.h"
#include "LHObject.h"
#include "LHArray.h"

#include <iostream>
#include "assert.h"
#include "sstream"
#include "fstream"
#include <string>
#include <map>
#include <stdlib.h>

int LHDictionary::numberOfDicts = 0;
//------------------------------------------------------------------------------
LHDictionary::LHDictionary()
{
    ++numberOfDicts;
}
LHDictionary::~LHDictionary(){
    
    //printf("LHDictionary dealloc %d\n", --numberOfDicts);

    for ( m_it=objects.begin() ; m_it != objects.end(); ++m_it)
    {
        //printf("deleting key %s\n", m_it->first.c_str());
        delete m_it->second;
    }
    objects.clear();
}
//------------------------------------------------------------------------------    
LHDictionary::LHDictionary(LHDictionary* other)
{    
    ++numberOfDicts;
    for ( other->m_it= other->objects.begin() ; other->m_it != other->objects.end(); ++other->m_it)
    {
        std::string key = other->m_it->first;
        LHObject* object= other->m_it->second;
        
        objects[key] = new LHObject(object);
    }
}
//------------------------------------------------------------------------------    
std::vector<std::string> LHDictionary::allKeys(void){

    std::vector<std::string> keys;
    for ( m_it=objects.begin() ; m_it != objects.end(); ++m_it)
    {
        keys.push_back((*m_it).first);
    }
    return keys;
}
//------------------------------------------------------------------------------
void LHDictionary::printAllKeys(void){

    printf("Printing all keys in the dictionary\n");
    
    for ( m_it=objects.begin() ; m_it != objects.end(); ++m_it)
    {
        printf("Key: %s\n", m_it->first.c_str());
    }
}
//------------------------------------------------------------------------------
void LHDictionary::print(void){

    printf("Dict Print..........:\n");
    for ( m_it=objects.begin() ; m_it != objects.end(); ++m_it)
    {
        printf("Key: %s\n", m_it->first.c_str());
        m_it->second->print();
    }
    printf("Dict END.............\n");
}
//------------------------------------------------------------------------------
LHDictionary::LHDictionary(std::stringstream& fileIN)
{     
    ++numberOfDicts;
    std::string lastKey = "";
    int objCounter = 0;
    std::string objText;
    
    //printf("DICT START\n");
    //printf("........................................................\n");
    //std::cout << fileIN.str() << std::endl;
    //printf("................................................................\n");    
    //file needs to start with <dict> else its not a LHDictionary file
        
    while(!fileIN.eof())
    {
        std::string line;
        getline(fileIN,line);
            
        //printf("D: c:%d %s\n", objCounter, line.c_str());
            
        if (std::string::npos != line.find("<key>")){
            if(1 < objCounter){
                objText+= line+"\n";
            }else{
                lastKey = valueForField(line);                    
            }
        }
        else if (std::string::npos != line.find("<string>")){
                
            if(1 < objCounter){
                objText+= line+"\n";
            }else{
                setObjectForKey(new LHObject(valueForField(line)), lastKey);
            }
        }
        else if (std::string::npos != line.find("<real>")){
            if(1 < objCounter){
                objText+= line+"\n";
            }else{
                setObjectForKey(new LHObject(floatFromString(valueForField(line))), lastKey);
            }
        }
        else if (std::string::npos != line.find("<integer>")){
            if(1 < objCounter){
                objText+= line+"\n";
            }else{
                setObjectForKey(new LHObject(intFromString(valueForField(line))), lastKey);
            }
        }
        else if (std::string::npos != line.find("<true/>")){
            if(1 < objCounter){
                objText+= line+"\n";
            }else{
                setObjectForKey(new LHObject(true), lastKey);
            }
        }
        else if (std::string::npos != line.find("<false/>")){
            if(1 < objCounter){
                objText+= line+"\n";
            }else{
                setObjectForKey(new LHObject(false), lastKey);
            }
        }
        else if (std::string::npos != line.find("<dict>")){
            ++objCounter;
            if(1 < objCounter){
                objText+= line+"\n";
            }
        }
        else if (std::string::npos != line.find("</dict>")){
            if(1 < objCounter){
                objText+= line+"\n";
            }
                
            --objCounter;
            if(1 == objCounter)
            {
                std::stringstream infoText(objText);
                setObjectForKey(new LHObject(new LHDictionary(infoText)), lastKey);
                objText = "";
            }
                
            if(0 > objCounter)
            {
                objText = "";
                objCounter = 1;
            }
        }
        else if (std::string::npos != line.find("<dict/>")){
            std::stringstream dummyText;
            setObjectForKey(new LHObject(new LHDictionary(dummyText)), lastKey);
        }
        else if (std::string::npos != line.find("<array>")){
            ++objCounter;
            if(1 != objCounter){
                    objText+= line+"\n";
            }
        }
        else if (std::string::npos != line.find("</array>")){
            if(1 != objCounter){
                objText+= line+"\n";
            }
            
            --objCounter;
                
            if(1 == objCounter)
            {
                std::stringstream infoText(objText);
                setObjectForKey(new LHObject(new LHArray(infoText)), lastKey);
                objText = "";
            }
                
            if(0 > objCounter)
            {
                objText = "";
                objCounter = 1;
            }
        }
        else if (std::string::npos != line.find("<array/>")){
            setObjectForKey(new LHObject(new LHArray()), lastKey);
            //objText = "";
            //objCounter = 1;
        }
    }
    
    //printf("DICT END   ................................................................\n");    
}
//------------------------------------------------------------------------------    
int LHDictionary::intFromString(const std::string& str)
{
    return atoi(str.c_str());
}
//------------------------------------------------------------------------------
float LHDictionary::floatFromString(const std::string& str)
{
    return atof(str.c_str());
}
//------------------------------------------------------------------------------
std::string LHDictionary::valueForField(const std::string& field)
{
    std::size_t posStart = field.find_first_of(">");
    std::size_t posEnd = field.find_last_of("<");
    return field.substr(posStart+1, posEnd-posStart-1);
}
//------------------------------------------------------------------------------
void LHDictionary::setObjectForKey(LHObject* obj, const std::string& key){
    
    LHObject* old_obj = objects[key];
    
    if(0 != old_obj){
        delete old_obj;
    }
    
    if(0 != obj)
        objects[key] = obj;
    
    //printf("D set object for KEY %s - objType %d \n", key.c_str(), (int)obj->type());
}
//------------------------------------------------------------------------------    
LHObject* LHDictionary::objectForKey(const std::string& str){
    return objects[str];
}
LHObject* LHDictionary::objectForKey(const char* str){
    return objectForKey(std::string(str));
}
//------------------------------------------------------------------------------
LHDictionary* LHDictionary::dictForKey(const char* str){
    LHObject* obj =  objectForKey(std::string(str));
    if(0 != obj)
        return obj->dictValue();
    
    return NULL;
}
//------------------------------------------------------------------------------
void* LHDictionary::pointerForKey(const char* str){
    LHObject* obj =  objectForKey(std::string(str));
    if(0 != obj)
        if(obj->type() == LHObject::LH_VOID_TYPE)
            return obj->voidValue();
    
    return NULL;    
}
//------------------------------------------------------------------------------
LHArray* LHDictionary::arrayForKey(const char* str){
    LHObject* obj =  objectForKey(std::string(str));
    if(0 != obj)
        return obj->arrayValue();
    
    return NULL;    
}
//------------------------------------------------------------------------------
void LHDictionary::removeObjectForKey(const std::string& key){
    LHObject* obj = objects[key];
    
    if(0 != obj)
    {
        delete obj;
    }
    objects.erase(key);
}
//------------------------------------------------------------------------------
void LHDictionary::removeAllObjects(void){
    
    for(LHDictionaryIterator it = objects.begin(); it != objects.end(); ++it)
    {
        LHObject* obj = it->second;
        delete obj;
    }
    objects.clear();
}
//------------------------------------------------------------------------------