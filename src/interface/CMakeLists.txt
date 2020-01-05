add_library(interface
        include/interface/interface.h

        src/interface.cpp)

target_include_directories(interface PUBLIC include)
target_link_libraries(interface PUBLIC util translator)
target_link_libraries(interface PRIVATE fmt)
