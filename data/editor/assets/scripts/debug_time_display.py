import esi
import time

def on_frame():
    scene_api = esi.get_scene_api()
    
    if scene_api.this_has_component(esi.CV.UI_TEXT_LABEL):
        current_time_string = time.strftime("%H:%M:%S", time.localtime())
        ui_text_label = scene_api.get_this_component_variant(esi.CV.UI_TEXT_LABEL).get_ui_text_label()
        ui_text_label.set_text("Time: " + current_time_string)
