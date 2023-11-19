#pragma once
#include <cstdlib>
#include <exception>
#include <algorithm>
#include <filesystem>

#include "nfd/nfd.hpp"
#include "stb/stb_image_write.h"

#include "Log.h"
#include "Image.h"


inline std::filesystem::path SaveFileDialog()
{
    NFD_Init();

    std::filesystem::path path;
    nfdchar_t* savePath = nullptr;
    const nfdfilteritem_t filterItems[5] = { { "PNG", "png" }, { "JPG", "jpg" }, { "JPEG", "jpeg" }, { "BMP", "bmp" }, { "TGA", "tga" } };
    const nfdresult_t result = NFD_SaveDialog(&savePath, filterItems, 5, NULL, "QRCode.png");
    
    if (result == NFD_OKAY)
    {
        path = savePath;
        NFD_FreePath(savePath);
    }
    else if (result == NFD_CANCEL)
        Log("[NFD] User pressed cancel");
    else // error opening the file
        Err(Local::Get(Local::Item::ErrImageWriterFailedToOpenNFD), NFD_GetError());

    NFD_Quit();
    return path;
}


inline void ImageWriterThrow(bool cond)
{
    if (cond) throw std::runtime_error("");
}


inline void SaveImage(const Image& img)
{
    std::filesystem::path path = SaveFileDialog();
    if (path.empty()) return;
    std::string extension = path.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) { return std::tolower(c); });

    try
    {
        if (extension == ".jpg" || extension == ".jpeg")
            ImageWriterThrow(!stbi_write_jpg(path.string().c_str(), img.Width(), img.Height(), Image::NumOfChannels, img.Data(), 100));
        else if (extension == ".bmp")
            ImageWriterThrow(!stbi_write_bmp(path.string().c_str(), img.Width(), img.Height(), Image::NumOfChannels, img.Data()));
        else if (extension == ".tga")
            ImageWriterThrow(!stbi_write_tga(path.string().c_str(), img.Width(), img.Height(), Image::NumOfChannels, img.Data()));
        else if (extension == ".png")
            ImageWriterThrow(!stbi_write_png(path.string().c_str(), img.Width(), img.Height(), Image::NumOfChannels, img.Data(), img.Width() * Image::NumOfChannels));
        else
        {
            path += ".png";
            ImageWriterThrow(!stbi_write_png(path.string().c_str(), img.Width(), img.Height(), Image::NumOfChannels, img.Data(), img.Width() * Image::NumOfChannels));
        }
    }
    catch (const std::runtime_error&)
    {
        Err(Local::Get(Local::Item::ErrImageWriterFailedToSaveImage), path.string().c_str(), img.Width(), img.Height());
    }
}