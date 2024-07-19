import omnific
import time

class omnific_script:

    def __init__(self):
        pass

    def on_logic(self):

        if omnific.has_component("GUI"):
            current_time_string = time.strftime("%H:%M:%S", time.localtime())
            gui = omnific.get_component("GUI")
            # ui_text_label.set_text("Time: " + current_time_string)
