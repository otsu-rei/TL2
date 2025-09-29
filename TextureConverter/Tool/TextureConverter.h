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

	static DirectX::ScratchImage LoadFromHDRFile(const std::filesystem::path& filepath);
	static DirectX::ScratchImage LoadFromTGAFile(const std::filesystem::path& filepath);
	static DirectX::ScratchImage LoadFromWICFile(const std::filesystem::path& filepath);
	static DirectX::ScratchImage LoadTexture(const std::filesystem::path& filepath);



};
