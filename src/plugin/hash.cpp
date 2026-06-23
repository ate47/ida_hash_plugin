#include <includes.hpp>
#include <hash_mini.hpp>
#include <hash.hpp>
#include <diskio.hpp>
#include <iostream>
#include <fstream>
#include <unordered_map>

namespace hash {
    namespace {
        struct AlgorithmData {
            const char* alg;
            uint64_t (*hash)(const char* str);
        } Algorithms[]{
            { "__HASH_64__", [](const char* str) -> uint64_t { return hash::Hash64(str); } },
            { "__HASH_32__", [](const char* str) -> uint64_t { return hash::HashX32(str); } },
            { "__HASH_IWDVar__", [](const char* str) -> uint64_t { return hash::HashIWDVar(str); } },
            { "__HASH_IWAsset__", [](const char* str) -> uint64_t { return hash::HashIWAsset(str); } },
            { "__HASH_JupScr__", [](const char* str) -> uint64_t { return hash::HashJupScr(str); } },
            { "__HASH_T10OmnVar__", [](const char* str) -> uint64_t { return hash::HashT10OmnVar(str); } },
            { "__HASH_T10Scr__", [](const char* str) -> uint64_t { return hash::HashT10Scr(str); } },
            { "__HASH_T10ScrSP__", [](const char* str) -> uint64_t { return hash::HashT10ScrSP(str); } },
            { "__HASH_T89Scr__", [](const char* str) -> uint64_t { return hash::HashT89Scr(str); } },
            { "__HASH_T7__", [](const char* str) -> uint64_t { return hash::HashT7(str); } },
        };
        std::unordered_map<uint64_t, const char*> hashes{};

        void AddString(const char* str) {
            if (!str || !*str)
                return; // empty

            for (const AlgorithmData& data : Algorithms) {
                hashes[data.hash(str) & hash::MASK60] = str;
            }
        }
    } // namespace

    const char* FindAlgorithm(uint64_t val, const char* str) {
        val &= hash::MASK60;
        for (const AlgorithmData& data : Algorithms) {
            if ((data.hash(str) & hash::MASK60) == val) {
                return data.alg;
            }
        }
        return "__HASH_UNKNOWN__";
    }

    bool ExtractPtr(uint64_t value, const char** algorithm, const char** string) {
        auto it{ hashes.find(value & hash::MASK60) };
        if (it == hashes.end()) {
            return false;
        }
        *string = it->second;
        *algorithm = FindAlgorithm(value, it->second);
        return true;
    }

    void LoadHashDir() {
        msg("Loading hash directory...\n");

        hashes.clear();

        char cfgpath[QMAXPATH];
        const char* rfile = getsysfile(cfgpath, sizeof(cfgpath), HASH_FILE, CFG_SUBDIR);
        if (rfile == nullptr) {
            msg("Can't find hash list file: %s\n", HASH_FILE);
            return;
        }

        std::ifstream is{ rfile };
        if (!is) {
            msg("Can't read hash list file: %s\n", rfile);
            return;
        }

        std::string line;
        size_t idx{};
        size_t added{};
        while (is && std::getline(is, line)) {
            idx++;
            if (line.empty() || line[0] == '#') {
                continue;
            }

            AddString(strdup(line.data()));
        }
    }
} // namespace hash