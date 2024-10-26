#include "pch.h"
#include "utils.h"
#include "TextureManager.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include "GUI.h"


TextureManager::TextureManager():
    m_AllTextures{}
{
    m_AllFilePaths = std::vector<std::string>
    {
        "CommonFloors.png",
        "CommonRoofs_02.png",
        "RoomEffects01.png",
        "CommonProps_02.png",
        "CrossRoad_02.png",
        "FloatingPlatforms.png",
        "CommonProps.png",
        "ForegroundProps.png",
        "CommonRoofs.png",
        "CommonBackground.png",
        "BlurEffectLevel_01.png",
        "CommonBackground_02.png",
        "Destructables.png",
        "LiftProps.png",
        "CommonProps_03.png",
        "HotSpring.png"
    };

    LoadTextures();
}

TextureManager::~TextureManager()
{
    std::map<std::string, Texture*>::iterator it;

    for (it = m_RuntimeLoadedTextures.begin() ; it != m_RuntimeLoadedTextures.end(); it++)
    {
        delete m_RuntimeLoadedTextures[it->first];
        m_RuntimeLoadedTextures[it->first] = nullptr;
    }
    m_RuntimeLoadedTextures.clear();

    for (int index{ 0 }; index < m_AllTextures.size(); ++index) 
    {
        delete m_AllTextures[index];
        m_AllTextures[index] = nullptr;
    }
    m_AllTextures.clear();
    m_AllFilePaths.clear();
}

std::vector<std::string> TextureManager::GetFilePaths()
{
    return m_AllFilePaths;
}

Texture* TextureManager::GetTexture(int index)
{
    if (index < 0 || index >= m_AllTextures.size()) {
        return nullptr;
    }
    return m_AllTextures[index];
}

Texture* TextureManager::GetTexture(const std::string& filepath)
{
    std::map<std::string, Texture*>::const_iterator findResult{ m_RuntimeLoadedTextures.find(filepath) };
    if (findResult != m_RuntimeLoadedTextures.end()) 
    {
        return m_RuntimeLoadedTextures[filepath];
    }
    else 
    {
        Texture* loadedTexture{ new Texture(filepath) };
        if (loadedTexture->IsCreationOk()) {
            m_RuntimeLoadedTextures.emplace(filepath, loadedTexture);
            return m_RuntimeLoadedTextures[filepath];
        }
        else {
            delete loadedTexture;
            return nullptr;
        }
    }

    return nullptr;
}

void TextureManager::LoadTextures()
{
    std::vector<std::string> filePaths{ ReadResourceDirectiory() };

    for (int index{ 0 }; index < m_AllFilePaths.size(); ++index) {
        m_AllTextures.push_back(new Texture("Texture2D/" + m_AllFilePaths[index]));
    }

}

std::vector<std::string> TextureManager::ReadResourceDirectiory()
{
    return std::vector<std::string>();
}
