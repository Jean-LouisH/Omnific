import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_update(self):
        movement_speed = 1.0
        inputs = omnific.get_inputs()
        transform = omnific.get_transform()

        if inputs.is_pressed(["s", "dpad_down"]):
            transform.translate_y(-movement_speed)
        if inputs.is_pressed(["w", "dpad_up"]):
            transform.translate_y(movement_speed)
        if inputs.is_pressed(["a", "dpad_left"]):
            transform.translate_x(-movement_speed)
        if inputs.is_pressed(["d", "dpad_right"]):
            transform.translate_x(movement_speed)
        if inputs.is_pressed(["q"]):
            transform.translate_z(movement_speed)
        if inputs.is_pressed(["e"]):
            transform.translate_z(-movement_speed)
    
        if inputs.is_on_release("p"):
            omnific.get_component("Camera").toggle_wireframe_mode()
