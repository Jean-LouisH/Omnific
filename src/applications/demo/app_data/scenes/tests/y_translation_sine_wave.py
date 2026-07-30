import omnific
import math

class omnific_script:

    def __init__(self):
        self.time = 0
        self.transform = None
        pass

    def on_entity_start(self):
        self.transform = omnific.get_transform()
        pass

    def on_fixed_update(self):
        delta = omnific.get_time_delta()
        y_translation_speed = 1.0
        amplitude = 1.0
        
        self.transform.translation.y = amplitude * math.sin(self.time * y_translation_speed)
        self.time += delta
        pass
