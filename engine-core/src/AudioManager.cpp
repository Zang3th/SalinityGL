#include "AudioManager.hpp"

namespace Core
{
    // ----- Public -----

    AudioManager::AudioManager()
        :   _engine(nullptr)
    {
        //Init library
        _engine = irrklang::createIrrKlangDevice();

        if (!_engine)
            LOG(ERROR) << "IrrKlang could not be initialized!";
        else
            LOG(INFO) << "IrrKlang initialized!";
    }

    AudioManager::~AudioManager()
    {
        //Release resources
        for(auto& sound : _sounds)
            sound->drop();

        if(_engine)
            _engine->drop();
    }

    void AudioManager::PlaySound2D(const std::string& filepath, const bool loop, const float volume)
    {
        irrklang::ISound* sound = _engine->play2D(filepath.c_str(), loop, false, true);

        if(sound)
        {
            sound->setVolume(volume);
            _sounds.push_back(sound);

            LOG(INFO) << "Successfully played 2D-Sound at: " << filepath;
        }
        else
            LOG(ERROR) << "Failed to play 2D-Sound at: " << filepath;
    }

    void AudioManager::PlaySound3D(const std::string& filepath, const glm::vec3& position, const bool loop, const float distance, const float volume)
    {
        irrklang::ISound* sound = _engine->play3D(filepath.c_str(), irrklang::vec3df(position.x, position.y, position.z), loop, false, true);

        if(sound)
        {
            sound->setMinDistance(distance);
            sound->setVolume(volume);
            _sounds.push_back(sound);

            LOG(INFO) << "Successfully played 3D-Sound at: " << filepath;
        }
        else
            LOG(ERROR) << "Failed to play 3D-Sound at: " << filepath;
    }

    void AudioManager::SetListenerPosition(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up)
    {
        _engine->setListenerPosition
        (
            irrklang::vec3df(pos.x, pos.y, pos.z),
            irrklang::vec3df(-front.x, front.y, front.z),
            irrklang::vec3df(0, 0, 0),
            irrklang::vec3df(up.x, up.y, up.z)
        );
    }
}