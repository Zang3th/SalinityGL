#include "Audio.hpp"

namespace Engine
{
    // ----- Public -----

    Audio::Audio()
        :   _engine(nullptr)
    {
        //Init library
        _engine = irrklang::createIrrKlangDevice();

        if(!_engine)
            Logger::Error("Failed", "Irrklang-Lib.");
        else
            Logger::Info("Init.", "Irrklang-Lib.");
    }

    Audio::~Audio()
    {
        //Release resources
        for(auto& sound : _sounds)
            sound->drop();

        if(_engine)
            _engine->drop();
    }

    void Audio::PlaySound2D(const std::string& filepath, const bool loop, const float volume)
    {
        irrklang::ISound* sound = _engine->play2D(filepath.c_str(), loop, false, true);

        if(sound)
        {
            sound->setVolume(volume);
            _sounds.push_back(sound);
            Logger::Info("Loaded", "2D-Sound", filepath);
        }
        else
            Logger::Error("Failed", "2D-Sound-Load", filepath);
    }

    void Audio::PlaySound3D(const std::string& filepath, const glm::vec3& position, const bool loop, const float distance, const float volume)
    {
        irrklang::ISound* sound = _engine->play3D(filepath.c_str(), irrklang::vec3df(position.x, position.y, position.z), loop, false, true);

        if(sound)
        {
            sound->setMinDistance(distance);
            sound->setVolume(volume);
            _sounds.push_back(sound);
            Logger::Info("Loaded", "3D-Sound", filepath);
        }
        else
            Logger::Error("Failed", "3D-Sound-Load", filepath);
    }

    void Audio::SetListenerPosition(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up)
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