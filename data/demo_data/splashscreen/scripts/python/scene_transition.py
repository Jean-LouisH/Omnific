import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_start(self):
        omnia.get_component("Timer").start(2.0)

    def on_logic(self):
        timer = omnia.get_component("Timer")
        if (timer.is_finished()):
            omnia.load_scene("selection_menu/scenes/entry.yml")
