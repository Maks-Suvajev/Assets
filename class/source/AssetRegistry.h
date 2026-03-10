#ifndef GFX_ASSETS_MANAGER_H
#define GFX_ASSETS_MANAGER_H

#include "AssetRegistry.h"

// STL
//#include <algorithm>
//#include <ranges>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

#include "Shader.h"
#include "TextureTypes.h"

class AssetRegistry
{
	public:

		AssetRegistry() = default;

        std::vector<std::filesystem::path> getAllFilesOfType(std::filesystem::path directory, std::vector<std::string> fileTypes);

        template<typename T>
        std::filesystem::path getDefaultAssetPath();

		// void resetShaderPaths();
        // void checkPathsAndStore(ShaderProgramFilePaths& foundPaths);
        // std::filesystem::path getDefaultShaderSourceDirPath();
		// ShaderProgramFilePaths loadShaderPaths(ShaderProgramFilenameStrings sourceFileNames);
        // std::vector<ShaderProgramFilePaths> loadShaderPathSet(std::vector<ShaderProgramFilenameStrings> shaderFilenames);
		// void refreshTexturePaths();
        // void populateTexturePaths(std::filesystem::path textureFolderPath);
		// std::vector<std::filesystem::path>& getTexturePaths();
        // void updateTextureFolderPath(std::filesystem::path path);
        // std::filesystem::path getTextureFolderPath();

};



template<typename T>
std::filesystem::path AssetRegistry::getDefaultAssetPath()
{
    if constexpr (std::is_same_v<T, gfx::Texture>)
    {
        #ifdef TEXTURES_DEFAULT_PATH
            return std::filesystem::path(TEXTURES_DEFAULT_PATH);
        #else

            // Construct
            std::filesystem::path sourceFilePath = __FILE__;
            std::filesystem::path sourceDirPath = sourceFilePath.parent_path();
            std::filesystem::path classDirPath = sourceDirPath.parent_path();
            std::filesystem::path assetsBaseDir = classDirPath.parent_path();

            return assetsBaseDir / defaultTexturesFolderName;

        #endif
    }
    else if constexpr (std::is_same_v<T, gfx::Shader>)
    {
        #ifdef SHADER_SOURCE_DEFAULT_PATH
            return std::filesystem::path(SHADER_SOURCE_DEFAULT_PATH);
        #else
            // Construct
            std::filesystem::path sourceFilePath = __FILE__;
            std::filesystem::path sourceDirPath = sourceFilePath.parent_path();
            std::filesystem::path assetsBaseDir = sourceDirPath.parent_path();
            std::filesystem::path renderEngineDir = assetsBaseDir.parent_path();

            return renderEngineDir / shaderModuleProjectName / shaderSourceFolderName;
        #endif
    }
}











#endif










