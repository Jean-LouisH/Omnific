import lilliputian as lp

def on_input():
    input_string = "q"
    other_input_string = "p"
    if lp.get_input_api().is_on_release([input_string, other_input_string]):
        lp.get_log_api().write("one of the inputs was pressed.")
