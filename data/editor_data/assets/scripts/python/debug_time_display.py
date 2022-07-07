import omnia
import time

class omnia_script:

    def __init__(self):
        pass

    def on_logic_frame(self):

        if omnia.has_component("UITextLabel"):
            current_time_string = time.strftime("%H:%M:%S", time.localtime())
            ui_text_label = omnia.get_component("UITextLabel")
            ui_text_label.set_text("Time: " + current_time_string)
