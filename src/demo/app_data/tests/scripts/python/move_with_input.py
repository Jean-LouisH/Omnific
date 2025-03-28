import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_update(self):
        movement_speed = 1.0
        input = omnific.get_inputs()
        transform = omnific.get_transform()

        if input.is_pressed(["s", "dpad_down"]):
            transform.globally_translate_y(-movement_speed)
        elif input.is_pressed(["w", "dpad_up"]):
            transform.globally_translate_y(movement_speed)
    
