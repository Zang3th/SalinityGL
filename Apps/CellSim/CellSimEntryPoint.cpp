#include "CellSimApp.hpp"

int main()
{
    //Create app
    CS::CellSimApp cellSimApp;

    //Init and check for success
    if(cellSimApp.Init())
    {
        //Start app
        while(cellSimApp.IsRunning())
        {
            cellSimApp.Update();
        }

        return 0;
    }

    return -1;
}
