import lilliputian
import time

def on_frame():
    scene_api = lilliputian.get_scene_api()
    
    if scene_api.has_component("ui_text_label"):
        current_time_string = time.strftime("%H:%M:%S", time.localtime())
        ui_text_label = scene_api.get_ui_text_label()
        ui_text_label.set_text("Time: " + current_time_string)
