#pragma once

#include <fmt/format.h>

#include <vector>
#include <fstream>

template <typename T>
std::vector<T> loadFileData(const std::string &path) {
    std::ifstream stream(path, std::ios::binary | std::ios::ate);
    if (!stream.is_open())
        throw std::runtime_error(fmt::format("Cannot load file from '{}'.", path));

    size_t size = stream.tellg();
    assert(size % sizeof(T) == 0);
    std::vector<T> data(size / sizeof(T));
    stream.seekg(0, std::ios::beg);
    stream.read(reinterpret_cast<char *>(data.data()), size);
    stream.close();
    return data;
}

template <typename T>
typename T::mapped_type &getOrThrow(T &map, typename T::key_type key) {
    auto reference = map.find(key);
    if (reference == map.end())
        throw std::runtime_error("Missing key in map.");

    return reference->second;
}

inline size_t allocate(const bool *map, size_t mapSize, size_t size, size_t alignment = 1) {
    for (size_t a = 0; a < mapSize - size + 1; a++) {
        if (a % alignment == 0 && !map[a]) {
            size_t start = a;

            do {
                a++;
            } while (a < mapSize && !map[a] && a - start < size);

            if (a - start == size)
                return start;
        }
    }

    return ~0u;
}

template <typename T>
bool contains(std::vector<T> array, T content) {
    return std::find(array.begin(), array.end(), content) != array.end();
}
