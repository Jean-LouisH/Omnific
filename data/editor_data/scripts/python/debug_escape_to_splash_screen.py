import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_input(self):
        if omnia.get_input().is_on_release("escape"):
            omnia.load_scene("assets/scenes/splash_screen.yml")

