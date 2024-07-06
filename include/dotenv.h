#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

namespace dotenv {
    inline void load_dotenv(const std::string& path = ".env") {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Could not open .env file: " << path << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue; // Skip comments and empty lines

            size_t delimiterPos = line.find('=');
            if (delimiterPos == std::string::npos) continue; // Skip lines without '='

            std::string name = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            // Using putenv instead of setenv
            std::string envSetting = name + "=" + value;
            if (putenv(const_cast<char*>(envSetting.c_str())) != 0) {
                std::cerr << "Failed to set environment variable: " << name << std::endl;
            }
        }

        file.close();
    }
}
