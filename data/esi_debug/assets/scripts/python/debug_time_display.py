import esi
import time

def on_frame():
    scene_api = esi.get_scene_api()
    
    if scene_api.has_component("UITextLabel"):
        current_time_string = time.strftime("%H:%M:%S", time.localtime())
        ui_text_label = scene_api.get_component("UITextLabel")
        ui_text_label.set_text("Time: " + current_time_string)
