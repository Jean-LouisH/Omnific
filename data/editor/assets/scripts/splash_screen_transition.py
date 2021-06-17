import lilliputian as lp

def on_start():
    scene_api = lp.get_scene_api()
    component_variant = scene_api.get_this_component_variant(lp.CV.COUNTDOWN_TIMER)
    countdown_timer = component_variant.get_countdown_timer()
    countdown_value = 5.0
    countdown_timer.start(countdown_value)

def on_frame():    
    if lp.get_scene_api().get_this_component_variant(lp.CV.COUNTDOWN_TIMER).get_countdown_timer().is_finished():
        lp.get_scene_api().load_scene("assets/scenes/debug.yml")
