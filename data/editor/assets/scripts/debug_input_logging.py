import lilliputian as lp

def on_input():
    input_string = "q"
    if lp.get_input_api().is_on_release(input_string):
        lp.get_log_api().write("input " + input_string + " was pressed.")
