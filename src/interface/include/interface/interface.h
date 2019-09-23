#pragma once

#include <util/spirv.h>

#include <string>

/**
 * Parses command line options and deals with any user interface.
 */
class Interface {
    std::string inputFilePath;
    std::string outputFilePath;

    bool parseParams(int count, char **args);

public:
    int exec(int count, char **args);
};
