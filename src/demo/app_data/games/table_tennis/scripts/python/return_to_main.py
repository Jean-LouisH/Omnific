import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_update(self):
        if omnific.get_inputs().is_on_release("escape"):
            omnific.load_scene("selection_menu/scenes/entry.yml")
        pass
    
