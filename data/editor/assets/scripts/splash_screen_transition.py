import lilliputian as lp

countdown_value = None
next_scene = None
countdown_timer = None

def on_start():
    global countdown_value, next_scene, countdown_timer
    
    countdown_value = 5.0
    next_scene = "assets/scenes/project_list.yml"
    countdown_timer = lp.get_scene_api().get_countdown_timer()
    countdown_timer.start(countdown_value)

def on_frame():
    global countdown_timer, next_scene
    
    if countdown_timer.is_finished():
        lp.get_scene_api().load_scene(next_scene)
