#include <iostream>
#include "handling.h"
#include <limits>

using namespace std;

int readInt(const string& message , int min , int max )
{
    int x;

    while(true)
    {
        cout << message;

        if(!(cin >> x))
        {

            
                    cout << "Invalid input!\n";
            
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
        }
           
        if( x < min || x > max )
        {
            cout<<"Invalid .Please try again\n ";
            continue;
        }
        
        return x ;
    }
}


int readchoice ( const string& text  , const vector<int>& validchoices)
{
   while (true)
   {
      int choice = readInt(text , -1 , 1000);

      for(int x : validchoices)
      {
        if(choice == x )
        {
            return choice;
        }
      }

      cout<<"Invalid Choice\n";

   }
   
}
bool validIndex(int index,int size)
{
    return index>=0 && index<size;
}
