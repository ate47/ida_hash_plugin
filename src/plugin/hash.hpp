#pragma once

namespace hash {
    const char* FindAlgorithm(uint64_t val, const char* str);
    bool ExtractPtr(uint64_t value, const char** algorithm, const char** string);
	void LoadHashDir();
}