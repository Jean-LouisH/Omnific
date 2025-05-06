import omnific
import math

class omnific_script:

    def __init__(self):
        self.time = 0
        pass

    def on_update(self):
        delta = omnific.get_time_delta()
        xyz_scale_speed = 1.0
        amplitude = 1.0
        
        transform = omnific.get_transform()
        transform.set_xyz_scale(amplitude * math.sin(self.time * xyz_scale_speed))
        self.time += delta
        pass
