import omnific_engine as oe


def on_input():
    if oe.get_input_api().is_on_release("escape"):
        oe.get_scene_api().load_scene("assets/scenes/splash_screen.yml")

