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

	void playSound3D(const char* filepath, glm::vec3* position)
	{
		irrklang::ISound* snd = _engine->play3D(filepath, irrklang::vec3df(position->x, position->y, position->z), false, true);
		if (snd)
		{
			snd->setMinDistance(30.0f); // a loud sound
			snd->setVolume(0.1);
			snd->setIsPaused(false); // unpause the sound
		}
	}

	bool isPlaying(const char* filepath)
	{
		return _engine->isCurrentlyPlaying(filepath);
	}

	void updateListenerPosition(glm::vec3* pos, glm::vec3* front, glm::vec3* up)
	{
		irrklang::vec3df Position(pos->x, pos->y, pos->z);
		irrklang::vec3df Front(-front->x, front->y, front->z);
		irrklang::vec3df Up(up->x, up->y, up->z);
		irrklang::vec3df velPerSecond(0, 0, 0);
		_engine->setListenerPosition(Position, Front, velPerSecond, Up);
	}
};