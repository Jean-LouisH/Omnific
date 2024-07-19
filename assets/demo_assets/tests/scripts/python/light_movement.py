import omnific
import math

class omnific_script:

    def __init__(self):
        pass

    def on_logic(self):
        transform = omnific.get_component("Transform")
        if (omnific.get_input().is_pressed("i")):
            transform.translate_z(-0.1)
        if (omnific.get_input().is_pressed("k")):
            transform.translate_z(0.1)
        if (omnific.get_input().is_pressed("j")):
            transform.translate_x(-0.1)
        if (omnific.get_input().is_pressed("l")):
            transform.translate_x(0.1)
        pass
