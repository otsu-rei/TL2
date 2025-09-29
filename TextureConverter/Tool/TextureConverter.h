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
#include <span>

////////////////////////////////////////////////////////////////////////////////////////////
// TextureConverter class
////////////////////////////////////////////////////////////////////////////////////////////
class TextureConverter {
public:

	////////////////////////////////////////////////////////////////////////////////////////////
	// Option structure
	////////////////////////////////////////////////////////////////////////////////////////////
	struct Option {

		void Parse(const std::span<char*>& options);

		//=========================================================================================
		// public variables
		//=========================================================================================

		std::optional<size_t> miplevels = std::nullopt; //!< 生成するmipmapの数
		//! [std::nullopt]: mipmapを生成しない
		//! [0]: 自動生成
		//! [other]: 指定数生成

	};

public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	static void ConvertWICToDDS(const std::filesystem::path& filepath, const std::span<char*>& options);

	static void Usage();

private:

	//=========================================================================================
	// private methods
	//=========================================================================================

	//* helper methods *//

	static DXGI_FORMAT ConvertFormatToSRGB(DXGI_FORMAT format);

	//* process methods *//

	static DirectX::ScratchImage ImportFromHDRFile(const std::filesystem::path& filepath);
	static DirectX::ScratchImage ImportFromTGAFile(const std::filesystem::path& filepath);
	static DirectX::ScratchImage ImportFromWICFile(const std::filesystem::path& filepath);
	static DirectX::ScratchImage ImportTexture(const std::filesystem::path& filepath);

	static DirectX::ScratchImage GenerateMipmaps(DirectX::ScratchImage&& image, const Option& option);

	static DirectX::ScratchImage Compress(DirectX::ScratchImage&& image);

	static void ExportToDDS(const std::filesystem::path& filepath, DirectX::ScratchImage& image);

};
