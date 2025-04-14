import omnific
import math

class omnific_script:

    def __init__(self):
        self.x = 0
        pass

    def on_fixed_update(self):
        fixed_delta = omnific.get_fixed_time_delta()
        x_vibration_speed = 10.0
        amplitude = 10.0
        
        transform = omnific.get_transform()
        transform.translate_x(amplitude * math.sin(self.x * x_vibration_speed) * fixed_delta)
        self.x += fixed_delta
        pass
