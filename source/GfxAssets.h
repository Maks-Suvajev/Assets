#ifndef GFX_ASSETS_H
#define GFX_ASSETS_H

#include "Assets.h"
#include <algorithm>
#include <ranges>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

namespace gfx {

#if  defined(_WIN32)
	inline constexpr char rootString[] =  "C:\\";
#elif defined(__unix__) || defined(__APPLE__) // Very system dependant, needs to be tested
	inline constexpr char rootString[] =  "/";
#else
	inline constexpr char rootString[] =  "C:\\"; // Windows default
#endif

inline constexpr char shaderModuleProjectName[] = "Shaders"; 
inline constexpr char shaderSourceFolderName[] = "glsl"; 

inline constexpr char texturesFolderName[] = "textures"; 

// extensions for file types used 
inline constexpr char vertShaderExtension[] = ".vs";
inline constexpr char fragShaderExtension[] = ".fs";

// Define type to hold shader paths
struct ShaderPaths
{
	// Single instance of each for now, will possibly need to become vector if multiple shaders become required
	std::filesystem::path vertexShader;
	std::filesystem::path fragmentShader;
};

// Implemented as a singleton, we should only ever need a single instance to pull all of the required assets
class GfxAssets : public Assets
{
	public:

		GfxAssets();
		void resetShaderPaths();
		void loadShaderPaths();
		void loadTexturePaths();
		std::filesystem::path getVertShaderPath(){return shaderPaths.vertexShader;};
		std::filesystem::path getFragShaderPath(){return shaderPaths.fragmentShader;};
		std::vector<std::filesystem::path> getTexturePaths();

	private:
		ShaderPaths shaderPaths;
		std::vector<std::filesystem::path> texturePaths;
		std::vector<std::string> textureFileTypes;
};
}

#endif










