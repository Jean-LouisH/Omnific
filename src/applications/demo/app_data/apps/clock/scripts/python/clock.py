import omnific
import time

class omnific_script:

    def __init__(self):
        pass

    def on_entity_start(self):
        omnific.get_window().change_title("Omnific Clock")

    def on_update(self):
        omnific.get_component("Label").set_text(time.strftime("%H:%M:%S"))
        pass
