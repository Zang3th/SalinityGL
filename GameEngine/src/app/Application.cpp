#pragma once

#include "../core/DisplayManager.hpp"

int main()
{
	DisplayManager displayManager(1800, 1200);
	displayManager.createDisplay();
	displayManager.printVersion();

	while(!displayManager.WindowShouldClose())
	{
		displayManager.updateDisplay();
	}

	displayManager.closeDisplay();
}