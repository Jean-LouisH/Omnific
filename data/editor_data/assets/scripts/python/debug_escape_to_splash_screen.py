import omnia


def on_input():
    if omnia.get_input().is_on_release("escape"):
        omnia.load_scene("assets/scenes/splash_screen.yml")

