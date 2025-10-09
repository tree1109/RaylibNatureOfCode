#include "TextureManager.h"

#include <ranges>
#include <stdexcept>

#include "SafeImage.h"

CTextureManager::~CTextureManager()
{
    for (const auto& texture : m_textures | std::views::values) {
        UnloadTexture(texture);
    }
}

void CTextureManager::UnloadTextures()
{
    for (const auto& texture : m_textures | std::views::values) {
        UnloadTexture(texture);
    }
    m_textures.clear();
}

void CTextureManager::LoadTexture(const char* name, const Image& image)
{
    if (m_textures.contains(name)) {
        throw std::runtime_error("Texture with this name already exists.");
    }

    Texture2D texture = LoadTextureFromImage(image);
    m_textures[name] = texture;
}

void CTextureManager::LoadTexture(const char* name, const CSafeImage& image)
{
    if (m_textures.contains(name)) {
        throw std::runtime_error("Texture with this name already exists.");
    }

    Texture2D texture = LoadTextureFromImage(image.GetImage());
    m_textures[name] = texture;
}

const Texture& CTextureManager::GetTexture(const char* name) const
{
    if (!m_textures.contains(name)) {
        throw std::runtime_error("Texture not found.");
    }

    return m_textures.at(name);
}
