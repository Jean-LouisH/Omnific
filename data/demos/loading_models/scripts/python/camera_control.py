import omnia

def on_logic_frame():
    movement_speed = 1.0
    hid = omnia.get_hid()
    transform = omnia.get_component("Transform")

    if hid.is_pressed(["s", "dpad_down"]):
        transform.globally_translate_z(movement_speed)
    elif hid.is_pressed(["w", "dpad_up"]):
        transform.globally_translate_z(-movement_speed)
    
    if hid.is_on_release("p"):
        omnia.get_component("Camera").toggle_wireframe_mode()
