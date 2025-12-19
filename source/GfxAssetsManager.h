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

constexpr char shaderModuleProjectName[] = "Shaders"; 
constexpr char shaderSourceFolderName[] = "glsl"; 

constexpr char texturesFolderName[] = "textures"; 

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
		ShaderPaths loadShaderPaths(ShaderFilenameStrings sourceFileNames);
        std::vector<ShaderPaths> loadShaderPathSet(std::vector<ShaderFilenameStrings> shaderFilenames);
		void loadTexturePaths();
		//std::filesystem::path getVertShaderPath(){return shaderPaths.vertexShader;};
		//std::filesystem::path getFragShaderPath(){return shaderPaths.fragmentShader;};
		std::vector<std::filesystem::path> getTexturePaths();

	private:
		std::unordered_map<std::string, ShaderPaths> shaderPaths;
		std::vector<std::filesystem::path> texturePaths;
		std::vector<std::string> textureFileTypes;
};
}

#endif










