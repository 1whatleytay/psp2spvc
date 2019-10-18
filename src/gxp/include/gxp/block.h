#pragma once

#include <gxp/usse.h>

#include <vector>

namespace gxp {
    class Builder;

    class Block {
        std::vector<usse::Instruction> instructions;

        void printDisassembly(const std::string &name,
            const std::vector<usse::RegisterReference> &sources,
            const usse::RegisterReference *destination = nullptr);

        explicit Block(Builder &parent);

        friend class gxp::Builder;
    public:
        Builder &parent;

        void createNop();
        void createMov(
            usse::RegisterReference source,
            usse::RegisterReference destination);
        void createPack(
            usse::RegisterReference source,
            usse::RegisterReference destination);
        void createDot(
            usse::RegisterReference first,
            usse::RegisterReference second,
            usse::RegisterReference destination);
        void createAdd(
            usse::RegisterReference first,
            usse::RegisterReference second,
            usse::RegisterReference destination);
        void createSub(
            usse::RegisterReference first,
            usse::RegisterReference second,
            usse::RegisterReference destination);
        void createMul(
            usse::RegisterReference first,
            usse::RegisterReference second,
            usse::RegisterReference destination);
        void createExp(
            usse::RegisterReference source,
            usse::RegisterReference destination);
        void createLog(
            usse::RegisterReference source,
            usse::RegisterReference destination);
        void createReverseSquareRoot(
            usse::RegisterReference source,
            usse::RegisterReference destination);
        void createMin(
            usse::RegisterReference first,
            usse::RegisterReference second,
            usse::RegisterReference destination);
        void createMax(
            usse::RegisterReference first,
            usse::RegisterReference second,
            usse::RegisterReference destination);
    };
}
