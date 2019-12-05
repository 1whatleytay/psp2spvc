#include <interface/interface.h>

#include <util/util.h>
#include <translator/translator.h>

#include <fmt/printf.h>

#define ERROR_RETURN_IF(cond, text) if (cond) { fmt::print(text); return false; }

bool Interface::parseParams(int count, char **args) {
    for (int a = 1; a < count; a++) {
        if (strcmp(args[a], "-o") == 0) {
            ERROR_RETURN_IF(a + 1 >= count, "Error, no output specified.\n")
            ERROR_RETURN_IF(!outputFilePath.empty(), "Multiple output files specified.\n")
            outputFilePath = args[a + 1];
            a++;
        } else if (strcmp(args[a], "-S") == 0) { // Print Disassembly
            config.printDisassembly = true;
        } else if (strcmp(args[a], "-A") == 0) { // Print Register Allocations
            config.printAllocations = true;
        } else if (strcmp(args[a], "-L") == 0) { // Print Optimization Debug Messages
            config.logDebug = true;
        } else if (strcmp(args[a], "-Oreg-space") == 0) { // Optimize Register Space
            config.optimizeRegisterSpace = true;
        } else {
            ERROR_RETURN_IF(!inputFilePath.empty(), "Multiple input files specified.\n")
            inputFilePath = args[a];
        }
    }

    ERROR_RETURN_IF(inputFilePath.empty(), "You must specify an input.\n")
    ERROR_RETURN_IF(outputFilePath.empty(), "You must specify an output.\n")

    return true;
}

#undef ERROR_RETURN_IF

int Interface::exec(int count, char **args) {
    if (!parseParams(count, args)) return 1;

#ifdef NDEBUG
    try {
#endif
        auto spirvData = loadFileData<uint32_t>(inputFilePath);
        CompilerGXP compiler(spirvData, config);
        std::vector<uint8_t> gxpData = compiler.compileData();
        std::ofstream stream(outputFilePath);
        stream.write(reinterpret_cast<char *>(gxpData.data()), gxpData.size());
        stream.close();
        fmt::print("Done.\n");
#ifdef NDEBUG
    } catch (std::runtime_error &e) {
        fmt::print("{}\n", e.what());
    }
#endif

    return 0;
}
