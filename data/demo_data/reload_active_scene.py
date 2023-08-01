import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_start(self):
        pass

    def on_logic(self):
        if omnia.get_input().is_on_release("f5"):
            omnia.reload_active_scene()
        pass
