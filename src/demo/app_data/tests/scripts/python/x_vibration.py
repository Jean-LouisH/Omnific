import omnific
import math

class omnific_script:

    def __init__(self):
        self.x = 0
        pass

    def on_logic(self):
        delta = omnific.get_time_delta()
        x_vibration_speed = 10.0
        
        transform = omnific.get_transform()
        #transform.translate_x(math.sin(self.x * xVibrationSpeed * delta))
        transform.translate_x(math.sin(self.x * x_vibration_speed) * delta)
        self.x += delta
        pass
