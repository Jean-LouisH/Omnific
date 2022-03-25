import omnia

def on_logic_frame():
    movement_speed = 1.0
    scene_api = omnia.get_scene_api()
    input_api = omnia.get_input_api()
    transform = scene_api.get_component("Transform")

    if input_api.is_pressed(["s", "dpad_down"]):
        transform.globally_translate_y(-movement_speed)
    elif input_api.is_pressed(["w", "dpad_up"]):
        transform.globally_translate_y(movement_speed)
    
