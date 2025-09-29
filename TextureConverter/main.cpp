//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* system
#include <System.h>

//* tool
#include "Tool/TextureConverter.h"

//* external
#include <magic_enum.hpp>

//* c++
#include <cstdint>
#include <iostream>
#include <format>
#include <span>

////////////////////////////////////////////////////////////////////////////////////////////
// Argument enum class
////////////////////////////////////////////////////////////////////////////////////////////
enum class Argument : size_t {
	ApplicationFilepath,
	InputFilepath,
};

////////////////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {

	// argumentの解析
	std::span<char*> arguments(argv, argc);

#ifdef _DEBUG
	std::cout << "> input arguments" << std::endl;
	for (size_t i = 1; i < arguments.size(); ++i) {
		std::cout << std::format("argument[{}] : {}", i, arguments[i]) << std::endl;
	}
#endif

	// argumentの確認
	if (arguments.size() < magic_enum::enum_count<Argument>()) {
		std::cout << std::format("argument error : {} arguments are required.", magic_enum::enum_count<Argument>()) << std::endl;
		TextureConverter::Usage();
		std::exit(EXIT_SUCCESS);
	}

	System::Init();

	// optionの解析
	std::span<char*> options = arguments.subspan(magic_enum::enum_count<Argument>());

#ifdef _DEBUG
	std::cout << "> option" << std::endl;
	for (size_t i = 0; i < options.size(); ++i) {
		std::cout << std::format("option[{}] : {}", i, options[i]) << std::endl;
	}
#endif

	TextureConverter::ConvertWICToDDS(arguments[static_cast<size_t>(Argument::InputFilepath)]);

	System::Term();

#ifdef _DEBUG
	// コンソール表示を確認できるように一時停止
	System::Pause();
#endif
	return 0;

}
