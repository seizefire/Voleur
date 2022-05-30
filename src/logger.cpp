#include <iostream>

#include "logger.h"

void log_error(std::string message){
	std::cout << "\x1b[31;1m!ERROR!\x1b[0m \x1b[37;1m" + message << std::endl;
}
void log_input(std::string key){
	std::cout << "\x1b[1A\x1b[2K\x1b[32m" + key + ": \x1b[33m";
}
void log_important_key(std::string key, std::string value){
	std::cout << "\x1b[0m\x1b[32m" + key + ": \x1b[33;1m" + value + "\x1b[0m" << std::endl;
}