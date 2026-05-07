import omnific
import sys

class omnific_script:

    def __init__(self):
        self.input_string = ""
        pass

    def on_entity_start(self):
        omnific.get_window().resize(480, 600)
        omnific.get_window().change_title("Omnific Audio Player")

    def on_input(self):
        pass

    def on_late_update(self):
        pass