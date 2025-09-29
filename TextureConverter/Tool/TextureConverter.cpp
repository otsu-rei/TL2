#include "TextureConverter.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* c++
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
// TextureConverter class methods
////////////////////////////////////////////////////////////////////////////////////////////

void TextureConverter::ConvertWICToDDS(const std::filesystem::path& filepath) {

	std::cout << "[TextureConverter] ConvertWICToDDS : " << filepath.generic_string() << std::endl;

	// imageの読み込み
	DirectX::ScratchImage image = LoadTexture(filepath);

}


DirectX::ScratchImage TextureConverter::LoadFromHDRFile(const std::filesystem::path& filepath) {
	DirectX::ScratchImage image = {};

	// hdrファイルの読み込み
	auto hr = DirectX::LoadFromHDRFile(
		filepath.generic_wstring().c_str(),
		nullptr,
		image
	);
	System::Assert(SUCCEEDED(hr), "texture load failed. filepath: " + filepath.generic_string());

	return image;
}

DirectX::ScratchImage TextureConverter::LoadFromTGAFile(const std::filesystem::path& filepath) {
	DirectX::ScratchImage image = {};

	// tgaファイルの読み込み
	auto hr = DirectX::LoadFromTGAFile(
		filepath.generic_wstring().c_str(),
		nullptr,
		image
	);
	System::Assert(SUCCEEDED(hr), "texture load failed. filepath: " + filepath.generic_string());

	return image;
}

DirectX::ScratchImage TextureConverter::LoadFromWICFile(const std::filesystem::path& filepath) {
	DirectX::ScratchImage image = {};

	DirectX::WIC_FLAGS flags = DirectX::WIC_FLAGS_NONE;

	/*if (option.encoding == Encoding::Lightness) {
		flags = DirectX::WIC_FLAGS_FORCE_SRGB | DirectX::WIC_FLAGS_DEFAULT_SRGB;

	} else if (option.encoding == Encoding::Intensity) {
		flags = DirectX::WIC_FLAGS_FORCE_RGB;
	}*/

	// wicファイルの読み込み
	auto hr = DirectX::LoadFromWICFile(
		filepath.generic_wstring().c_str(),
		flags,
		nullptr,
		image
	);
	System::Assert(SUCCEEDED(hr), "texture load failed. filepath: " + filepath.generic_string());

	return image;
}

DirectX::ScratchImage TextureConverter::LoadTexture(const std::filesystem::path& filepath) {
	const std::filesystem::path& extension = filepath.extension();

	if (extension == ".hdr") { //!< filenameが".hdr"で終わっている場合
		return LoadFromHDRFile(filepath);

	} else if (extension == ".tga") { //!< filenameが".tga"で終わっている場合
		return LoadFromTGAFile(filepath);

	} else {
		return LoadFromWICFile(filepath);
	}
}
