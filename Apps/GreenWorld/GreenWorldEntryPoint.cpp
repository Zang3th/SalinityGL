#include "GreenWorldApp.hpp"

int main()
{
    //Create app
    GW::GreenWorldApp greenWorldApp;

    //Check for success
    if(greenWorldApp.GetInitSuccess())
    {
        //Start app
        while(greenWorldApp.IsRunning())
        {
            greenWorldApp.Update();
        }
    }
    else
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}