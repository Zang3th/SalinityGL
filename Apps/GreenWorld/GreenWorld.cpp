#include "GreenWorldApp.hpp"

int main()
{
    //Create app
    GreenWorld::GreenWorldApp greenWorldApp;

    //Start app
    while(greenWorldApp.IsRunning())
        greenWorldApp.Update();

    return 0;    
}