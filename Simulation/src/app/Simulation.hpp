#pragma once

#include "SimDisplayManager.hpp"
#include "ObjectManager.hpp"

class Simulation
{
private:
	SimDisplayManager _simDisplayManager;
	ObjectManager _objectManager;
	
public:
	//---------------------------Application-Management---------------------------//
	Simulation()
		: _simDisplayManager(), _objectManager()
	{
		_simDisplayManager.createDisplay();
	}

	~Simulation()
	{
		
	}

	void init()
	{
		_objectManager.init();
	}

	void updateModels()
	{
		_objectManager.updateObjects();
	}

	void render()
	{
		_objectManager.renderObjects();
	}
	
	//---------------------------Display-Management---------------------------//
	void printVersion()
	{
		_simDisplayManager.printVersion();
	}

	GLFWwindow* getWindow()
	{
		return _simDisplayManager.getWindow();
	}

	int windowShouldClose()
	{
		return _simDisplayManager.windowShouldClose();
	}

	void measureFrameTime()
	{
		_simDisplayManager.measureFrameTime();
	}

	void clear()
	{
		_simDisplayManager.clear();
	}
	
	void updateDisplay()
	{
		_simDisplayManager.updateDisplay();
	}

	void processInput()
	{
		_simDisplayManager.processInput();
	}
	
	void closeDisplay()
	{
		_simDisplayManager.closeDisplay();
	}
};