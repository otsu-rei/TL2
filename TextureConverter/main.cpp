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

////////////////////////////////////////////////////////////////////////////////////////////
// Argument enum class
////////////////////////////////////////////////////////////////////////////////////////////
enum class Argument : size_t {
	ApplicationFilepath,
	InputFilepath
};

////////////////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
	// argumentの確認
	System::Assert(argc == magic_enum::enum_count<Argument>(), "invalid argument.");

	System::Init();

	TextureConverter::ConvertWICToDDS(argv[static_cast<size_t>(Argument::InputFilepath)]);

	System::Term();

#ifdef _DEBUG
	// コンソール表示を確認できるように一時停止
	System::Pouse();
#endif
	return 0;

}
