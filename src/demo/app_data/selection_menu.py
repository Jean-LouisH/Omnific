import omnific
import sys


project_name_type_pairs = {
    "pong" : "game",
    "platformer" : "game",
    "fps" : "game",
    "racing" : "game",
    "bullet_hell" : "game",
    "flight_sim" : "game",
    "clock" : "app",
    "calculator" : "app",
    "music_keyboard" : "app",
    "drawing_editor" : "app",
    "scene_hierarchy_test" : "test",
    "haptics_test" : "test",
    "inputs_test" : "test",
    "audio_test" : "test",
    "physics_test" : "test",
    "shader_test" : "test",
    "gui_test": "test",
    "animation_test": "test"
    }


class omnific_script:

    def __init__(self):
        self.project_paths = {}
        self.last_letter = 'a'

        project_names_list = project_name_type_pairs.keys()

        for project_name in project_names_list:
            omnific_script.list_project(self, project_name, project_name_type_pairs[project_name])

        pass

    def on_entity_start(self):
        omnific.get_window().resize(853, 480)
        print("Python: " + sys.version)
        project_print_string = ""

        project_selection_letters = list(self.project_paths.keys())
        project_names_list = list(project_name_type_pairs.keys())

        for i in range(len(project_selection_letters)):
            project_print_string += project_selection_letters[i] + ": " + project_names_list[i] + "\n"
        
        gui = omnific.get_component("GUI")
        gui.set_to_label(
            """Select one of the following by pressing the corresponding letter key

""" + project_print_string)
        pass

    def on_input(self):
        inputs = omnific.get_inputs()
        
        for letter, project_path in self.project_paths.items():
            if inputs.is_on_release(letter):
                omnific.load_scene(project_path)
                break
        pass

    def list_project(self, project_name, project_type):

        menu_item_path = ""
        
        if project_type == "app":
            menu_item_path = "apps/" + project_name + "/scenes/" + project_name + ".yml"
            pass
        elif project_type == "game":
            menu_item_path = "games/" + project_name + "/scenes/" + project_name + ".yml"
            pass
        elif project_type == "test":
            menu_item_path = "tests/scenes/" + project_name + ".yml"
            pass

        self.project_paths.update({self.last_letter : menu_item_path})
        self.last_letter = chr(ord(self.last_letter[0]) + 1)

        pass
