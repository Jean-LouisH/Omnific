import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_entity_start(self):
        pass

    def on_input(self):
        if omnific.get_inputs().is_on_release("escape"):
            omnific.load_scene("selection_menu.yml")
        pass
