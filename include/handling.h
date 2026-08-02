#ifndef HANDLING_H
#define HANDLING_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int readInt(const string& , int , int);

int readchoice ( const string& , const vector<int>&);

bool validIndex(int ,int);

template<typename T>
bool isNull(T* ptr ,const string& message )
{
    if(ptr==nullptr)
    {
        cout<<message<<"\n";
        return true;
    }

    return false;
}

#endif