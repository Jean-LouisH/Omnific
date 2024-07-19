import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_start(self):
        pass

    def on_logic(self):
        if omnific.get_input().is_on_release("escape"):
            omnific.change_to_scene("selection_menu.yml")
        pass
