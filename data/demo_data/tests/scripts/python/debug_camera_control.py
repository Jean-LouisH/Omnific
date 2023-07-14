import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_logic(self):
        movement_speed = 1.0
        input = omnia.get_input()
        transform = omnia.get_component("Transform")

        if input.is_pressed(["down"]):
            transform.translate_z(movement_speed)
        elif input.is_pressed(["up"]):
            transform.translate_z(-movement_speed)
    
        if input.is_on_release("l"):
            omnia.get_component("Camera").toggle_wireframe_mode()
