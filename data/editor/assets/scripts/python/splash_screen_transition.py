import omnia

def on_start():
    scene_api = omnia.get_scene_api()
    countdown_timer = scene_api.get_component("CountdownTimer")
    countdown_value = 5.0
    countdown_timer.start(countdown_value)

def on_frame():    
    if omnia.get_scene_api().get_component("CountdownTimer").is_finished():
        omnia.get_scene_api().load_scene("assets/scenes/debug.yml")
