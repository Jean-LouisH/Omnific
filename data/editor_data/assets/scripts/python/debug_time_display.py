import omnia
import time

class omnia_script:

    def __init__(self):
        pass

    def on_logic_frame(self):

        if omnia.has_component("GUI"):
            current_time_string = time.strftime("%H:%M:%S", time.localtime())
            gui = omnia.get_component("GUI")
            # ui_text_label.set_text("Time: " + current_time_string)
