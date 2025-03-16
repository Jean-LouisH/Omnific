import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_entity_start(self):
        pass

    def on_late_update(self):
        if omnific.get_input().is_on_release("escape"):
            omnific.change_to_scene("selection_menu.yml")
        pass
