#pragma once

#include "irrKlang.h"

class AudioManager
{
private:
	irrklang::ISoundEngine* _engine = nullptr;

public:
	AudioManager()
	{
		//start up the engine
		_engine = irrklang::createIrrKlangDevice();

		if (!_engine)
			spdlog::error("Audio: Error starting up the soundengine");
	}

	~AudioManager()
	{
		_engine->drop();
	}

	void playSound2D(const char* filepath, bool loop)
	{
		irrklang::ISound* snd = _engine->play2D(filepath, loop, false, true);

		if(snd)
			snd->setVolume(0.3);
	}
};