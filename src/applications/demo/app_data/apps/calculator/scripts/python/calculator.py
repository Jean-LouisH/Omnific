import omnific
import sys

class omnific_script:

    def __init__(self):
        self.input_string = ""
        pass

    def on_entity_start(self):
        omnific.get_window().resize(480, 600)
        omnific.get_window().change_title("Omnific Calculator")

        # for i in range(9):
        #     stringified_number = str(i)
        #     omnific.get_scene().add_empty_entity(stringified_number)
        #     number_key_gui = omnific.GUI()
        #     pass
        # pass

    def on_input(self):
        # inputs = omnific.get_inputs()
        
        # for letter, project_path in self.project_paths.items():
        #     if inputs.is_on_release(letter):
        #         omnific.load_scene(project_path)
        #         break
        pass

    def on_late_update(self):
        for event in omnific.query_events("calculator_key_pressed"):
            key_name = event.get_parameters().strings["key_name"]

            if key_name == "=":
                pass
            else:
                input_string += key_name
        pass