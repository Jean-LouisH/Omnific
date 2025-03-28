import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_update(self):
        movement_speed = 1.0
        input = omnific.get_inputs()
        transform = omnific.get_transform()

        if input.is_pressed(["s", "dpad_down"]):
            transform.translate_z(movement_speed)
        elif input.is_pressed(["w", "dpad_up"]):
            transform.translate_z(-movement_speed)
    
        if input.is_on_release("p"):
            omnific.get_component("Camera").toggle_wireframe_mode()
