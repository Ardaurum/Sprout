#pragma once

#include "Sprout/Core/Core.h"
#include "Sprout/Core/Identifier.h"

#include <unordered_map>
#include <variant>

#if SPROUT_DEBUG
#define SCLI_DEFINE_ARG(name, type, symbol, description) CLISystem::Get().Define(Identifier32(name), type, symbol, name, description);
#else
#define SCLI_DEFINE_ARG(name, type, symbol, description) CLISystem::Get().Define(Identifier32(name), type, symbol);
#endif

#define SCLI_ID(name) Identifier32(#name)

namespace Sprout
{
    class CLISystem
    {
    public:
        enum class Type : uint8_t
        {
            Boolean,
            Number,
            String,

            Count
        };

        CLISystem(CLISystem const&) = delete;
        CLISystem(CLISystem const&&) = delete;

        void ParseArgs(int const argc, char const* const* argv);
        void Define(ID32 const identifier, Type const optionType, char const symbol = 0, char const* debugName = "", char const* description = "");
        bool HasValue(ID32 const arg) const;
        bool IsTrue(ID32 const arg) const;
        double GetNumber(ID32 const arg) const;
        char const* GetString(ID32 const arg) const;
        std::string_view GetStringView(ID32 const arg) const;
        
        static inline CLISystem& Get() { return Instance; }

    private:
        static CLISystem Instance;

        CLISystem();
        ~CLISystem();

        struct CLIDefinition
        {
        public:
            CLIDefinition()
                : OptionType(Type::Boolean), Symbol(0), DebugName(""), Description("")
            { }

            CLIDefinition(Type const optionType, char const symbol = 0, char const* debugName = "", char const* description = "") noexcept
                : OptionType(optionType), Symbol(symbol), DebugName(debugName), Description(description)
            { }

            Type OptionType;
            char Symbol;
            std::string DebugName;
            std::string Description;
        };

        std::unordered_map<char, ID32> SymbolMap;
        std::unordered_map<ID32, CLIDefinition> Definitions;

        using CLIValue = std::variant<std::string, double>;
        std::unordered_map<ID32, CLIValue> Values;
    };
}