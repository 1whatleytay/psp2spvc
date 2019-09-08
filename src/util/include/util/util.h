#pragma once

#include <vector>
#include <fstream>

template <typename T>
std::vector<T> loadFileData(const std::string &path) {
    std::ifstream stream(path, std::ios::binary | std::ios::ate);
    size_t size = stream.tellg();
    assert(size % sizeof(T) == 0);
    std::vector<T> data(size / sizeof(T));
    stream.seekg(0, std::ios::beg);
    stream.read(reinterpret_cast<char *>(data.data()), size);
    stream.close();
    return data;
}
