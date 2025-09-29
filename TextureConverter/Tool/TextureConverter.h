#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* system
#include <System.h>

//* external
#include <DirectXTex.h>

//* c++
#include <filesystem>

////////////////////////////////////////////////////////////////////////////////////////////
// TextureConverter class
////////////////////////////////////////////////////////////////////////////////////////////
class TextureConverter {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	static void ConvertWICToDDS(const std::filesystem::path& filepath);

private:

	//=========================================================================================
	// private methods
	//=========================================================================================

	static DirectX::ScratchImage ImportFromHDRFile(const std::filesystem::path& filepath);
	static DirectX::ScratchImage ImportFromTGAFile(const std::filesystem::path& filepath);
	static DirectX::ScratchImage ImportFromWICFile(const std::filesystem::path& filepath);
	static DirectX::ScratchImage ImportTexture(const std::filesystem::path& filepath);

	static DXGI_FORMAT ConvertFormatToSRGB(DXGI_FORMAT format);

	static void ExportToDDS(const std::filesystem::path& filepath, DirectX::ScratchImage& image);

};
