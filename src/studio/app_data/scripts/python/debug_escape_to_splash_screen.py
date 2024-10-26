import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_input(self):
        if omnific.get_input().is_on_release("escape"):
            omnific.load_scene("assets/scenes/splash_screen.yml")

