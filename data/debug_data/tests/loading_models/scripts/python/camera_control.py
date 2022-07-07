import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_logic_frame(self):
        movement_speed = 1.0
        input = omnia.get_input()
        transform = omnia.get_component("Transform")

        if input.is_pressed(["s", "dpad_down"]):
            transform.globally_translate_z(movement_speed)
        elif input.is_pressed(["w", "dpad_up"]):
            transform.globally_translate_z(-movement_speed)
    
        if input.is_on_release("p"):
            omnia.get_component("Camera").toggle_wireframe_mode()
