#pragma once

namespace hash {
    constexpr const char* HASH_FILE = "hash_strings.txt";
    const char* FindAlgorithm(uint64_t val, const char* str);
    bool ExtractPtr(uint64_t value, const char** algorithm, const char** string);
	void LoadHashDir();
}