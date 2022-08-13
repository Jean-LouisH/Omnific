import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_start(self):
        timer = omnia.get_component("Timer")
        countdown_value = 5.0
        timer.start(countdown_value)

    def on_logic_frame(self):    
        if omnia.get_component("Timer").is_finished():
            omnia.load_scene("assets/scenes/debug.yml")
