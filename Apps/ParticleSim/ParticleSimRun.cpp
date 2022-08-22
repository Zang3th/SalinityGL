#include "ParticleSimApp.hpp"

int main()
{
    //Create app
    ParticleSim::App particleSimApp;

    //Start app
    while(particleSimApp.IsRunning())
        particleSimApp.Update();

    return 0;
}