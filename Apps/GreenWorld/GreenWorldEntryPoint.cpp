#include "GreenWorldApp.hpp"

int main()
{
    //Create app
    GW::GreenWorldApp greenWorldApp;

    //Init and check for success
    if(greenWorldApp.Init())
    {
        //Start app
        while(greenWorldApp.IsRunning())
        {
            greenWorldApp.Update();
        }

        return 0;
    }

    return -1;
}
