import omnia

def on_logic_frame():
    scene_api = omnia.get_scene_api()
    transform = scene_api.get_component("Transform")
    transform.rotate_y(1.0)
