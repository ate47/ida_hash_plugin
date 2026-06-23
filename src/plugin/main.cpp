#include <includes.hpp>
#include <loader.hpp>
#include <lz4.h>
#include <hexrays.hpp>
#include <hash.hpp>
#include <format>

namespace {
    struct ida_local hash_hex_converter_t : public ctree_visitor_t {
      public:
        cfunc_t* func;
        hash_hex_converter_t(cfunc_t* func) : ctree_visitor_t(CV_FAST), func(func) {}

        int idaapi visit_expr(cexpr_t* e) override {
            if (e->op != ctype_t::cot_num) {
                return 0;
            }

            uint64_t number{ e->numval() };

            // qstring str;
            // e->print1(&str, func);
            // msg("visit %s %s %llx\n", get_ctype_name(e->op), str.c_str(), number);

            if (number < 0x100000) {
                return 0; // too small
            }

            const char* algorithm;
            const char* string;

            if (hash::ExtractPtr(number, &algorithm, &string)) {
                tinfo_t t{ e->type };
                e->cleanup();

                carglist_t* args{ new carglist_t };
                carg_t& str{ args->push_back() };

                str.op = cot_str;
                str.string = qstrdup(string);
                // fixme: find better way than that
                str.type = t;

                e->assign(*call_helper(t, args, "%s", algorithm));
            }

            return 0;
        }
    };

    class plugin_ctx_t : public plugmod_t {
      public:
        plugin_ctx_t() {
            msg("Loading IDAHashPlugin\n");

            hash::LoadHashDir();
            install_hexrays_callback(hr_callback, nullptr);
        }
        ~plugin_ctx_t() {
            remove_hexrays_callback(hr_callback, nullptr);
            term_hexrays_plugin();
        }

        static ssize_t idaapi hr_callback(void* ud, hexrays_event_t event, va_list va) {

            if (event == hxe_maturity) {
                cfunc_t* cfunc = va_arg(va, cfunc_t*);
                ctree_maturity_t mat = va_argi(va, ctree_maturity_t);
                if (mat == CMAT_FINAL) {
                    hash_hex_converter_t v{ cfunc };
                    v.apply_to_exprs(&cfunc->body, nullptr);
                }
            }
            return 0;
        }

        bool idaapi run(size_t) override { return true; }
    };

    plugmod_t* idaapi init(void) {
        if (!init_hexrays_plugin()) {
            return PLUGIN_SKIP; // no decompiler
        }
        return new plugin_ctx_t();
    }
} // namespace

plugin_t PLUGIN = {
    IDP_INTERFACE_VERSION,
    PLUGIN_MULTI, // plugin flags
    init,
    nullptr,
    nullptr,
    "IDAHash plugin for Hex-Rays decompiler", // long comment about the plugin
    nullptr,                                  // multiline help about the plugin
    "Hex-Rays Hash converter",                // the preferred short name of the plugin
    nullptr,                                  // the preferred hotkey to run the plugin
};