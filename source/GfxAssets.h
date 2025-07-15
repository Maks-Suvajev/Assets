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

		// 
   	 	inline constexpr char vertShaderExtension[] = ".vs";
		inline constexpr char fragShaderExtension[] = ".fs";

		// Implemented as a singleton, we should only ever need a single instance to pull all of the required assets
		class GfxAssets : public Assets
		{
			public:
				GfxAssets();
				std::vector<std::filesystem::path> loadAllShaderPaths();
				std::filesystem::path getShaderPath(std::string shaderExtension);
				std::filesystem::path getVertShaderPath(){return getShaderPath(vertShaderExtension);};
				std::filesystem::path getFragShaderPath(){return getShaderPath(fragShaderExtension);};

			private:
				const std::vector<std::string> shaderFileTypes;
				std::vector<std::filesystem::path> shaderPaths;

				std::filesystem::path vertexShader;
				std::filesystem::path fragmentShader;

		};
	}


#endif










