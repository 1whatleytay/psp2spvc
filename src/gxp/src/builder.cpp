#include <gxp/builder.h>

#include <gxp/usse.h>
#include <gxp/instructions.h>

#include <fmt/format.h>

#define OFFSET_OF(parent, child) (reinterpret_cast<uintptr_t>(&parent.child) - reinterpret_cast<uintptr_t>(&parent))

namespace gxp {
    constexpr uint16_t containerIndexSA = 14;

    class MovRegisterData {
    public:
        uint8_t destMask = 0;
        int32_t swizzleIndex = 0;
        usse::RegisterReference source;
        usse::RegisterReference destination;
    };

    std::vector<MovRegisterData> splitRegisterF32(usse::RegisterReference source, usse::RegisterReference destination) {
        std::vector<MovRegisterData> data;

        uint32_t destMask = destination.getSwizzleMask();

        for (uint32_t a = 0; a < 2; a++) {
            uint8_t mask = (destMask & (0b11u << (a * 2))) >> (a * 2);

            usse::RegisterReference sourceHalf = source.getComponents(a * 2, 2);
            usse::RegisterReference destinationHalf = destination.getComponents(a * 2, 2);

            if (mask & 0b01u) {
                if (mask & 0b10u) {
                    if (usse::areSwizzlesInMatchingHalf(sourceHalf.swizzle[0], sourceHalf.swizzle[1])) {
                        data.push_back({
                            mask,
                            usse::getSwizzleVec4Index({
                                sourceHalf.swizzle[0],
                                sourceHalf.swizzle[1],
                                usse::SwizzleChannel::DontCare,
                                usse::SwizzleChannel::DontCare,
                            }),
                            source.getComponents(a * 2, 2), // is this source index right?
                            destination.getComponents(a * 2, 2)
                        });
                    } else {
                        // X and Y swizzle seperately
                        data.push_back({
                            0b01,
                            usse::getSwizzleVec4Index({
                                sourceHalf.swizzle[0],
                                usse::SwizzleChannel::DontCare,
                                usse::SwizzleChannel::DontCare,
                                usse::SwizzleChannel::DontCare,
                            }),
                            source.getComponents(a * 2, 2), // copy both components anyway
                            destination.getComponents(a * 2, 2)
                        });
                        data.push_back({
                            0b10,
                            usse::getSwizzleVec4Index({
                                usse::SwizzleChannel::DontCare,
                                sourceHalf.swizzle[1],
                                usse::SwizzleChannel::DontCare,
                                usse::SwizzleChannel::DontCare,
                            }),
                            source.getComponents(a * 2, 2), // copy both components anyway
                            destination.getComponents(a * 2, 2)
                        });
                    }
                } else {
                    data.push_back({
                        0b01,
                        usse::getSwizzleVec4Index({
                            sourceHalf.swizzle[0],
                            usse::SwizzleChannel::DontCare,
                            usse::SwizzleChannel::DontCare,
                            usse::SwizzleChannel::DontCare,
                        }),
                        source.getComponents(a * 2, 2), // copy both components anyway
                        destination.getComponents(a * 2, 2)
                    });
                }
            } else if (mask & 0b10u) {
                data.push_back({
                    0b10,
                    usse::getSwizzleVec4Index({
                        usse::SwizzleChannel::DontCare,
                        sourceHalf.swizzle[1],
                        usse::SwizzleChannel::DontCare,
                        usse::SwizzleChannel::DontCare,
                    }),
                    source.getComponents(a * 2, 2), // copy both components anyway
                    destination.getComponents(a * 2, 2)
                });
            }
        }

        return data;
    }

    void Builder::setType(gxp::ShaderType type) {
        header.type = static_cast<uint8_t>(type);
    }

    ShaderType Builder::getType() {
        return static_cast<ShaderType>(header.type);
    }

    void Block::createNop() {
        instructions.push_back(usse::makeNOP());
    }

    void Block::createMov(usse::RegisterReference source, usse::RegisterReference destination) {
        usse::BankLayout srcBankLayout = usse::BankLayout::srcLayout(source.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        const auto movs = splitRegisterF32(source, destination);

        for (const auto &mov : movs) {
            instructions.push_back(usse::makeVMOV(
                0, // pred
                0, // skipinv
                0, // test_bit_2
                0, // src0_comp_sel
                0, // syncstart
                destBankLayout.extension, // dest_bank_ext
                0, // end_or_src0_bank_ext
                srcBankLayout.extension, // src1_bank_ext
                0, // src2_bank_ext
                0, // move_type
                0, // repeat_count
                0, // nosched
                static_cast<usse::Param>(destination.type.type) & 0b111u, // move_data_type
                0, // test_bit_1
                mov.swizzleIndex, // src0_swiz
                0, // src0_bank_sel
                destBankLayout.number, // dest_bank_sel
                srcBankLayout.number, // src1_bank_sel
                0, // src2_bank_sel
                mov.destMask, // dest_mask
                destBankLayout.getIndex(mov.destination), // dest_n
                0, // src0_n
                srcBankLayout.getIndex(mov.source), // src1_n
                0 // src2_n
            ));
        }
    }

    void Block::createPack(usse::RegisterReference source, usse::RegisterReference destination) {
        usse::BankLayout srcBankLayout = usse::BankLayout::srcLayout(source.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        if (destination.type.type == usse::Type::Signed32 || destination.type.type == usse::Type::Unsigned32
            || source.type.type == usse::Type::Signed32 || source.type.type == usse::Type::Unsigned32)
            throw std::runtime_error("Cannot pack S32/U32 type.");

        usse::Param typeTable[] = {
            1, // Signed8
            4, // Signed16
            0, // Signed32 - Unsupported
            7, // Fixed10
            5, // Float16
            6, // Float32
            0, // Unsigned8
            3, // Unsigned16
            0, // Unsigned32 - Unsupported
            2, // Output8
        };

        instructions.push_back(usse::makeVPCK(
            0, // pred
            0, // skipinv
            0, // nosched
            0, // unknown
            0, // syncstart
            destBankLayout.extension, // dest_bank_ext
            0, // end
            srcBankLayout.extension, // src1_bank_ext
            srcBankLayout.extension, // src2_bank_ext
            0, // repeat_count
            typeTable[static_cast<uint32_t>(source.type.type)], // src_fmt
            typeTable[static_cast<uint32_t>(destination.type.type)], // dest_fmt
            destination.getSwizzleMask(), // dest_mask
            destBankLayout.number, // dest_bank_sel
            srcBankLayout.number, // src1_bank_sel
            srcBankLayout.number, // src2_bank_sel
            destBankLayout.getIndex(destination), // dest_n
            destination.swizzle.size() > 3 ? static_cast<uint32_t>(destination.swizzle[3]) & 0b11u : 0, // comp_sel_3
            false, // scale
            static_cast<uint32_t>(destination.swizzle[1]) & 0b11u, // comp_sel_1
            static_cast<uint32_t>(destination.swizzle[2]) & 0b11u, // comp_sel_2
            srcBankLayout.getIndex(source.getHalf(0)), // src1_n
            static_cast<uint32_t>(destination.swizzle[0]) & 0b10u >> 1u, // comp0_sel_bit1
            srcBankLayout.getIndex(source.getHalf(1)), // src2_n
            static_cast<uint32_t>(destination.swizzle[0]) & 0b01u // comp_sel_0_bit0
        ));
    }


    void Block::createDot(
        usse::RegisterReference first,
        usse::RegisterReference second,
        usse::RegisterReference destination) {
        usse::BankLayout firstBankLayout = usse::BankLayout::srcLayout(first.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        assert(second.bank == usse::RegisterBank::Internal);

        instructions.push_back(usse::makeVDP(
            0, // pred
            0, // skipinv
            0, // clip_plane_enable
            first.type.components == 4, // opcode2
            destBankLayout.extension, // dest_use_bank_ext
            0, // end
            firstBankLayout.extension, // src1_bank_ext
            3, /* Seems to be the normal value... */ // increment_mode
            0, // gpi0_abs
            0, // repeat_count
            0, // nosched
            destination.getSwizzleMask(), // write_mask
            0, // src1_neg
            0, // src1_abs
            0, // clip_plane_n
            destBankLayout.number, // dest_bank
            firstBankLayout.number, // src1_bank
            second.index, // gpi0_n
            destBankLayout.getIndex(destination), // dest_n
            second.getSwizzleIndex(), // gpi0_swiz
            first.swizzle.size() > 3 ? static_cast<usse::Param>(first.swizzle[3]) : 0, // src1_swiz_w
            static_cast<usse::Param>(first.swizzle[2]), // src1_swiz_z
            static_cast<usse::Param>(first.swizzle[1]), // src1_swiz_y
            static_cast<usse::Param>(first.swizzle[0]), // src1_swiz_x
            firstBankLayout.getIndex(first) // src1_n
            ));
    }

    void Block::createSub(
        usse::RegisterReference first,
        usse::RegisterReference second,
        usse::RegisterReference destination) {
        usse::BankLayout firstBankLayout = usse::BankLayout::srcLayout(first.bank);
        usse::BankLayout secondBankLayout = usse::BankLayout::srcLayout(second.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        uint32_t shift = 0;
        uint32_t firstSwizzle = 0;
        for (usse::SwizzleChannel channel : first.swizzle) {
            firstSwizzle |= static_cast<uint32_t>(channel) << shift;
            shift += 3;
        }

        // First/Second sources are flipped so negative effect can be applied to src1. -x + y = y - x
        instructions.push_back(usse::makeVNMAD32(
            0, // pred
            0, // skipinv
            (firstSwizzle >> 10u) & 0b11u, // src1_swiz_10_11
            0, // syncstart
            destBankLayout.extension, // dest_bank_ext
            (firstSwizzle >> 9u) & 0b1u, // src1_swiz_9
            secondBankLayout.extension, // src1_bank_ext
            firstBankLayout.extension, // src2_bank_ext
            second.getSwizzleIndex(), // src2_swiz
            0, // nosched
            destination.getSwizzleMask(), // dest_mask
            0b01, // src1_mod
            0b0, // src2_mod
            (firstSwizzle >> 7u) & 0b11u, // src1_swiz_7_8
            destBankLayout.number, // dest_bank_sel
            secondBankLayout.number, // src1_bank_sel
            firstBankLayout.number, // src2_bank_sel
            destBankLayout.getIndex(destination), // dest_n
            (firstSwizzle >> 0u) & 0b1111111u, // src1_swiz_0_6
            static_cast<usse::Param>(usse::InstructionVNMADOp::Add), // op2
            secondBankLayout.getIndex(second), // src1_n
            firstBankLayout.getIndex(first) // src2_n
        ));
    }

    void Block::createMul(
        usse::RegisterReference first,
        usse::RegisterReference second,
        usse::RegisterReference destination) {
        usse::BankLayout firstBankLayout = usse::BankLayout::srcLayout(first.bank);
        usse::BankLayout secondBankLayout = usse::BankLayout::srcLayout(second.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        uint32_t shift = 0;
        uint32_t firstSwizzle = 0;
        for (usse::SwizzleChannel channel : first.swizzle) {
            firstSwizzle |= static_cast<uint32_t>(channel) << shift;
            shift += 3;
        }

        instructions.push_back(usse::makeVNMAD32(
            0, // pred
            0, // skipinv
            (firstSwizzle >> 10u) & 0b11u, // src1_swiz_10_11
            0, // syncstart
            destBankLayout.extension, // dest_bank_ext
            (firstSwizzle >> 9u) & 0b1u, // src1_swiz_9
            firstBankLayout.extension, // src1_bank_ext
            secondBankLayout.extension, // src2_bank_ext
            second.getSwizzleIndex(), // src2_swiz
            0, // nosched
            destination.getSwizzleMask(), // dest_mask
            0b00, // src1_mod
            0b0, // src2_mod
            (firstSwizzle >> 7u) & 0b11u, // src1_swiz_7_8
            destBankLayout.number, // dest_bank_sel
            firstBankLayout.number, // src1_bank_sel
            secondBankLayout.number, // src2_bank_sel
            destBankLayout.getIndex(destination), // dest_n
            (firstSwizzle >> 0u) & 0b1111111u, // src1_swiz_0_6
            static_cast<usse::Param>(usse::InstructionVNMADOp::Multiply), // op2
            firstBankLayout.getIndex(first), // src1_n
            secondBankLayout.getIndex(second) // src2_n
        ));
    }

    void Block::createReverseSquareRoot(
        usse::RegisterReference source,
        usse::RegisterReference destination) {
        usse::BankLayout srcBankLayout = usse::BankLayout::srcLayout(source.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        usse::Param typeTable[] = {
            0, // Signed8 - Unsupported
            0, // Signed16 - Unsupported
            0, // Signed32 - Unsupported
            2, // Fixed10
            1, // Float16
            0, // Float32
            0, // Unsigned8 - Unsupported
            0, // Unsigned16 - Unsupported
            0, // Unsigned32 - Unsupported
            0, // Output8 - Unsupported
        };

        instructions.push_back(usse::makeVCOMP(
            0, // pred
            0, // skipinv
            typeTable[static_cast<uint32_t>(destination.type.type)], // dest_type
            0, // syncstart
            destBankLayout.extension, // dest_bank_ext
            0, // end
            srcBankLayout.extension, // src1_bank_ext
            0, // repeat_count
            0, // nosched
            static_cast<usse::Param>(usse::InstructionVCOMPOp::ReverseSquareRoot), // op2
            typeTable[static_cast<uint32_t>(source.type.type)], // src_type
            0b00, // src1_mod
            static_cast<usse::Param>(source.swizzle[0]), // src_comp
            destBankLayout.number, // dest_bank
            srcBankLayout.number, // src1_bank
            destBankLayout.getIndex(destination), // dest_n
            srcBankLayout.getIndex(source), // src1_n
            destination.getSwizzleMask() // write_mask
        ));
    }

    void Block::createMin(
        usse::RegisterReference first,
        usse::RegisterReference second,
        usse::RegisterReference destination) {
        usse::BankLayout firstBankLayout = usse::BankLayout::srcLayout(first.bank);
        usse::BankLayout secondBankLayout = usse::BankLayout::srcLayout(second.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        uint32_t shift = 0;
        uint32_t firstSwizzle = 0;
        for (usse::SwizzleChannel channel : first.swizzle) {
            firstSwizzle |= static_cast<uint32_t>(channel) << shift;
            shift += 3;
        }

        instructions.push_back(usse::makeVNMAD32(
            0, // pred
            0, // skipinv
            (firstSwizzle >> 10u) & 0b11u, // src1_swiz_10_11
            0, // syncstart
            destBankLayout.extension, // dest_bank_ext
            (firstSwizzle >> 9u) & 0b1u, // src1_swiz_9
            firstBankLayout.extension, // src1_bank_ext
            secondBankLayout.extension, // src2_bank_ext
            second.getSwizzleIndex(), // src2_swiz
            0, // nosched
            destination.getSwizzleMask(), // dest_mask
            0b00, // src1_mod
            0b0, // src2_mod
            (firstSwizzle >> 7u) & 0b11u, // src1_swiz_7_8
            destBankLayout.number, // dest_bank_sel
            firstBankLayout.number, // src1_bank_sel
            secondBankLayout.number, // src2_bank_sel
            destBankLayout.getIndex(destination), // dest_n
            (firstSwizzle >> 0u) & 0b1111111u, // src1_swiz_0_6
            static_cast<usse::Param>(usse::InstructionVNMADOp::Min), // op2
            firstBankLayout.getIndex(first), // src1_n
            secondBankLayout.getIndex(second) // src2_n
        ));
    }

    void Block::createMax(
        usse::RegisterReference first,
        usse::RegisterReference second,
        usse::RegisterReference destination) {
        usse::BankLayout firstBankLayout = usse::BankLayout::srcLayout(first.bank);
        usse::BankLayout secondBankLayout = usse::BankLayout::srcLayout(second.bank);
        usse::BankLayout destBankLayout = usse::BankLayout::destLayout(destination.bank);

        uint32_t shift = 0;
        uint32_t firstSwizzle = 0;
        for (usse::SwizzleChannel channel : first.swizzle) {
            firstSwizzle |= static_cast<uint32_t>(channel) << shift;
            shift += 3;
        }

        instructions.push_back(usse::makeVNMAD32(
            0, // pred
            0, // skipinv
            (firstSwizzle >> 10u) & 0b11u, // src1_swiz_10_11
            0, // syncstart
            destBankLayout.extension, // dest_bank_ext
            (firstSwizzle >> 9u) & 0b1u, // src1_swiz_9
            firstBankLayout.extension, // src1_bank_ext
            secondBankLayout.extension, // src2_bank_ext
            second.getSwizzleIndex(), // src2_swiz
            0, // nosched
            destination.getSwizzleMask(), // dest_mask
            0b00, // src1_mod
            0b0, // src2_mod
            (firstSwizzle >> 7u) & 0b11u, // src1_swiz_7_8
            destBankLayout.number, // dest_bank_sel
            firstBankLayout.number, // src1_bank_sel
            secondBankLayout.number, // src2_bank_sel
            destBankLayout.getIndex(destination), // dest_n
            (firstSwizzle >> 0u) & 0b1111111u, // src1_swiz_0_6
            static_cast<usse::Param>(usse::InstructionVNMADOp::Max), // op2
            firstBankLayout.getIndex(first), // src1_n
            secondBankLayout.getIndex(second) // src2_n
        ));
    }

    Block::Block(gxp::Builder &parent) : parent(parent) { }

    usse::RegisterBank Parameter::getBank() {
        switch (category) {
        case ParameterCategory::Attribute:
            return usse::RegisterBank::Primary;
        case ParameterCategory::Uniform:
            return usse::RegisterBank::Secondary;
        default:
            return usse::RegisterBank::Invalid;
        }
    }

    Block *Builder::createPrimaryBlock() {
        size_t index = primaryBlocks.size();
        primaryBlocks.push_back(Block(*this));

        return &primaryBlocks[index];
    }

    Block *Builder::createSecondaryBlock() {
        size_t index = secondaryBlocks.size();
        secondaryBlocks.push_back(Block(*this));

        return &secondaryBlocks[index];
    }

    usse::RegisterReference Builder::allocateRegister(usse::RegisterBank bank, usse::DataType type) {
        uint32_t index = 0;
        uint32_t size = usse::getTypeSize(type.type) * type.components * type.arraySize / 4;

        switch (bank) {
        case usse::RegisterBank::Primary:
            index = paRegPointer;
            paRegPointer += size;
            break;
        case usse::RegisterBank::Secondary:
            index = saRegPointer;
            saRegPointer += size;
            break;
        case usse::RegisterBank::Output:
            index = oRegPointer;
            oRegPointer += size;
            break;
        case usse::RegisterBank::Temporary:
            index = tRegPointer;
            tRegPointer += size;
            break;
        case usse::RegisterBank::Internal:
            index = iRegPointer;
            iRegPointer += size;
            break;
        default:
            throw std::runtime_error("Missing allocation method for bank.");
        }

        fmt::print("Allocating {} registers of type {} (vec{}[{}]), size {} at index {}.\n",
            usse::getBankName(bank), usse::getTypeName(type.type), type.components, type.arraySize, size, index);

        return usse::RegisterReference(type, bank, index, size);
    }

    void Builder::freeRegister(usse::RegisterReference reg) {
        if (reg.bank == usse::RegisterBank::Internal && reg.index + reg.size == iRegPointer) {
            iRegPointer -= reg.size;
        }
    }

    usse::RegisterReference Builder::registerParameter(const Parameter &parameter) {
        size_t index = parameters.size();
        parameters.push_back(parameter);

        usse::RegisterReference reg = allocateRegister(parameters[index].getBank(), parameter.type);
        parameters[index].resourceIndex = reg.index;
        parameters[index].containerIndex = 0;

        return reg;
    }

    std::unordered_map<ProgramVarying, usse::RegisterReference> Builder::registerVertexVaryings(
        const std::vector<ProgramVarying> &outputs, const std::vector<ProgramVectorInfo> &texCoords) {
        varyings.varyings_count = outputs.size() + texCoords.size();

        varyings.vertex_outputs1 = 0;
        varyings.vertex_outputs2 = 0;

        for (ProgramVarying output : outputs) {
            if (output == ProgramVarying::Position) continue;
            if (isTexCoordVarying(output))
                throw std::runtime_error("TexCoord passed as regular output to createVaryings.");

            varyings.vertex_outputs1 |= getVertexVaryingBits(output);
        }

        for (ProgramVectorInfo texCoord : texCoords) {
            if (!isTexCoordVarying(texCoord.varying))
                throw std::runtime_error("Non-TexCoord passed as TexCoord to createVaryings.");

            uint32_t texCoordIndex = static_cast<uint32_t>(texCoord.varying)
                - static_cast<uint32_t>(ProgramVarying::TexCoord0);

            uint32_t texCoordBits = 0;
            texCoordBits |= (texCoord.components - 1) & 0b11u;

            varyings.vertex_outputs2 |= texCoordBits << (texCoordIndex * 3u);
        }

        std::unordered_map<ProgramVarying, usse::RegisterReference> references;

        for (auto a = static_cast<uint32_t>(ProgramVarying::Position);
            a < static_cast<uint32_t>(ProgramVarying::TexCoord0); a++) {
            auto varying = static_cast<ProgramVarying>(a);

            if (varying == ProgramVarying::Position || varyings.vertex_outputs1 & getVertexVaryingBits(varying)) {
                references[varying] = allocateRegister(usse::RegisterBank::Output,
                    { usse::Type::Float32, 4, 1 });
            }
        }

        for (auto a = static_cast<uint32_t>(ProgramVarying::TexCoord0);
            a <= static_cast<uint32_t>(ProgramVarying::TexCoord9); a++) {
            auto varying = static_cast<ProgramVarying>(a);

            auto texCoordInfo = std::find_if(texCoords.begin(), texCoords.end(),
                [varying](const ProgramVectorInfo &info) { return info.varying == varying; });

            if (texCoordInfo != texCoords.end()) {
                references[varying] = allocateRegister(usse::RegisterBank::Output,
                    {usse::Type::Float32, texCoordInfo->components, 1 });
            }
        }

        for (auto a = static_cast<uint32_t>(ProgramVarying::PointSize);
             a <= static_cast<uint32_t>(ProgramVarying::Clip7); a++) {
            auto varying = static_cast<ProgramVarying>(a);

            if (varyings.vertex_outputs1 & getVertexVaryingBits(varying)) {
                uint32_t varyingSize = varying == ProgramVarying::PointSize ? 1 : 4;

                references[varying] = allocateRegister(usse::RegisterBank::Output,
                    { usse::Type::Float32, varyingSize, 1 });
            }
        }

        return references;
    }

    std::unordered_map<ProgramVarying, usse::RegisterReference> Builder::registerFragmentVaryings(
        const std::vector<ProgramVectorInfo> &inputs) {
        std::unordered_map<ProgramVarying, usse::RegisterReference> references;

        for (ProgramVectorInfo varying : inputs) {
            usse::DataType type = {usse::Type::Float32, varying.components, 1 };
            usse::RegisterReference reference = allocateRegister(usse::RegisterBank::Primary, type);

            // What the heck is going on with fragment inputs!?!?
            ProgramFragmentInputInfo input;
            input.size = (reference.size - 1) << 4u;
            input.componentInfo = 0b11u << 4u; // 0b11 = Float, 0b10 = Half?
            input.resourceIndex = reference.index;

            input.attributeInfo |= getFragmentVaryingBits(varying.varying); // Id
            input.attributeInfo |= 0x10A000u; // 0x20000000 = Half, 0x10000000 = Fixed, 0x10A000 = Float...
            input.attributeInfo |= (reference.type.components - 1) << 22u; // Component Count
            input.attributeInfo |= 0xFu; // Not a Sampler!

            // Samplers are not yet supported.

            fragmentInputs.push_back(input);

            references[varying.varying] = reference;
        }

        return references;
    }

    usse::RegisterReference Builder::createFragmentOutput(usse::Type type, uint32_t components) {
        varyings.output_comp_count = components;
        varyings.output_param_type = static_cast<uint8_t>(getParameterTypeFromUSSEType(type));

        return usse::RegisterReference({ type, components, 1 },
            usse::RegisterBank::Primary, 0, usse::getTypeSize(type) * components / 4);
    }

    std::vector<uint8_t> Builder::build() {
        std::vector<uint8_t> data(sizeof(ProgramHeader) + sizeof(ProgramVaryings));
        header.varyingsOffset = sizeof(ProgramHeader) - OFFSET_OF(header, varyingsOffset);

        // Strings
        class StringEntry {
        public:
            size_t index = 0;
            std::string text;
        };

        std::vector<StringEntry> stringDB;

        for (const Parameter &param : parameters) {
            StringEntry entry;
            entry.index = data.size();
            entry.text = param.name;
            data.insert(data.end(), entry.text.begin(), entry.text.end());
            data.insert(data.end(), '\0');
            stringDB.push_back(entry);
        }

        // Containers
        header.containerCount = 1;
        header.containerOffset = data.size() - OFFSET_OF(header, containerOffset);
        {
            ProgramContainerInfo info = {
                containerIndexSA, // Container Index
                0, // ??
                0, // Register Index
                static_cast<uint16_t>(saRegPointer) // Register Count
            };
            data.insert(data.end(),
                reinterpret_cast<uint8_t *>(&info),
                reinterpret_cast<uint8_t *>(&info)
                + sizeof(ProgramContainerInfo));
        }

        // Parameters
        header.parameterCount = parameters.size();
        header.parametersOffset = data.size() - OFFSET_OF(header, parametersOffset);
        for (const Parameter &param : parameters) {
            ProgramParameterInfo parameter;
            parameter.resourceIndex = param.resourceIndex;
            parameter.arraySize = param.type.arraySize;
            parameter.semantic = static_cast<uint16_t>(param.semantic);
            parameter.config = createParameterConfig(param.category, getParameterTypeFromUSSEType(param.type.type),
                param.type.components, containerIndexSA);

            auto stringEntry = std::find_if(stringDB.begin(), stringDB.end(), [param](const StringEntry &entry) {
                return entry.text == param.name;
            });
            if (stringEntry == stringDB.end())
                throw std::runtime_error(fmt::format("String \"{}\" is missing from gxp db.", param.name));

            parameter.nameOffset = stringEntry->index - (data.size() + OFFSET_OF(parameter, nameOffset));

            std::vector<uint8_t> paramData(sizeof(parameter));
            std::memcpy(paramData.data(), &parameter, sizeof(parameter));
            data.insert(data.end(), paramData.begin(), paramData.end());
        }

        // Varyings
        if (getType() == ShaderType::Fragment && !fragmentInputs.empty()) {
            varyings.varyings_count = fragmentInputs.size();
            varyings.vertex_outputs1 = data.size()
                - (sizeof(ProgramHeader) + OFFSET_OF(varyings, varyings_count) + sizeof(uint32_t));
            data.insert(data.end(),
                reinterpret_cast<uint8_t *>(fragmentInputs.data()),
                reinterpret_cast<uint8_t *>(fragmentInputs.data())
                + fragmentInputs.size() * sizeof(ProgramFragmentInputInfo));
        }

        // Code
        header.primaryRegCount = paRegPointer;
        header.secondaryRegCount = saRegPointer;
        header.tempRegCount1 = tRegPointer;
        header.tempRegCount2 = tRegPointer; // Difference between both reg counts?
        {
            header.secondaryProgramOffset = data.size() - OFFSET_OF(header, secondaryProgramOffset);
            for (const Block &block : secondaryBlocks) {
                data.insert(data.end(),
                    reinterpret_cast<const uint8_t *>(block.instructions.data()),
                    reinterpret_cast<const uint8_t *>(block.instructions.data())
                    + block.instructions.size() * sizeof(usse::Instruction));
            }
            header.secondaryProgramOffsetEnd = data.size() - OFFSET_OF(header, secondaryProgramOffsetEnd);

            header.primaryProgramOffset = data.size() - OFFSET_OF(header, primaryProgramOffset);
            usse::Instruction phase = usse::makePHAS(
                0, // sprvv
                true, // end
                true, // imm
                0, // src1_bank_ext
                0, // src2_bank_ext
                0, // mode
                0, // rate_hi
                0, // rate_lo_or_nosched
                0, // wait_cond
                0, // temp_count
                0, // src1_bank
                0, // src2_bank
                0, // exe_addr_high
                0, // src1_n_or_exe_addr_mid
                0 // src2_n_or_exe_addr_low
                );
            data.insert(data.end(),
                reinterpret_cast<uint8_t *>(&phase),
                reinterpret_cast<uint8_t *>(&phase) + sizeof(phase));

            header.primaryProgramInstructionCount++;
            for (const Block &block : primaryBlocks) {
                header.primaryProgramInstructionCount += block.instructions.size();
                data.insert(data.end(),
                            reinterpret_cast<const uint8_t *>(block.instructions.data()),
                            reinterpret_cast<const uint8_t *>(block.instructions.data())
                            + block.instructions.size() * sizeof(usse::Instruction));
            }

            usse::BankLayout emitLayout = usse::BankLayout::srcLayout(usse::RegisterBank::Immediate);

            usse::Instruction emit = usse::makeEMIT(
                0, // sideband_high
                0, // src0_bank_ext
                true, // end
                0, // src1_bank_ext
                emitLayout.extension, // src2_bank_ext
                1, /* Target = MTE */ // target
                0, // task_start_or_mte_hi
                1, /* MTE = Vertex */ // task_end_or_mte_lo
                0, // nosched
                0, // sideband_mid
                0, // src0_bank
                0, // incp
                0, // src1_bank
                emitLayout.number, // src2_bank
                0, // sideband_low
                true, // freep
                0, // src0_n
                0, // src1_n
                0 // src2_n
                );
            data.insert(data.end(),
                reinterpret_cast<uint8_t *>(&emit),
                reinterpret_cast<uint8_t *>(&emit) + sizeof(emit));
        }

        std::memcpy(data.data(), &header, sizeof(ProgramHeader));
        std::memcpy(data.data() + sizeof(ProgramHeader), &varyings, sizeof(ProgramVaryings));

        return data;
    }

    Builder::Builder() {
        header.magic = gxpMagic;
        header.majorVersion = 1;
        header.minorVersion = 4;
    }
}
