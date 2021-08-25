#include "include/GW_App.hpp"

int main()
{
    //Create app
    GW::App greenWorldApp;

    //Start app
    while(greenWorldApp.IsRunning())
        greenWorldApp.Update();

    return 0;    
}