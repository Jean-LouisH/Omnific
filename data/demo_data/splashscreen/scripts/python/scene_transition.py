import omnia

def on_start():
    omnia.get_component("CountdownTimer").start(2.0)

def on_logic_frame():
    countdown_timer =  omnia.get_component("CountdownTimer")
    if (countdown_timer.is_finished()):
        omnia.load_scene("selection_menu/scenes/entry.yml")
