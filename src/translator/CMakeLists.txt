add_library(translator
        include/translator/config.h
        include/translator/translator.h

        src/codes.cpp
        src/translator.cpp)
target_include_directories(translator PUBLIC include)
target_link_libraries(translator PUBLIC util gxp)
