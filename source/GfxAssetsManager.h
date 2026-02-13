#ifndef GFX_ASSETS_MANAGER_H
#define GFX_ASSETS_MANAGER_H

#include "AssetsManager.h"
#include <algorithm>
#include <ranges>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

#include "ShaderManager.h"

namespace gfx {

#if  defined(_WIN32)
	inline constexpr char rootString[] =  "C:\\";
#elif defined(__unix__) || defined(__APPLE__) // Very system dependant, needs to be tested
	inline constexpr char rootString[] =  "/";
#else
	inline constexpr char rootString[] =  "C:\\"; // Windows default
#endif

constexpr char defaultTexturesFolderName[] = "textures"; 

// extensions for file types used 
constexpr char vertShaderExtension[] = ".vs";
constexpr char fragShaderExtension[] = ".fs";

const std::vector<std::string> supportedTextureFileTypes = {".png", ".jpg"}; // Evaluated at run time

// Implemented as a singleton, we should only ever need a single instance to pull all of the required assets
class GfxAssetsManager : public AssetsManager
{
	public:

		GfxAssetsManager();
		void resetShaderPaths();
        void checkPathsAndStore(ShaderProgramFilePaths& foundPaths);
        std::filesystem::path getDefaultShaderSourceDirPath();
		ShaderProgramFilePaths loadShaderPaths(ShaderProgramFilenameStrings sourceFileNames);
        std::vector<ShaderProgramFilePaths> loadShaderPathSet(std::vector<ShaderProgramFilenameStrings> shaderFilenames);
		void loadTexturePaths();
        void populateTexturePaths(std::filesystem::path textureFolderPath);
		std::vector<std::filesystem::path> getTexturePaths();
        std::filesystem::path getDefaultTexturesDirPath();

	private:
		std::unordered_map<std::string, ShaderProgramFilePaths> shaderPaths;
		std::vector<std::filesystem::path>                      texturePaths;
		std::vector<std::string>                                textureFileTypes;
        std::filesystem::path                                   textureFolderPath;
        std::filesystem::path                                   shaderFolderPath;
};
}

#endif










