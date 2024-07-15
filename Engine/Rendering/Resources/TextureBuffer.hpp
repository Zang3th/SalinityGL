#include "stb_image.hpp"
#include "ErrorManager.hpp"
#include "glm.hpp"

#include <cstring>

namespace Engine
{
    class TextureBuffer
    {
        friend class GLTexture;

        private:
            struct PxColor
            {
                unsigned char r;
                unsigned char g;
                unsigned char b;
            };

            uint32         _initStatus, _width, _height, _channels;
            GLenum         _format;
            bool           _saveBackup;
            unsigned char *_pxBuffer, *_backupBuffer;

            [[nodiscard]] uint32 Init(const std::string& filepath);

            [[nodiscard]] unsigned char* GetRawData() const;
            [[nodiscard]] bool           GetPxColor(uint32 x, uint32 y, PxColor* colorOut) const;
            [[nodiscard]] bool           SetPxColor(uint32 x, uint32 y, const PxColor& color) const;
            [[nodiscard]] bool           ResetPxColor(uint32 x, uint32 y) const;

        public:
            TextureBuffer(const std::string& filepath, bool saveBackup);
            ~TextureBuffer();

            void ActivateTextureFlipOnLoad() const;
            void DeactivateTextureFlipOnLoad() const;

            [[nodiscard]] uint32 GetInitStatus() const;
            [[nodiscard]] bool   SubsampleArea(uint32 xpos, uint32 ypos, uint32 sampleAmount, glm::uvec3* colorOut) const;
    };
}
