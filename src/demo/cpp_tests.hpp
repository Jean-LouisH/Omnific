#pragma once

#include <systems/cpp_scripting_system/cpp_script_instance.hpp> 

namespace OmnificDemo
{
    class CPPTests : public Omnific::CPPScriptInstance
    {
    public:
        /* Custom classes should derive their own type string
		and update the base type name.*/
		CPPTests()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "CPPTests";

		/* Custom classes should have their own clone method 
		for the CPPScriptAssembly instance derivation. */
		virtual CPPScriptInstance* instance() override
		{
			return new CPPTests(*this);
		}

		/* Add all the methods from CPPScript to override. */
		virtual void on_entity_start() override;
        virtual void on_update() override;

		/* custom public methods. */
        private:
    };
}