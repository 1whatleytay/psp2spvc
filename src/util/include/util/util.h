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

template<typename T>
typename T::mapped_type &getOrThrow(T &map, typename T::key_type key) {
    auto reference = map.find(key);
    if (reference == map.end())
        throw std::runtime_error("Missing key in map.");

    return reference->second;
}
