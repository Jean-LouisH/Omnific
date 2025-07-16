import omnific
import math

class omnific_script:

    def __init__(self):
        pass

    def on_fixed_update(self):
        transform = omnific.get_transform()
        movement_speed = 1.0 * omnific.get_time_delta()

        if (omnific.get_inputs().is_pressed("i")):
            transform.translate_z(-movement_speed)
        if (omnific.get_inputs().is_pressed("k")):
            transform.translate_z(movement_speed)
        if (omnific.get_inputs().is_pressed("j")):
            transform.translate_x(-movement_speed)
        if (omnific.get_inputs().is_pressed("l")):
            transform.translate_x(movement_speed)
        pass
