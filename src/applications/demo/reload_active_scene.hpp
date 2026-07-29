#pragma once

#include <systems/cpp_scripting_system/cpp_script_instance.hpp> 

namespace OmnificDemo
{
    class ReloadActiveScene : public Omnific::CPPScriptInstance
    {
    public:
        /* Custom classes should derive their own type string
		and update the base type name.*/
		ReloadActiveScene()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "ReloadActiveScene";

		/* Custom classes should have their own clone method 
		for the CPPScriptAssembly instance derivation. */
		virtual CPPScriptInstance* instance() override
		{
			return new ReloadActiveScene(*this);
		}

		/* Add all the methods from CPPScript to override. */
        virtual void on_input() override;

		/* custom public methods. */
        private:
    };
}