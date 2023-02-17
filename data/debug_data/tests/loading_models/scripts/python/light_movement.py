import omnia
import math

class omnia_script:

    def __init__(self):
        pass

    def on_logic(self):
        transform = omnia.get_component("Transform")
        if (omnia.get_input().is_pressed("i")):
            transform.translate_z(-0.1)
        if (omnia.get_input().is_pressed("k")):
            transform.translate_z(0.1)
        if (omnia.get_input().is_pressed("j")):
            transform.translate_x(-0.1)
        if (omnia.get_input().is_pressed("l")):
            transform.translate_x(0.1)
        pass
