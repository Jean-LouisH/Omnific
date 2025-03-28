import omnific
import math

class omnific_script:

    def __init__(self):
        pass

    def on_update(self):
        transform = omnific.get_transform()
        if (omnific.get_inputs().is_pressed("i")):
            transform.translate_z(-0.1)
        if (omnific.get_inputs().is_pressed("k")):
            transform.translate_z(0.1)
        if (omnific.get_inputs().is_pressed("j")):
            transform.translate_x(-0.1)
        if (omnific.get_inputs().is_pressed("l")):
            transform.translate_x(0.1)
        pass
