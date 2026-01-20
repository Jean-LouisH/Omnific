#pragma once

#include <systems/cpp_scripting_system/cpp_script_instance.hpp> 

namespace OmnificDemo
{
    class ZRotationSineWave : public Omnific::CPPScriptInstance
    {
    public:
        /* Custom classes should derive their own type string
		and update the base type name.*/
		ZRotationSineWave()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "ZRotationSineWave";

		/* Custom classes should have their own clone method 
		for the CPPScriptAssembly instance derivation. */
		virtual CPPScriptInstance* instance() override
		{
			return new ZRotationSineWave(*this);
		}

		/* Add all the methods from CPPScript to override. */
		virtual void on_entity_start() override;
        virtual void on_fixed_update() override;

		/* custom public methods. */
        private:
			float time;
			std::shared_ptr<Omnific::Transform> transform;
    };
}