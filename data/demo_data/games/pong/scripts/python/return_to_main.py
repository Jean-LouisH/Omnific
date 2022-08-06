import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_logic_frame(self):
        if omnia.get_input().is_on_release("escape"):
            omnia.load_scene("selection_menu/scenes/entry.yml")
        pass
    
