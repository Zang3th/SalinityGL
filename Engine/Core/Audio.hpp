#pragma once

#include "irrKlang.h"
#include "glm.hpp"
#include "Logger.hpp"

#include <vector>

namespace Engine
{
    class Audio
    {
        private:
            irrklang::ISoundEngine* _engine;
            std::vector<irrklang::ISound*> _sounds;

        public:
            Audio();
            ~Audio();
            void PlaySound2D(const std::string& filepath, bool loop, float volume);
            void PlaySound3D(const std::string& filepath, const glm::vec3& position,  bool loop, float distance, float volume);
            void SetListenerPosition(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up);
    };
}