#pragma once

#include <gxp/usse.h>

#include <optional>

namespace usse::disasm {
    std::string disassembleReference(usse::RegisterReference reference);
    std::string disassemble(const std::string &name,
        const std::vector<usse::RegisterReference> &sources,
        const usse::RegisterReference *destination = nullptr);
}
