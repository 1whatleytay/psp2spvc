add_library(util INTERFACE)

target_include_directories(util INTERFACE include)
target_link_libraries(util INTERFACE fmt spirv-cross-reflect spirv-cross-cpp)

# util is just headers for now. I want to expand the use for this module soon.
