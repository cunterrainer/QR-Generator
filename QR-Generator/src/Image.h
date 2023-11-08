#pragma once
#include <string>
#include <cstdint>

#include "GLFW/glfw3.h"
#include "qrcodegen/cpp/qrcodegen.hpp"

#include "Log.h"

class Image
{
public:
    static constexpr int NumOfChannels = 4;
private:
    GLubyte* m_Pixel = nullptr;
    GLsizei m_Width = 0;
    GLsizei m_Height = 0;
    GLuint m_GpuImage = 0;
    bool m_Created = false;
private:
    inline void Delete()
    {
        if(m_GpuImage != 0)
            glDeleteTextures(1, &m_GpuImage);
        delete[] m_Pixel;
        m_Width = 0;
        m_Height = 0;
        m_GpuImage = 0;
        m_Pixel = nullptr;
        m_Created = false;
    }

    inline void CreateGpuImage()
    {
        // Create a OpenGL texture identifier
        glGenTextures(1, &m_GpuImage);
        glBindTexture(GL_TEXTURE_2D, m_GpuImage);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Pixel);
    }
public:
    inline ~Image()
    {
        Delete();
    }

    inline void Assign(const qrcodegen::QrCode& qr, int32_t border, size_t scale = 1)
    {
        assert(scale != 0 && "Image::Assign scale can't be less than 1");
        Delete();
        m_Width = (qr.getSize() + border * 2) * scale;
        m_Height = (qr.getSize() + border * 2) * scale;
        m_Pixel = new GLubyte[(unsigned int)(NumOfChannels * m_Width * m_Height)];

        size_t idx = 0;
        for (int32_t y = -border; y < qr.getSize() + border; y++)
        {
            for (int32_t x = -border; x < qr.getSize() + border; x++)
            {
                const uint8_t pixelColor = qr.getModule(x, y) ? 0 : 0xFF;
                for (size_t i = 0; i < scale; ++i)
                {
                    m_Pixel[idx++] = pixelColor;
                    m_Pixel[idx++] = pixelColor;
                    m_Pixel[idx++] = pixelColor;
                    m_Pixel[idx++] = 0xFF;
                }
            }
            
            // copy row scale-1 times
            for (size_t i = 0; i < scale - 1; ++i)
            {
                std::memcpy(&m_Pixel[idx], &m_Pixel[idx - m_Width * NumOfChannels], m_Width * NumOfChannels);
                idx += 4 * m_Width;
            }
        }
        CreateGpuImage();
    }

    inline GLuint GetGpuImage() const { return m_GpuImage; }
    inline int Height() const { return m_Height; }
    inline int Width() const { return m_Width; }
    inline unsigned char* Data() const { return m_Pixel; }
    inline bool Created() const { return m_Created; }
};