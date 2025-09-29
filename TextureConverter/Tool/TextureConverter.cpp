#include "TextureConverter.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* externals
#include <magic_enum.hpp>

//* c++
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////
// Option structure
////////////////////////////////////////////////////////////////////////////////////////////

void TextureConverter::Option::Parse(const std::span<char*>& options) {
	for (size_t i = 0; i < options.size(); ++i) {
		const std::string_view option = options[i];

		if (option == "-mip") { //!< mipmap生成に関するoption
			miplevels = 0;

			if (i + 1 < options.size()) {
				//!< 次のoptionが存在する場合
				const std::string_view nextoption = options[i + 1];

				if (nextoption[0] != '-') {
					//!< 次のoptionが'-'で始まらない場合は、mipmapの数として解釈する
					miplevels = std::stoul(std::string(nextoption));
					++i; //!< 次のoptionは読み飛ばす
				}
			}

		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// TextureConverter class methods
////////////////////////////////////////////////////////////////////////////////////////////

void TextureConverter::ConvertWICToDDS(const std::filesystem::path& filepath, const std::span<char*>& options) {

	if (filepath.extension() == ".dds") {
		std::cout << "[TextureConverter] ConvertWICToDDS (extension is [.dds]) : " << filepath.generic_string() << std::endl;
		return;
	}

	std::cout << "[TextureConverter] ConvertWICToDDS : " << filepath.generic_string() << std::endl;

	// optionの解析
	Option option = {};
	option.Parse(options);

	// imageの読み込み
	DirectX::ScratchImage image = ImportTexture(filepath);

	// mipmapの生成
	if (option.miplevels.has_value()) {
		image = GenerateMipmaps(std::move(image));
	}

	// 圧縮formatの変換
	image = Compress(std::move(image));

	// ddsファイルとして出力
	ExportToDDS(filepath, image);

	std::cout << "[TextureConverter] ConvertWICToDDS complete." << std::endl;

}

void TextureConverter::Usage() {
	// TODO: usageの表示
}

DXGI_FORMAT TextureConverter::ConvertFormatToSRGB(DXGI_FORMAT format) {
	std::cout << "[TextureConverter] ConvertFormatToSRGB : " << magic_enum::enum_name(format) << " -> " << magic_enum::enum_name(DirectX::MakeSRGB(format)) << std::endl;
	return DirectX::MakeSRGB(format);
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

DirectX::ScratchImage TextureConverter::GenerateMipmaps(DirectX::ScratchImage&& image, const Option& option) {

	if (image.GetMetadata().mipLevels > 1) {
		//!< 既にmipmapが存在する場合は生成しない
		std::cout << "[TextureConverter] GenerateMipmaps (already has mipmaps.)" << std::endl;
		return std::move(image);
	}

	DirectX::ScratchImage mipimage = {};

	auto hr = DirectX::GenerateMipMaps(
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT,
		option.miplevels.value(),
		mipimage
	);
	System::Assert(SUCCEEDED(hr), "mipmaps create failed.");

	std::cout << "[TextureConverter] GenerateMipmap complete." << std::endl;
	return mipimage;
}

DirectX::ScratchImage TextureConverter::Compress(DirectX::ScratchImage&& image) {

	if (DirectX::IsCompressed(image.GetMetadata().format)) {
		//!< 既に圧縮formatの場合は変換しない
		std::cout << "[TextureConverter] Compress (already compressed format.)" << std::endl;
		return std::move(image);
	}

	DirectX::ScratchImage compimage = {};

	auto hr = DirectX::Compress(
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		DXGI_FORMAT_BC7_UNORM_SRGB,
		DirectX::TEX_COMPRESS_SRGB_OUT | DirectX::TEX_COMPRESS_PARALLEL,
		1.0f,
		compimage
	);
	System::Assert(SUCCEEDED(hr), "texture compress failed.");

	std::cout << "[TextureConverter] Compress complete." << std::endl;
	return compimage;
}

void TextureConverter::ExportToDDS(const std::filesystem::path& filepath, DirectX::ScratchImage& image) {

	// 出力先のpathを生成
	std::filesystem::path path = filepath;
	path.replace_extension(".dds");

	auto hr = DirectX::SaveToDDSFile(
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		DirectX::DDS_FLAGS_NONE,
		path.generic_wstring().c_str()
	);
	System::Assert(SUCCEEDED(hr), "texture export failed. filepath: " + path.generic_string());

	std::cout << "[TextureConverter] ExportToDDS complete. filename: " << path.filename().generic_string() << std::endl;
}
