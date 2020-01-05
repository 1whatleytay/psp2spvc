#include <interface/interface.h>

#include <util/util.h>
#include <translator/translator.h>

#include <fmt/printf.h>

void Interface::printHelp() {
    std::string helpMessage = R"(Usage: psp2spvc [options] path/to/input.spv -o path/to/output.gxp

Options:
    -h, --help        Shows help message.
    -S                Print shader assembly to console.
    -A                Print register allocation messages to console.
    -L                Print other debug messages to console.
    -Oreg-space       Enable register space optimization. Required for larger shaders.
)";

    fmt::print(helpMessage);
}

bool Interface::parseParams(int count, char **args) {
    for (int a = 1; a < count; a++) {
        if (strcmp(args[a], "-o") == 0) {
            if (a + 1 >= count) {
                fmt::print("-o option is missing an output path.\n");
                break;
            }

            if (!outputFilePath.empty())
                fmt::print("An output path has already been specified, ignoring \"{}\".\n", args[a + 1]);
            else
                outputFilePath = args[a + 1];

            a++;
        } else if (strcmp(args[a], "-S") == 0) { // Print Disassembly
            config.printDisassembly = true;
        } else if (strcmp(args[a], "-A") == 0) { // Print Register Allocations
            config.printAllocations = true;
        } else if (strcmp(args[a], "-L") == 0) { // Print Optimization Debug Messages
            config.logDebug = true;
        } else if (strcmp(args[a], "-h") == 0 || strcmp(args[a], "--help") == 0) {
            help = true;
        } else if (strcmp(args[a], "-Oreg-space") == 0) { // Optimize Register Space
            config.optimizeRegisterSpace = true;
        } else {
            if (!inputFilePath.empty())
                fmt::print("Unknown argument {}. Ignoring.", args[a]);
            else
                inputFilePath = args[a];
        }
    }

    if (!help) {
        if (inputFilePath.empty())
            fmt::print("You must specify an input.\n");
        else if (outputFilePath.empty())
            fmt::print("You must specify an output path.\n");
        else
            return true;
    }

    printHelp();
    return false;
}

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
