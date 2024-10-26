import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_start(self):
        timer = omnific.get_component("Timer")
        countdown_value = 5.0
        timer.start(countdown_value)

    def on_logic(self):    
        if omnific.get_component("Timer").is_finished():
            omnific.load_scene("assets/scenes/debug.yml")
