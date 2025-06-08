#pragma once

#ifndef INCLUDE_TEXTURE_MANAGER_H
#define INCLUDE_TEXTURE_MANAGER_H

#include <raylib.h>
#include <string>
#include <unordered_map>

class CSafeImage;

class CTextureManager {
public:
    CTextureManager() = default;
    ~CTextureManager();

    CTextureManager(const CTextureManager&) = delete;
    CTextureManager& operator=(const CTextureManager&) = delete;
    CTextureManager(CTextureManager&&) = default;
    CTextureManager& operator=(CTextureManager&&) = default;

    void UnloadTextures();

    void LoadTexture(const char* name, const Image& image);
    void LoadTexture(const char* name, const CSafeImage& image);

    const Texture& GetTexture(const char* name) const;

private:
    std::unordered_map<std::string, Texture2D> m_textures;
};

#endif //INCLUDE_TEXTURE_MANAGER_H
