import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_input(self):
        if omnific.get_inputs().is_on_release("escape"):
            omnific.load_scene("scenes/splash_screen.yml")

