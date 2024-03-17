#include "Shared.hpp"

namespace {
    namespace format_strings {

        char const* address_begin = R"GEN(
#include <Geode/Bindings.hpp>
#include <Geode/modify/Addresses.hpp>
#include <Geode/modify/Traits.hpp>
#include <Geode/loader/Tulip.hpp>

using namespace geode;
)GEN";

        char const* declare_address = R"GEN(
template <>
uintptr_t geode::modifier::address<{index}>() {{
	static uintptr_t ret = {address};
	return ret;
}}
)GEN";

        char const* declare_metadata_begin = R"GEN(
Result<tulip::hook::HandlerMetadata> geode::modifier::handlerMetadataForAddress(uintptr_t address) {
	static auto s_value = []() {
		std::map<uintptr_t, tulip::hook::HandlerMetadata(*)()> ret;
)GEN";

        char const* declare_metadata = R"GEN(
        {{
        	using FunctionType = {return}(*)({class_name}{const}*{parameter_comma}{parameter_types});
			ret[modifier::address<{index}>()] = +[](){{
				return tulip::hook::HandlerMetadata{{
					.m_convention = geode::hook::createConvention(tulip::hook::TulipConvention::{convention}),
					.m_abstract = tulip::hook::AbstractFunction::from(FunctionType(nullptr)),
				}}; 
			}};
        }}  
)GEN";

        char const* declare_metadata_static = R"GEN(
        {{
        	using FunctionType = {return}(*)({parameter_types});
			ret[modifier::address<{index}>()] = +[](){{
				return tulip::hook::HandlerMetadata{{
					.m_convention = geode::hook::createConvention(tulip::hook::TulipConvention::{convention}),
					.m_abstract = tulip::hook::AbstractFunction::from(FunctionType(nullptr)),
				}};
			}};
        }}  
)GEN";

        char const* declare_metadata_structor = R"GEN(
        {{
        	using FunctionType = void(*)({class_name}*{parameter_comma}{parameter_types});
			ret[modifier::address<{index}>()] = +[](){{
				return tulip::hook::HandlerMetadata{{
					.m_convention = geode::hook::createConvention(tulip::hook::TulipConvention::{convention}),
					.m_abstract = tulip::hook::AbstractFunction::from(FunctionType(nullptr)),
				}};
			}};
        }}  
)GEN";

        char const* declare_metadata_end = R"GEN(
        return ret;
    }();
    if (s_value.count(address) > 0) return geode::Ok(std::move(s_value[address]()));
    return geode::Err("Address is not registered for wrapper");
}
)GEN";

    }
}

std::string generateAddressHeader(Root const& root) {
    std::string output;
    output += format_strings::address_begin;

    return output;

    for (auto& f : root.functions) {
        if (codegen::getStatus(f) == BindStatus::Missing) continue;

        std::string address_str;

        if (codegen::getStatus(f) == BindStatus::Binded) {
            address_str = fmt::format(
                "addresser::getNonVirtual(Resolve<{}>::func(&{}))",
                codegen::getParameterTypes(f.prototype),
                f.prototype.name
            );
        }
        else if (codegen::getStatus(f) == BindStatus::NeedsBinding) {
            address_str = fmt::format("base::get() + 0x{:x}", codegen::platformNumber(f.binds));
        }
        else {
            continue;
        }

        output += fmt::format(
            format_strings::declare_address,
            fmt::arg("address", address_str),
            fmt::arg("index", codegen::getId(&f))
        );
    }

    for (auto& c : root.classes) {
        for (auto& field : c.fields) {
            if (codegen::getStatus(field) == BindStatus::Missing) continue;

            std::string address_str;

            auto fn = field.get_as<FunctionBindField>();

            if (!fn) {
                continue;
            }

            const auto isWindowsCocosCtor = [&] {
                return codegen::platform == Platform::Windows
                    && is_cocos_class(field.parent) 
                    // && codegen::getStatus(field) == BindStatus::Binded
                    && fn->prototype.type != FunctionType::Normal;
            };

            if (codegen::getStatus(field) == BindStatus::NeedsBinding || codegen::platformNumber(field) != -1) {
                if (is_cocos_class(field.parent) && codegen::platform == Platform::Windows) {
                    address_str = fmt::format("base::getCocos() + 0x{:x}", codegen::platformNumber(fn->binds));
                }
                else {
                    address_str = fmt::format("base::get() + 0x{:x}", codegen::platformNumber(fn->binds));
                }
            }
            else if (codegen::shouldAndroidBind(fn)) {
                auto const mangled = generateAndroidSymbol(c, fn);
                address_str = fmt::format( // thumb
                    "reinterpret_cast<uintptr_t>(dlsym(dlopen(\"libcocos2dcpp.so\", RTLD_NOW), \"{}\"))",
                    mangled
                );
            }
            else if (isWindowsCocosCtor()) {
                auto const mangled = generateWindowsSymbol(c, fn);
                address_str = fmt::format(
                    "reinterpret_cast<uintptr_t>(GetProcAddress(GetModuleHandleA(\"libcocos2d.dll\"), \"{}\"))",
                    mangled
                );
            }
            else if (codegen::getStatus(field) == BindStatus::Binded && fn->prototype.type == FunctionType::Normal) {
                address_str = fmt::format(
                    "addresser::get{}Virtual(Resolve<{}>::func(&{}::{}))",
                    str_if("Non", !fn->prototype.is_virtual),
                    codegen::getParameterTypes(fn->prototype),
                    field.parent,
                    fn->prototype.name
                );
            }
            else {
                continue;
            }

            output += fmt::format(
                format_strings::declare_address,
                fmt::arg("address", address_str),
                fmt::arg("index", codegen::getId(&field))
            );
        }
    }

    // TODO: this eats too much of compile time make it opt in maybe

    // output += format_strings::declare_metadata_begin;

    // for (auto& c : root.classes) {
    //     for (auto& field : c.fields) {
    //         std::string address_str;

    //         auto fn = field.get_as<FunctionBindField>();

    //         if (!fn) {
    //             continue;
    //         }

    //         if (codegen::getStatus(field) == BindStatus::Binded) {
    //             address_str = fmt::format(
    //                 "addresser::get{}Virtual(Resolve<{}>::func(&{}::{}))",
    //                 str_if("Non", !fn->beginning.is_virtual),
    //                 codegen::getParameterTypes(fn->beginning),
    //                 field.parent,
    //                 fn->beginning.name
    //             );
    //         }
    //         else if (codegen::getStatus(field) == BindStatus::NeedsBinding) {
    //             address_str = fmt::format("base::get() + 0x{:x}", codegen::platformNumber(fn->binds));
    //         }
    //         else {
    //             continue;
    //         }

    //         char const* used_declare_format;

    //         switch (fn->beginning.type) {
    //             case FunctionType::Normal:
    //                 used_declare_format = format_strings::declare_metadata;
    //                 break;
    //             case FunctionType::Ctor:
    //             case FunctionType::Dtor:
    //                 used_declare_format = format_strings::declare_metadata_structor;
    //                 break;
    //         }

    //         if (fn->beginning.is_static)
    //             used_declare_format = format_strings::declare_metadata_static;

    //         output += fmt::format(
    //             used_declare_format,
    //             fmt::arg("address", address_str),
    //             fmt::arg("class_name", c.name),
    //             fmt::arg("const", str_if(" const ", fn->beginning.is_const)),
    //             fmt::arg("convention", codegen::getModifyConventionName(field)),
    //             fmt::arg("return", bank.getReturn(fn->beginning, c.name)),
    //             fmt::arg("parameters", codegen::getParameters(fn->beginning)),
    //             fmt::arg("parameter_types", codegen::getParameterTypes(fn->beginning)),
    //             fmt::arg("arguments", codegen::getParameterNames(fn->beginning)),
    //             fmt::arg("parameter_comma", str_if(", ", !fn->beginning.args.empty())),
    //             fmt::arg("index", field.field_id)
    //         );
    //     }
    // }

    // output += format_strings::declare_metadata_end;
    return output;
}
