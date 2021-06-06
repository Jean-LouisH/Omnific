import lilliputian as lp

def on_start():
    countdown_value = 5.0
    countdown_timer = lp.get_scene_api().get_countdown_timer()
    countdown_timer.start(countdown_value)

def on_frame():    
    if lp.get_scene_api().get_countdown_timer().is_finished():
        lp.get_scene_api().load_scene("assets/scenes/debug.yml")
