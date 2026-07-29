#pragma once

#include <string>
#include <systems/cpp_scripting_system/cpp_script_instance.hpp> 

namespace OmnificDemo
{
    class SelectionMenu : public Omnific::CPPScriptInstance
    {
    public:
        /* Custom classes should derive their own type string
		and update the base type name.*/
		SelectionMenu()
		{
			this->type = TYPE_STRING;
			this->project_name_type_pairs = {
				{"pong" , "game"},
				{"spinning_tops" , "game"},
				//{"flight_combat", "game"},
				//{"racing" , "game"},
				//{"platformer", "game"},
				//{"shooter" , "game"},
				//{"fighting" , "game"},
				{"audio_player" , "app"},
				{"image_viewer" , "app"},
				{"clock" , "app"},
				{"calculator" , "app"},
				{"music_keyboard" , "app"},
				{"drawing_editor" , "app"},
				{"model_viewer" , "app"},
				{"text_editor" , "app"},
				//{"pathfinding" , "simulation"},
				//{"handwriting_recognition" , "simulation"},
				//{"digital_circuits" , "simulation"},
				//{"elastic_collisions" , "simulation"},
				//{"n_body_gravitation" , "simulation"},
				//{"cloth" , "simulation"},
				//{"lightning" , "simulation"},
				//{"water_waves" , "simulation"},
				//{"path_tracing" , "simulation"},
				//{"fluid_dynamics" , "simulation"},
				//{"traffic" , "simulation"},
				{"scene_hierarchy_test" , "test"},
				//{"haptics_test" , "test"},
				//{"inputs_test" , "test"},
				{"audio_test" , "test"},
				//{"physics_test" , "test"},
				{"shader_test" , "test"},
				{"gui_test", "test"}
				//, {"animation_test", "test"}
			};

			for (const auto& pair : this->project_name_type_pairs)
			{
				this->list_project(pair.first, pair.second);
			}
		}

		static constexpr const char* TYPE_STRING = "SelectionMenu";

		/* Custom classes should have their own clone method 
		for the CPPScriptAssembly instance derivation. */
		virtual CPPScriptInstance* instance() override
		{
			return new SelectionMenu(*this);
		}

		/* Add all the methods from CPPScript to override. */
		virtual void on_entity_start() override;
		virtual void on_input() override;
        virtual void list_project(std::string project_name, std::string project_type);

		/* custom public methods. */
        private:
			std::vector<std::pair<std::string, std::string>> project_paths;
			std::string last_letter = "a";
			std::vector<std::pair<std::string, std::string>> project_name_type_pairs;
    };
}