#include "LiquefiedApp.hpp"

int main()
{
    //Create app
    Liq::LiquefiedApp liquefiedApp;

    //Init and check for success
    if(liquefiedApp.Init())
    {
        //Start app
        while(liquefiedApp.IsRunning())
        {
            liquefiedApp.Update();
        }

        return 0;
    }

    return -1;
}
