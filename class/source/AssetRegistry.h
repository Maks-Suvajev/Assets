#ifndef GFX_ASSETS_MANAGER_H
#define GFX_ASSETS_MANAGER_H

#include "AssetRegistry.h"

// STL
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>

#include "ShaderTypes.h"
#include "TextureTypes.h"
#include "Material.h"

class AssetRegistry
{
	public:

		AssetRegistry() = default;

        std::vector<std::filesystem::path> getAllFilesOfType(std::filesystem::path directory, std::vector<std::string> fileTypes);

        template<typename T>
        std::filesystem::path getDefaultAssetPath();
};



template<typename T>
std::filesystem::path AssetRegistry::getDefaultAssetPath()
{
    std::filesystem::path sourceFilePath = __FILE__;
    std::filesystem::path sourceDirPath = sourceFilePath.parent_path();
    std::filesystem::path classDirPath = sourceDirPath.parent_path();
    std::filesystem::path assetsBaseDir = classDirPath.parent_path();

    std::filesystem::path resourcesDir = assetsBaseDir / "resources";

    if constexpr (std::is_same_v<T, gfx::Texture>)
    {
        #ifdef TEXTURES_DEFAULT_PATH
            return std::filesystem::path(TEXTURES_DEFAULT_PATH);
        #else
            // Construct
            return std::canonical(resourcesDir / gfx::defaultTexturesFolderName);

        #endif
    }
    else if constexpr (std::is_same_v<T, gfx::ShaderSource>)
    {
        #ifdef SHADER_SOURCE_DEFAULT_PATH
            return std::filesystem::path(SHADER_SOURCE_DEFAULT_PATH);
        #else
            // Construct
            std::filesystem::path renderEngineDir = assetsBaseDir.parent_path();

            return std::canonical(resourcesDir / gfx::shaderSourceFolderName);
        #endif
    }
    else if constexpr (std::is_same_v<T, gfx::Material>)
    {
        return std::filesystem::path(); // Return default empty path
    }
    else
    {
        static_assert(gfx::always_false<T>::value, "Type provided for AssetRegistry::getDefaultAssetPath() not currently supported.");
    }
}











#endif










