#define MINIAUDIO_IMPLEMENTATION

#include "Audio.hpp"

namespace Engine
{
    // ----- Public -----

    Audio::Audio()
    {
        if(ma_engine_init(nullptr, &_engine) != MA_SUCCESS)
        {
            ma_engine_start(&_engine);
            Logger::Error("Failed", "Audio-Init.");
        }
        else
        {
            Logger::Info("Loaded", "Miniaudio");
        }
    }

    Audio::~Audio()
    {
        for(auto& sound : _soundStorage)
        {
            ma_sound_uninit(sound);
            free(sound);
        }

        ma_engine_uninit(&_engine);
    }

    void Audio::PlaySound2D(const std::string& filepath, const bool loop, const float volume)
    {
        auto sound = (ma_sound*)malloc(sizeof(ma_sound));

        if(ma_sound_init_from_file(&_engine, filepath.c_str(), MA_SOUND_FLAG_NO_SPATIALIZATION, nullptr, nullptr, sound) != MA_SUCCESS)
        {
            Logger::Error("Failed", "Sound-Loading", filepath);
        }
        else
        {
            Logger::Info("Loaded", "2D-Sound", filepath);

            ma_sound_set_looping(sound, loop);
            ma_sound_set_volume(sound, volume);
            ma_sound_start(sound);
            _soundStorage.push_back(sound);
        }
    }

    void Audio::PlaySound3D(const std::string& filepath, const bool loop, const float volume, const glm::vec3& pos, const float rolloff)
    {
        auto sound = (ma_sound*)malloc(sizeof(ma_sound));

        if(ma_sound_init_from_file(&_engine, filepath.c_str(), 0, nullptr, nullptr, sound) != MA_SUCCESS)
        {
            Logger::Error("Failed", "Sound-Loading", filepath);
        }
        else
        {
            Logger::Info("Loaded", "3D-Sound", filepath);

            ma_sound_set_looping(sound, loop);
            ma_sound_set_volume(sound, volume);
            ma_sound_set_pinned_listener_index(sound, 0);
            ma_sound_set_position(sound, pos.x, pos.y, pos.z);
            ma_sound_set_rolloff(sound, rolloff);
            ma_sound_start(sound);
            _soundStorage.push_back(sound);
        }
    }

    void Audio::SetListenerPosition(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up)
    {
        ma_engine_listener_set_position(&_engine, 0, pos.x, pos.y, pos.z);
        ma_engine_listener_set_direction(&_engine, 0, front.x, front.y, front.z);
        ma_engine_listener_set_world_up(&_engine, 0, up.x, up.y, up. z);
    }
}