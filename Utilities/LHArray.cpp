//
//  LHInfoObjects.h
//  plistReaderProject
//
//  Created by Bogdan Vladu on 15.12.2011.
//  Copyright (c) 2011 Bogdan Vladu. All rights reserved.
//

#include "LHArray.h"
#include "LHDictionary.h"
#include "LHObject.h"

int LHArray::numberOfArrays = 0;
//------------------------------------------------------------------------------
LHArray::LHArray()
{
    ++numberOfArrays;
}
//------------------------------------------------------------------------------
LHArray::~LHArray(){
    
    //printf("ARRAY DEALLOC %d\n", --numberOfArrays);
    for(size_t i = 0; i< objects.size(); ++i)
    {
        delete objects[i];
    }
    objects.clear();
}
//------------------------------------------------------------------------------
LHArray::LHArray(LHArray* other){
    
    ++numberOfArrays;
    for(int i = 0; i< (int)other->objects.size(); ++i)
    {
        LHObject* obj = other->objects[i];
        objects.push_back(new LHObject(obj));
    }
}
//------------------------------------------------------------------------------
void LHArray::addObject(LHObject* obj){
    
    if(0 != obj)
        objects.push_back(obj);
    
    //printf("Add obj to A\n");
}
//------------------------------------------------------------------------------
void LHArray::print(void){
    
    printf("Print ARRAY........\n");
    for(int i = 0; i< (int)objects.size(); ++i)
    {
        LHObject* obj = objects[i];
        obj->print();
    }
    printf("ARRAY END..........\n");
}
//------------------------------------------------------------------------------
LHArray::LHArray(std::stringstream& fileIN){

    ++numberOfArrays;
    int objCounter = 0;
    std::string objText;
    
    //file needs to start with <dict> else its not a LHDictionary file
    
    //printf("ARRAY START\n");
    //printf("........................................................\n");
    //std::cout << fileIN.str() << std::endl;
    //printf("........................................................\n");
    
    while(!fileIN.eof())
    {
        std::string line;
        getline(fileIN,line);
        
        //printf("A: c:%d %s\n", objCounter, line.c_str());
        
        if (std::string::npos != line.find("<key>")){
            if(1 < objCounter){
                objText+= line+"\n";
            }
        }
        else if (std::string::npos != line.find("<string>")){
            
            if(1 < objCounter){
                objText+= line+"\n";
            }else{
                addObject(new LHObject(valueForField(line)));
            }
        }
        else if (std::string::npos != line.find("<real>")){
            if(1 < objCounter){
                objText+= line+"\n";
            }else{
                addObject(new LHObject(floatFromString(valueForField(line))));
            }
        }
        else if (std::string::npos != line.find("<integer>")){
            if(1 < objCounter){
                objText+= line+"\n";
            }else{
                addObject(new LHObject(intFromString(valueForField(line))));
            }
        }
        else if (std::string::npos != line.find("<true/>")){
            if(1 < objCounter){
                objText+= line+"\n";
            }else{
                addObject(new LHObject(true));
            }
        }
        else if (std::string::npos != line.find("<false/>")){
            if(1 < objCounter){
                objText+= line+"\n";
            }else{
                addObject(new LHObject(false));
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
                addObject(new LHObject(new LHDictionary(infoText)));
                objText = "";
            }
            
            if(0 > objCounter)
            {
                objText = "";
                objCounter = 1;
            }
        }
        else if (std::string::npos != line.find("<dict/>")){
            addObject(new LHObject(new LHDictionary()));
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
                addObject(new LHObject(new LHArray(infoText)));
                objText = "";
            }
            
            if(0 > objCounter)
            {
                objText = "";
                objCounter = 1;
            }
        }
        else if (std::string::npos != line.find("<array/>")){
            addObject(new LHObject(new LHArray()));
            //objText = "";
            //objCounter = 1;
        }
    }
    
    //printf("ARRAY END ........................................................\n");
}
//------------------------------------------------------------------------------
LHObject* LHArray::objectAtIndex(const int& idx){

    if(idx >= 0 && idx < (int)objects.size())
    {
        return objects[idx];
    }
    
    printf("ERROR: Index out of bounds");
    return 0;
}
//------------------------------------------------------------------------------
int LHArray::count(void){
    return (int)objects.size();
}
//------------------------------------------------------------------------------
std::string LHArray::valueForField(const std::string& field)
{
    std::size_t posStart = field.find_first_of(">");
    std::size_t posEnd = field.find_last_of("<");
    return field.substr(posStart+1, posEnd-posStart-1);
}
//------------------------------------------------------------------------------
int LHArray::intFromString(const std::string& str)
{
    return atoi(str.c_str());
}
//------------------------------------------------------------------------------
float LHArray::floatFromString(const std::string& str)
{
    return atof(str.c_str());
}
//------------------------------------------------------------------------------