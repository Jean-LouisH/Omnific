#pragma once

#include <systems/cpp_scripting_system/cpp_script_instance.hpp> 

namespace OmnificDemo
{
    class EscapeToSelectionMenu : public Omnific::CPPScriptInstance
    {
    public:
        /* Custom classes should derive their own type string
		and update the base type name.*/
		EscapeToSelectionMenu()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "EscapeToSelectionMenu";

		/* Custom classes should have their own clone method 
		for the CPPScriptAssembly instance derivation. */
		virtual CPPScriptInstance* instance() override
		{
			return new EscapeToSelectionMenu(*this);
		}

		/* Add all the methods from CPPScript to override. */
        virtual void on_input() override;

		/* custom public methods. */
        private:
    };
}