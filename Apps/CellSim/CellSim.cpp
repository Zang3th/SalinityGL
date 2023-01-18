#include "CellSimApp.hpp"

int main()
{
    //Create app
    CS::CellSimApp cellSimApp;

    //Start app
    while(cellSimApp.IsRunning())
        cellSimApp.Update();

    return 0;
}