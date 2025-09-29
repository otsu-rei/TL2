#include "TextureConverter.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* externals
#include <magic_enum.hpp>

//* c++
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
// TextureConverter class methods
////////////////////////////////////////////////////////////////////////////////////////////

void TextureConverter::ConvertWICToDDS(const std::filesystem::path& filepath) {
	const std::filesystem::path& extension = filepath.extension();
	if (extension == ".dds") {
		std::cout << "[TextureConverter] ConvertWICToDDS (extension is [.dds]) : " << filepath.generic_string() << std::endl;
		return;
	}

	std::cout << "[TextureConverter] ConvertWICToDDS : " << filepath.generic_string() << std::endl;

	// imageの読み込み
	DirectX::ScratchImage image = ImportTexture(filepath);

	// ddsファイルとして出力
	ExportToDDS(filepath, image);

	std::cout << "[TextureConverter] complete." << std::endl;

}


DirectX::ScratchImage TextureConverter::ImportFromHDRFile(const std::filesystem::path& filepath) {

	DirectX::ScratchImage image;

	// hdrファイルの読み込み
	auto hr = DirectX::LoadFromHDRFile(
		filepath.generic_wstring().c_str(),
		nullptr,
		image
	);
	System::Assert(SUCCEEDED(hr), "texture import failed. filepath: " + filepath.generic_string());

	return image;
}

DirectX::ScratchImage TextureConverter::ImportFromTGAFile(const std::filesystem::path& filepath) {

	DirectX::ScratchImage image;

	// tgaファイルの読み込み
	auto hr = DirectX::LoadFromTGAFile(
		filepath.generic_wstring().c_str(),
		nullptr,
		image
	);
	System::Assert(SUCCEEDED(hr), "texture import failed. filepath: " + filepath.generic_string());

	return image;
}

DirectX::ScratchImage TextureConverter::ImportFromWICFile(const std::filesystem::path& filepath) {

	DirectX::ScratchImage image;

	DirectX::WIC_FLAGS flags = DirectX::WIC_FLAGS_NONE;

	// wicファイルの読み込み
	auto hr = DirectX::LoadFromWICFile(
		filepath.generic_wstring().c_str(),
		flags,
		nullptr,
		image
	);
	System::Assert(SUCCEEDED(hr), "texture import failed. filepath: " + filepath.generic_string());

	return image;
}

DirectX::ScratchImage TextureConverter::ImportTexture(const std::filesystem::path& filepath) {
	const std::filesystem::path& extension = filepath.extension();

	if (extension == ".hdr") { //!< filenameが".hdr"で終わっている場合
		return ImportFromHDRFile(filepath);

	} else if (extension == ".tga") { //!< filenameが".tga"で終わっている場合
		return ImportFromTGAFile(filepath);

	} else {
		return ImportFromWICFile(filepath);
	}
}

DXGI_FORMAT TextureConverter::ConvertFormatToSRGB(DXGI_FORMAT format) {
	std::cout << "[TextureConverter] ConvertFormatToSRGB : " << magic_enum::enum_name(format) << " -> " << magic_enum::enum_name(DirectX::MakeSRGB(format)) << std::endl;
	return DirectX::MakeSRGB(format);
}

void TextureConverter::ExportToDDS(const std::filesystem::path& filepath, DirectX::ScratchImage& image) {

	// 出力先のpathを生成
	std::filesystem::path path = filepath;
	path.replace_extension(".dds");

	DirectX::TexMetadata metadata = image.GetMetadata();

	// SRGB Formatに変換
	metadata.format = ConvertFormatToSRGB(metadata.format);

	auto hr = DirectX::SaveToDDSFile(
		image.GetImages(),
		image.GetImageCount(),
		metadata,
		DirectX::DDS_FLAGS_NONE,
		path.generic_wstring().c_str()
	);
	System::Assert(SUCCEEDED(hr), "texture export failed. filepath: " + path.generic_string());

}
