import omnia
import time

def on_logic_frame():

    if omnia.has_component("UITextLabel"):
        current_time_string = time.strftime("%H:%M:%S", time.localtime())
        ui_text_label = omnia.get_component("UITextLabel")
        ui_text_label.set_text("Time: " + current_time_string)
