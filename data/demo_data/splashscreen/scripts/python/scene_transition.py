import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_start(self):
        omnia.get_component("CountdownTimer").start(2.0)

    def on_logic_frame(self):
        countdown_timer =  omnia.get_component("CountdownTimer")
        if (countdown_timer.is_finished()):
            omnia.load_scene("selection_menu/scenes/entry.yml")
