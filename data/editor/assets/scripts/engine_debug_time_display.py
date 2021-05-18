import lilliputian
import time

def on_frame():
    current_time_string = time.strftime("%H:%M:%S", time.localtime())
    entity = lilliputian.get_scene_api().get_calling_entity()
    ui_text_label = entity.get_component("UITextLabel")
    ui_text_label.set_text("Time: " + current_time_string)
