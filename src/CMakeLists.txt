add_subdirectory(util)
add_subdirectory(gxp)
add_subdirectory(translator)
add_subdirectory(interface)

add_executable(psp2spvc
        main.cpp)
target_link_libraries(psp2spvc util interface)
