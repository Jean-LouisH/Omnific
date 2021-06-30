import lilliputian as lp

def on_input():
    if lp.get_input_api().is_on_release(["q", "p", "button_a"], 0):
        lp.get_log_api().write("one of the inputs was pressed.")
