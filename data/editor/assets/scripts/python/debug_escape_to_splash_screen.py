import omnia


def on_input():
    if omnia.get_input_api().is_on_release("escape"):
        omnia.get_scene_api().load_scene("assets/scenes/splash_screen.yml")

