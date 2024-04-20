#include "CellSimApp.hpp"

int main()
{
    //Create app
    CS::CellSimApp cellSimApp;

    //Check for success
    if(cellSimApp.GetInitSuccess())
    {
        //Start app
        while(cellSimApp.IsRunning())
        {
            cellSimApp.Update();
        }
    }
    else
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}