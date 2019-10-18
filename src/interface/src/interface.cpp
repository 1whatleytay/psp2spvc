#include <interface/interface.h>

#include <util/util.h>
#include <translator/translator.h>

#include <fmt/printf.h>

#define ERROR_RETURN_IF(cond, text) if (cond) { fmt::print(text); return false; }

bool Interface::parseParams(int count, char **args) {
    for (int a = 1; a < count; a++) {
        if (strcmp(args[a], "-o") == 0) {
            ERROR_RETURN_IF(a + 1 >= count, "Error, no output specified.")
            ERROR_RETURN_IF(!outputFilePath.empty(), "Multiple output files specified.")
            outputFilePath = args[a + 1];
            a++;
        } else if (strcmp(args[a], "-disasm") == 0) {
            config.printDisassembly = true;
        } else if (strcmp(args[a], "-alloc") == 0) {
            config.printAllocations = true;
        } else {
            ERROR_RETURN_IF(!inputFilePath.empty(), "Multiple input files specified.")
            inputFilePath = args[a];
        }
    }

    ERROR_RETURN_IF(inputFilePath.empty(), "You must specify an input.")
    ERROR_RETURN_IF(outputFilePath.empty(), "You must specify an output.")

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
        fmt::printf("Done.");
#ifdef NDEBUG
    } catch (std::runtime_error &e) {
        fmt::print("{}\n", e.what());
    }
#endif

    return 0;
}
