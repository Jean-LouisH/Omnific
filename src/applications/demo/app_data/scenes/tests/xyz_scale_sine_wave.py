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
        xyz_scale_speed = 1.0
        amplitude = 1.0
        
        self.transform.set_xyz_scale(amplitude * math.sin(self.time * xyz_scale_speed))
        self.time += delta
        pass
