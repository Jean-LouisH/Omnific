import esi


def on_input():
    if esi.get_input_api().is_on_release("escape"):
        esi.get_scene_api().load_scene("assets/scenes/splash_screen.yml")

