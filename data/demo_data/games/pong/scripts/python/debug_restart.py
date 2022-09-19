import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_input(self):
        inp = omnia.get_input()
        if inp.is_pressed("f1"):
            inp.request_restart()
