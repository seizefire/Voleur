#include <conio.h>
#include <iostream>
#include <Windows.h>
#include <tomcrypt.h>

#include "jitbit.h"
#include "logger.h"

extern const ltc_math_descriptor ltm_desc;

int main(int argc, char** argv){
	ltc_mp = ltm_desc;
	HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleMode;
	GetConsoleMode(handleOut, &consoleMode);
	SetConsoleMode(handleOut, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN);
	std::cout << std::endl;
	log_input("Licensor");
	char licensor[256];
	memset(licensor, 0, 256);
	std::cin.getline(licensor, 256);
	std::string code = std::string((char*) jitbit_generate_code((unsigned char*) licensor));
	if(code.length() == 0){
		return 0;
	}
	log_important_key("Serial Key", code);
	log_important_key("Expires", "Apr 01 7461");
	std::cout << std::endl << "Apply? (Y/N): ";
	char key = (char) _getch();
	if(key == 'Y' || key == 'y'){
		std::cout << key << std::endl;
		HKEY hkey;
		LONG result = RegOpenKeyExA(HKEY_CURRENT_USER, (LPCSTR)"Software\\Jitbit\\Macro Recorder", 0, KEY_ALL_ACCESS, &hkey);
		if(result != ERROR_SUCCESS){
			log_error("Unable to find registry key. Have you run Macro Recorder?");
			return 0;
		}
		result = RegSetKeyValueA(hkey, NULL, (LPCSTR) "Email", REG_SZ, (LPCSTR) licensor, (strlen(licensor) + 1) * sizeof(char));
		if(result != ERROR_SUCCESS){
			log_error("Failed to set the licensor in registry");
			return 0;
		}
		result = RegSetKeyValueA(hkey, NULL, (LPCSTR) "Code", REG_SZ, (LPCSTR) code.c_str(), (code.length() + 1) * sizeof(char));
		if(result != ERROR_SUCCESS){
			log_error("Failed to set the serial key in registry");
			return 0;
		}
		RegCloseKey(hkey);
	}else if(key == 'N' || key == 'n'){
		std::cout << key << std::endl;
	}
}
