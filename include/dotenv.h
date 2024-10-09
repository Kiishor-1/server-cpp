#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

// #ifdef _WIN32
// #include <windows.h> // For SetEnvironmentVariable on Windows
// #else
#include <unistd.h> // For POSIX systems
// #endif

namespace dotenv
{
    inline void load_dotenv(const std::string &path = ".env")
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cerr << "Could not open .env file: " << path << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#')
                continue; // Skip comments and empty lines

            size_t delimiterPos = line.find('=');
            if (delimiterPos == std::string::npos)
                continue; // Skip lines without '='

            std::string name = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

#ifdef _WIN32
            // On Windows, use _putenv_s to set environment variables
            if (_putenv_s(name.c_str(), value.c_str()) != 0)
            {
                std::cerr << "Failed to set environment variable: " << name << std::endl;
            }
#else
            // On POSIX systems, use setenv
            if (setenv(name.c_str(), value.c_str(), 1) != 0)
            {
                std::cerr << "Failed to set environment variable: " << name << std::endl;
            }
#endif
        }

        file.close();
    }
}
