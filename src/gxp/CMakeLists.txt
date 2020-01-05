add_library(gxp
        include/gxp/usse.h
        include/gxp/instructions.h
        include/gxp/disasm.h
        include/gxp/gxp.h
        include/gxp/block.h
        include/gxp/builder.h

        src/usse.cpp
        src/instructions.cpp
        src/disasm.cpp
        src/gxp.cpp
        src/builder.cpp
        src/block.cpp)
target_include_directories(gxp PUBLIC include)
target_link_libraries(gxp PUBLIC util)
