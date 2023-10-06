#pragma once

#include "miniaudio.h"
#include "glm.hpp"
#include "Logger.hpp"
#include "Types.hpp"

#include <vector>

namespace Engine
{
    class Audio
    {
        private:
            ma_engine _engine;
            std::vector<ma_sound*> _soundStorage;
            uint32 _initStatus;

        public:
            Audio();
            ~Audio();
            void PlaySound2D(const std::string& filepath, bool loop, float volume);
            void PlaySound3D(const std::string& filepath, bool loop, float volume, const glm::vec3& pos, float rolloff);
            void SetListenerPosition(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up);
            [[nodiscard]]uint32 GetInitStatus() const;
    };
}