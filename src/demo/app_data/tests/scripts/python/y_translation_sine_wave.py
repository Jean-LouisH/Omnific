import omnific
import math

class omnific_script:

    def __init__(self):
        self.time = 0
        pass

    def on_fixed_update(self):
        delta = omnific.get_time_delta()
        y_translation_speed = 1.0
        amplitude = 1.0
        
        transform = omnific.get_transform()
        transform.translation.y = amplitude * math.sin(self.time * y_translation_speed)
        self.time += delta
        pass
