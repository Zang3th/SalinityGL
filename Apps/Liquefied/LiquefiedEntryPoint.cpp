#include "LiquefiedApp.hpp"

int main()
{
    //Create app
    Liq::LiquefiedApp liquefiedApp;

    //Check for success
    if(liquefiedApp.GetInitSuccess())
    {
        //Start app
        while(liquefiedApp.IsRunning())
        {
            liquefiedApp.Update();
        }
    }
    else
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}