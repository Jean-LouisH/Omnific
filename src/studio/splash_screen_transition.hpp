#pragma once

#include <systems/cpp_scripting_system/cpp_script_instance.hpp> 

namespace OmnificEditor
{
    class SplashScreenTransition : public Omnific::CPPScriptInstance
    {
        public:
        /* Custom classes should derive their own type string
		and update the base type name.*/
		SplashScreenTransition()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "SplashScreenTransition";

		/* Custom classes should have their own clone method 
		for the CPPScriptAssembly instance derivation. */
		virtual CPPScriptInstance* instance() override
		{
			return new SplashScreenTransition(*this);
		}

		/* Add all the methods from CPPScript to override. */
		virtual void on_start() override;
        virtual void on_logic() override;

		/* custom public methods. */
        private:
    };
}
