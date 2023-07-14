import omnia


project_name_type_pairs = {
    "paper_airplanes": "game",
    "pong": "game",
    "table_tennis": "game",
    "components_demonstration": "test",
    "entity_tree_rotation_demo": "test",
    "event_bus": "test",
    "imgui_test": "test",
    "loading_models": "test",
    "scene_tree_layers": "test"
    }


class omnia_script:

    def __init__(self):
        self.project_paths = {}
        self.last_letter = 'A'

        project_names_list = project_name_type_pairs.keys()

        for project_name in project_names_list:
            omnia_script.list_project(self, project_name, project_name_type_pairs[project_name])

        pass

    def on_start(self):
        project_print_string = ""

        project_selection_letters = list(self.project_paths.keys())
        project_names_list = list(project_name_type_pairs.keys())

        for i in range(len(project_selection_letters)):
            project_print_string += project_selection_letters[i] + ": " + project_names_list[i] + "\n"
        
        gui = omnia.get_component("GUI")
        gui.set_text(
            """Select one of the following by pressing the corresponding letter key

""" + project_print_string)
        pass

    def on_logic(self):
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
