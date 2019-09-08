#include <gxp/builder.h>

#include <gxp/usse.h>
#include <gxp/instructions.h>

#include <fmt/format.h>

#define OFFSET_OF(parent, child) (reinterpret_cast<uintptr_t>(&parent.child) - reinterpret_cast<uintptr_t>(&parent))

namespace gxp {
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
//        usse::SwizzleVec4 swizzleDescription = usse::getSwizzleVec4DontCare();
//        uint32_t swizzleIndex = 0;
//        for (uint32_t a = 0; a < 4; a++) {
//            if (destMask & (1u << a)) {
//                if (swizzleIndex >= source.swizzle.size())
//                    throw std::runtime_error("Swizzle out of bounds.");
//                swizzleDescription[a] = source.swizzle[swizzleIndex++];
//            }
//        }

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
                mov.destination.index / (destBankLayout.needsDiv() ? 2 : 1), // dest_n
                0, // src0_n
                mov.source.index / (srcBankLayout.needsDiv() ? 2 : 1), // src1_n
                0 // src2_n
            ));
        }
    }

    void Block::createPack(usse::RegisterReference source, usse::RegisterReference destination) {
        usse::BankLayout srcBankLayout = usse::BankLayout::destLayout(source.bank);
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
            destination.index, // dest_n
            static_cast<uint32_t>(destination.swizzle[3]) & 0b11u, // comp_sel_3
            1, // scale
            static_cast<uint32_t>(destination.swizzle[1]) & 0b11u, // comp_sel_1
            static_cast<uint32_t>(destination.swizzle[2]) & 0b11u, // comp_sel_2
            source.index, // src1_n
            static_cast<uint32_t>(destination.swizzle[0]) & 0b10u >> 1u, // comp0_sel_bit1
            source.index + source.size / 2, // src2_n
            static_cast<uint32_t>(destination.swizzle[3]) & 0b01u // comp_sel_0_bit0
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
        uint32_t size = usse::getTypeSize(type.type) * type.components / 4;

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
        default:
            throw std::runtime_error("Missing allocation method for bank.");
        }

//        fmt::print("Allocating {} registers of type {} (vec{}[{}]), size {} at index {}.\n",
//            usse::getBankName(bank), usse::getTypeName(type.type), type.components, type.arraySize, reg.size, reg.index);

        return usse::RegisterReference(type, bank, index, size);
    }

    usse::RegisterReference Builder::registerParameter(const Parameter &parameter) {
        size_t index = parameters.size();
        parameters.push_back(parameter);

        usse::RegisterReference reg = allocateRegister(parameters[index].getBank(), parameter.type);

        return reg;
    }

    std::unordered_map<ProgramVarying, usse::RegisterReference> Builder::registerVaryings(
        const std::vector<ProgramVarying> &outputs, const std::vector<ProgramTexCoordInfo> &texCoords) {
        // TODO: This is only for vertex. Either make this method work for fragment or make another method for fragment.
        varyings.varyings_count = outputs.size() + texCoords.size();

        varyings.vertex_outputs1 = 0;
        varyings.vertex_outputs2 = 0;

        for (ProgramVarying output : outputs) {
            if (output == ProgramVarying::Position) continue;
            if (isTexCoordVarying(output))
                throw std::runtime_error("TexCoord passed as regular output to createVaryings.");

            varyings.vertex_outputs1 |= getVaryingBits(output);
        }

        for (ProgramTexCoordInfo texCoord : texCoords) {
            if (!isTexCoordVarying(texCoord.varying))
                throw std::runtime_error("Non-TexCoord passed as TexCoord to createVaryings.");

            uint32_t texCoordIndex = static_cast<uint32_t>(texCoord.varying)
                - static_cast<uint32_t>(ProgramVarying::TexCoord0);

            uint32_t texCoordBits = 0;
            texCoordBits |= (texCoord.componentCount - 1) & 0b11u;

            varyings.vertex_outputs2 |= texCoordBits << (texCoordIndex * 3u);
        }

        std::unordered_map<ProgramVarying, usse::RegisterReference> references;

        for (auto a = static_cast<uint32_t>(ProgramVarying::Position);
            a < static_cast<uint32_t>(ProgramVarying::TexCoord0); a++) {
            auto varying = static_cast<ProgramVarying>(a);

            if (varying == ProgramVarying::Position || varyings.vertex_outputs1 & getVaryingBits(varying)) {
                references[varying] = allocateRegister(usse::RegisterBank::Output,
                    { usse::Type::Float32, 4, 1 });

                oRegPointer += 4;
            }
        }

        for (auto a = static_cast<uint32_t>(ProgramVarying::TexCoord0);
            a <= static_cast<uint32_t>(ProgramVarying::TexCoord9); a++) {
            auto varying = static_cast<ProgramVarying>(a);

            auto texCoordInfo = std::find_if(texCoords.begin(), texCoords.end(),
                [varying](const ProgramTexCoordInfo &info) { return info.varying == varying; });

            if (texCoordInfo != texCoords.end()) {
                references[varying] = allocateRegister(usse::RegisterBank::Output,
                    { usse::Type::Float32, texCoordInfo->componentCount, 1 });

                oRegPointer += texCoordInfo->componentCount;
            }
        }

        for (auto a = static_cast<uint32_t>(ProgramVarying::PointSize);
             a <= static_cast<uint32_t>(ProgramVarying::Clip7); a++) {
            auto varying = static_cast<ProgramVarying>(a);

            if (varyings.vertex_outputs1 & getVaryingBits(varying)) {
                uint32_t varyingSize = varying == ProgramVarying::PointSize ? 1 : 4;

                references[varying] = allocateRegister(usse::RegisterBank::Output,
                    { usse::Type::Float32, varyingSize, 1 });

                oRegPointer += varyingSize;
            }
        }

        return references;
    }

    std::vector<uint8_t> Builder::build() {
        std::vector<uint8_t> data(sizeof(ProgramHeader));

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

        // Parameters
        header.parameterCount = parameters.size();
        header.parametersOffset = data.size() - OFFSET_OF(header, parametersOffset);
        for (const Parameter &param : parameters) {
            ProgramParameterInfo parameter;
            parameter.resourceIndex = param.resourceIndex;
            parameter.arraySize = param.type.arraySize;
            parameter.semantic = static_cast<uint16_t>(param.semantic);
            parameter.config.setType(getParameterTypeFromUSSEType(param.type.type));
            parameter.config.setCategory(param.category);
            parameter.config.setComponentCount(param.type.components);
            parameter.config.setContainerIndex(param.containerIndex);

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
        header.varyingsOffset = data.size() - OFFSET_OF(header, varyingsOffset);
        {
            std::vector<uint8_t> varyingsData(sizeof(varyings));
            std::memcpy(varyingsData.data(), &varyings, sizeof(varyings));
            data.insert(data.end(), varyingsData.begin(), varyingsData.end());
        }

        // Code
        header.primaryRegCount = paRegPointer;
        header.secondaryRegCount = saRegPointer;
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

        return data;
    }

    Builder::Builder() {
        header.magic = gxpMagic;
        header.majorVersion = 1;
        header.minorVersion = 4;
    }
}
