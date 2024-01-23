#include "spch.h"
#include "CLISystem.h"
#include "EnumUtils.h"

namespace Sprout
{
    STRING_ENUM(CLISystem, Type, "Boolean", "Number", "String")

    CLISystem Sprout::CLISystem::Instance;

    CLISystem::CLISystem()
        : SymbolMap()
        , Definitions()
        , Values()
    {
    }

    CLISystem::~CLISystem()
    {
    }

    void CLISystem::ParseArgs(int const argc, char const* const* argv)
    {
        int index = 1;
        while (index < argc)
        {
            char const* currentArg = argv[index];
            if (currentArg[0] != '-')
            {
                SPROUT_CORE_WARN("Expected option or symbol starting with '-'. Got {0} instead.", currentArg);
                ++index;
                continue;
            }

            if (strcmp(currentArg, "--help") == 0)
            {
                SPROUT_CORE_INFO("CLI Args:")
                    for (std::pair<ID32, CLIDefinition> const& def : Definitions)
                    {
                        std::string test = TypeToString(def.second.OptionType);
                        SPROUT_CORE_INFO("--{0} -{1} ({2}) : {3}", def.second.DebugName.c_str(), def.second.Symbol, TypeToString(def.second.OptionType), def.second.Description.c_str())
                    }
                ++index;
                continue;
            }

            ID32 option;
            if (currentArg[1] == '-')
            {
                option = Identifier32(currentArg + 2);
            }
            else
            {
                auto const symbolIt = SymbolMap.find(currentArg[1]);
                SPROUT_CORE_ASSERT_MSG(symbolIt != SymbolMap.end(), "There's no definition for symbol: ", currentArg);
                option = symbolIt->second;
            }

            auto const definitionIt = Definitions.find(option);
            SPROUT_CORE_ASSERT_MSG(definitionIt != Definitions.end(), "There's no definition for identifier: ", currentArg);
            CLIDefinition const& definition = definitionIt->second;

            SPROUT_CORE_ASSERT_MSG(Values.find(option) == Values.end(), "Argument {0} was already parsed!", definition.DebugName);

            switch (definition.OptionType)
            {
            case Type::Boolean:
                ++index;
                SPROUT_CORE_ASSERT_MSG(strcicmp(argv[index], "true") == 0 || strcicmp(argv[index], "false") == 0, "Boolean value requires either true or false value. Got: {0}", argv[index]);
                Values[option] = (strcicmp(argv[index], "true") == 0 ? 1 : 0);
                break;
            case Type::Number:
                ++index;
                Values[option] = std::stod(argv[index]);
                break;
            case Type::String:
                ++index;
                Values[option] = argv[index];
                break;
            default:
                SPROUT_CORE_ERROR("Not supported CLI type: {0}", definition.OptionType);
                break;
            }
            ++index;
        }
    }

    void CLISystem::Define(ID32 const identifier, Type const optionType, char const symbol, char const* debugName, char const* description)
    {
        SPROUT_CORE_ASSERT_MSG(Definitions.find(identifier) == Definitions.end() && (symbol == 0 || SymbolMap.find(symbol) == SymbolMap.end()), "Identifier or symbol is already defined. Identifier: {0}, Symbol: {1}", identifier, symbol);
        SPROUT_CORE_ASSERT_MSG(identifier != Identifier32("help"), "Command Line Argument name cannot be '--help'. Please redefine it.");
#ifdef SPROUT_DEBUG
        for (size_t i = 0; i < strlen(debugName); ++i)
        {
            if (std::isspace(debugName[i]))
            {
                SPROUT_CORE_ERROR("Debug name: {0} can't contain spaces!", debugName);
                break;
            }
        }
#endif

        if (symbol != 0)
        {
            SymbolMap.emplace(symbol, identifier);
        }
        Definitions.emplace(identifier, CLIDefinition(optionType, symbol, debugName, description));
    }

    bool CLISystem::HasValue(ID32 const arg) const
    {
        return Values.find(arg) != Values.end();
    }

    bool CLISystem::IsTrue(ID32 const arg) const
    {
        return std::get<double>(Values.at(arg)) != 0;
    }

    double CLISystem::GetNumber(ID32 const arg) const
    {
        return std::get<double>(Values.at(arg));
    }

    char const* CLISystem::GetString(ID32 const arg) const
    {
        return std::get<std::string>(Values.at(arg)).c_str();
    }

    std::string_view CLISystem::GetStringView(ID32 const arg) const
    {
        return std::get<std::string>(Values.at(arg));
    }
}