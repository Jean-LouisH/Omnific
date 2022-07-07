import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_start(self):
        countdown_timer = omnia.get_component("CountdownTimer")
        countdown_value = 5.0
        countdown_timer.start(countdown_value)

    def on_logic_frame(self):    
        if omnia.get_component("CountdownTimer").is_finished():
            omnia.load_scene("assets/scenes/debug.yml")
