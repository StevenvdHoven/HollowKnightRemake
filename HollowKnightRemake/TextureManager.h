#pragma once
#include <vector>
#include <map>
#include "Texture.h"

class TextureManager final
{
public:
	TextureManager();
	~TextureManager();

	std::vector<std::string> GetFilePaths();

	Texture* GetTexture(int index);
	Texture* GetTexture(const std::string& filepath);
	void LoadTextures();
private:
	std::vector<std::string> ReadResourceDirectiory();
	

private:
	std::vector<std::string> m_AllFilePaths;
	std::vector<Texture*> m_AllTextures;
	std::map<std::string, Texture*> m_RuntimeLoadedTextures;
};

