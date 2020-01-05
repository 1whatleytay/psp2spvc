#pragma once

#include <translator/translator.h>

#include <util/spirv.h>

#include <string>

/**
 * Parses command line options and deals with any user interface.
 */
class Interface {
    std::string inputFilePath;
    std::string outputFilePath;

    bool help = false;

    TranslatorConfig config;

    static void printHelp();
    bool parseParams(int count, char **args);

public:
    int exec(int count, char **args);
};
