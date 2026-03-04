
#include "GfxAssetRegistry.h"

namespace gfx {

//Constuctor 
GfxAssetRegistry::GfxAssetRegistry()
: m_textureFileTypes(supportedTextureFileTypes),
  m_textureFolderPath(getDefaultTexturesDirPath()),
  m_shaderFolderPath(getDefaultShaderSourceDirPath())
{
}

std::vector<ShaderProgramFilePaths> GfxAssetRegistry::loadShaderPathSet(std::vector<ShaderProgramFilenameStrings> shaderFilenames)
{
    std::vector<ShaderProgramFilePaths> returnVector;

    for (const auto& shaderFilename : shaderFilenames)
    {
        ShaderProgramFilePaths pulledPaths = loadShaderPaths(shaderFilename);

        if (!pulledPaths.fragmentShader.empty() && !pulledPaths.vertexShader.empty())
        {
            returnVector.push_back(pulledPaths);
        }
    }

    return returnVector;
} 

void GfxAssetRegistry::updateTextureFolderPath(std::filesystem::path path)
{
    m_textureFolderPath = path;
    refreshTexturePaths();
}
    
std::filesystem::path GfxAssetRegistry::getTextureFolderPath()
{
    return m_textureFolderPath;
}

std::filesystem::path GfxAssetRegistry::getDefaultShaderSourceDirPath()
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

void GfxAssetRegistry::checkPathsAndStore(ShaderProgramFilePaths& foundPaths)
{
    bool missingData = false; // Only store the set if there is no data missing

    if (foundPaths.fragmentShader.empty() == true)
    {
        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "ERROR::Fragment shader path not found." << std::endl;
        #endif

        missingData = true;
    }
    
    if (foundPaths.vertexShader.empty() == true)
    {
        std::cout << "ERROR::Vertex shader path not found." << std::endl;
        missingData = true;
    }

    if (missingData == false)
    {
        m_shaderPaths[foundPaths.setName] = foundPaths;
    }
}

ShaderProgramFilePaths GfxAssetRegistry::loadShaderPaths(ShaderProgramFilenameStrings sourceFileNames)
{
    if (m_shaderPaths.contains(sourceFileNames.setName))
    {
        #ifdef ENABLE_DEBUG_MESSAGES

            if (m_shaderPaths[sourceFileNames.setName].vertexShader.filename().string() != sourceFileNames.vertexShader)
            {
                    std::cout << "ERROR::Shader set name found but with differing vertex shader:" << std::endl;
                    std::cout << "ERROR::Loaded vertex shader in set = " << m_shaderPaths[sourceFileNames.setName].vertexShader.filename().string() << std::endl;
                    std::cout << "ERROR::Requested vertex shader     =  " << sourceFileNames.vertexShader << std::endl;
                    std::cout << "ERROR::Please explicitly delete and load new set or change the name if you want different files." << std::endl;
            }

            if (m_shaderPaths[sourceFileNames.setName].fragmentShader.filename().string() != sourceFileNames.fragmentShader)
            {
                    std::cout << "ERROR::Shader set name found but with differing fragment shader:" << std::endl;
                    std::cout << "ERROR::Loaded fragment shader in set = " << m_shaderPaths[sourceFileNames.setName].fragmentShader.filename().string() << std::endl;
                    std::cout << "ERROR::Requested fragment shader     =  " << sourceFileNames.fragmentShader << std::endl;
                    std::cout << "ERROR::Please explicitly delete and load new set or change the name if you want different files." << std::endl;
            }

        #endif

        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "DEBUG::Returning already loaded set." << std::endl;
        #endif

        return m_shaderPaths[sourceFileNames.setName];
    }

    ShaderProgramFilePaths returnPaths;
    returnPaths.setName = sourceFileNames.setName;

    if (m_shaderFolderPath.empty())
    {
        m_shaderFolderPath = getDefaultShaderSourceDirPath();
    }

    std::cout << "Path being search is: " << m_shaderFolderPath.string() << std::endl;
    if (std::filesystem::exists(m_shaderFolderPath) && std::filesystem::is_directory(m_shaderFolderPath))
    {
        for (const auto& file : std::filesystem::directory_iterator(m_shaderFolderPath))
        {
            if (file.is_regular_file())
            {
                if (file.path().filename().string() == sourceFileNames.vertexShader)
                {
                    #ifdef ENABLE_DEBUG_MESSAGES
                        std::cout << "DEBUG::Vertex shader: " << file.path().filename() << std::endl;
                    #endif

                    returnPaths.vertexShader = file.path();
                }

                if (file.path().filename().string() == sourceFileNames.fragmentShader)
                {
                    #ifdef ENABLE_DEBUG_MESSAGES
                        std::cout << "DEBUG::Fragment shader: " << file.path().filename() << std::endl;
                    #endif

                    returnPaths.fragmentShader = file.path();
                }
            }
        }
    }
    else
    {
        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "ERROR::Current shader source directory is invalid: " << m_shaderFolderPath.string() << std::endl;
        #endif
    }

    checkPathsAndStore(returnPaths);

    return returnPaths;
} 

std::filesystem::path GfxAssetRegistry::getDefaultTexturesDirPath()
{
    #ifdef TEXTURES_DEFAULT_PATH
        return std::filesystem::path(TEXTURES_DEFAULT_PATH);
    #else

        // Construct
        std::filesystem::path sourceFilePath = __FILE__;
        std::filesystem::path sourceDirPath = sourceFilePath.parent_path();
        std::filesystem::path assetsBaseDir = sourceDirPath.parent_path();

        return assetsBaseDir / defaultTexturesFolderName;

    #endif
}

void GfxAssetRegistry::populateTexturePaths(std::filesystem::path textureFolderPath)
{
    for (const auto& file : std::filesystem::directory_iterator(textureFolderPath))
    {
        if (file.is_regular_file())
        {
            if (std::find(m_textureFileTypes.begin(), m_textureFileTypes.end(), file.path().extension().string()) != m_textureFileTypes.end())
            {
                #ifdef ENABLE_DEBUG_MESSAGES
                    std::cout << "DEBUG::Texture file found: " << file.path().filename() << std::endl;
                #endif

                m_texturePaths.push_back(file.path());
            }
        }
    }
}

void GfxAssetRegistry::refreshTexturePaths()
{
    m_texturePaths.clear();

    if (m_textureFolderPath.empty())
    {
        m_textureFolderPath = getDefaultTexturesDirPath();
    }

    if (std::filesystem::exists(m_textureFolderPath) && std::filesystem::is_directory(m_textureFolderPath))
    {
        populateTexturePaths(m_textureFolderPath);
    }
    else
    {
        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "ERROR::Texture path directory is not valid: " << m_textureFolderPath.string() << std::endl;
        #endif
    }

    if (m_texturePaths.empty())
    {
        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "ERROR::No textures detected! Please check texture path is configured correctly" << std::endl;
        #endif
    }
}

std::vector<std::filesystem::path>& GfxAssetRegistry::getTexturePaths()
{
    // Always refresh before returning in case new texture was added to folder
    refreshTexturePaths();

    return m_texturePaths;
}

void GfxAssetRegistry::resetShaderPaths()
{
    m_shaderPaths.clear();
}

}






