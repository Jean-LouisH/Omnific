from lilliputian import scene

def __init__(self):
    self.countdown_value = 5.0
    self.next_scene = "project_list"

def on_start():
    entity = scene.get_calling_entity()
    countdown_timer = entity.get_component("CountdownTimer")
    countdown_timer.start(self.countdown_value)

def on_frame():
    if countdown_timer.is_finished():
        scene.change_to(self.next_scene)