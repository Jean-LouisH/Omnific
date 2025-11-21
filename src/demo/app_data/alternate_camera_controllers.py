import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_entity_start(self):
        scene = omnific.get_scene()
        self.camera_entity = scene.get_entity_by_name("Camera")
        self.camera = scene.get_component("Camera", self.camera_entity.get_id())
        self.initial_transform = self.camera_entity.get_transform()
        pass

    def on_input(self):
        inputs = omnific.get_inputs()

        if inputs.is_right_mouse_button_pressed():
            self.camera.set_controller_state_by_string("flyby")
            inputs.set_relative_mouse_mode(True)
        else:
            self.camera.set_controller_state_by_string("none")
            inputs.set_relative_mouse_mode(False)
        pass
