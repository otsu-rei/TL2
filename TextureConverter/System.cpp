#include "System.h"

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* windows
#include <combaseapi.h>

//* c++
#include <iostream>
#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////////////////
// System class methods
////////////////////////////////////////////////////////////////////////////////////////////

void System::Init() {
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	System::Assert(SUCCEEDED(hr), "CoInitializeEx failed");
}

void System::Term() {
	CoUninitialize();
}

void System::Pause() {
	std::cout << "[System] Pause : Please press Enter key..." << std::endl;
	std::cin.get();
}

void System::Assert(bool expr, const std::string& label, const std::source_location& location) {
	if (expr) [[likely]] {
		return;
	}

	System::Assertion(label, location);
}

void System::Assertion(const std::string& label, const std::source_location& location) {

	std::cout << "[System] Assertion" << std::endl;
	std::cout << "> label" << std::endl;
	std::cout << " " << label << std::endl;
	std::cout << "> location" << std::endl;
	std::cout << " filename: " << location.file_name() << std::endl;
	std::cout << " function: " << location.function_name() << std::endl;
	std::cout << " line:     " << location.line() << std::endl;

	throw std::runtime_error(label);
}
