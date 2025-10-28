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

inline constexpr char shaderModuleProjectName[] = "Shaders"; // TODO: Should be moved externally to project config
inline constexpr char shaderSourceFolderName[] = "glsl"; // TODO: same as above

inline constexpr char texturesFolderName[] = "textures"; // TODO: Should be moved externally to project config

// extensions for file types used TODO: possibly moved to config file later
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
		void loadShaderPaths(); //TODO: Duplicate code between two path loading functions. Need to combine this into a single function.
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










