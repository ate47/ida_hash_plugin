#include <iostream>
#include <hash_mini.hpp>

// Demo executable, by loading it into IDA with the plugin, it should reflect all the hashes with the right functions.

#ifdef _WIN32
#define NO_INLINE __declspec(noinline)
#else
#define NO_INLINE
#endif

void NO_INLINE WriteHash(uint64_t val) { std::cout << "0x" << std::hex << val << "\n"; }

void NO_INLINE WriteHashes(uint64_t* arr, size_t count) {
    for (size_t i = 0; i < count; i++) {
        WriteHash(arr[i]);
    }
}

int main(int argc, const char* argv[]) {
    constexpr uint64_t vHash64 = hash::Hash64("test");
    constexpr uint64_t vHashIWDVar = hash::HashIWDVar("test");
    constexpr uint64_t vHashIWAsset = hash::HashIWAsset("test");
    constexpr uint64_t vHashJupScr = hash::HashJupScr("test");
    constexpr uint64_t vHashT10OmnVar = hash::HashT10OmnVar("test");
    constexpr uint64_t vHashT10Scr = hash::HashT10Scr("test");
    constexpr uint64_t vHashT10ScrSP = hash::HashT10ScrSP("test");
    constexpr uint64_t vHashT89Scr = hash::HashT89Scr("test");
    constexpr uint64_t vHashT7 = hash::HashT7("test");

    WriteHash(vHash64);
    WriteHash(vHashT7);

    // test hashes generated in the C code
    uint64_t hashes[]{
        vHash64,     vHashIWDVar,   vHashIWAsset, vHashJupScr, vHashT10OmnVar,
        vHashT10Scr, vHashT10ScrSP, vHashT89Scr,  vHashT7,
    };

    // test hashes generated in the IDA view
    static constinit uint64_t hashes_global[]{
        hash::Hash64("test"),       hash::HashIWDVar("test"),    hash::HashIWAsset("test"),
        hash::HashJupScr("test"),   hash::HashT10OmnVar("test"), hash::HashT10Scr("test"),
        hash::HashT10ScrSP("test"), hash::HashT89Scr("test"),    hash::HashT7("test"),
    };

    WriteHashes(hashes, sizeof(hashes) / sizeof(*hashes));
    WriteHashes(hashes_global, sizeof(hashes_global) / sizeof(*hashes_global));

    return 0;
}