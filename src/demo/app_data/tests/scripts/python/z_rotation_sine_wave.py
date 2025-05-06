import omnific
import math

class omnific_script:

    def __init__(self):
        self.time = 0
        pass

    def on_update(self):
        delta = omnific.get_time_delta()
        z_rotation_speed = 1.0
        amplitude = 45.0
        
        transform = omnific.get_transform()
        transform.rotation.z = amplitude * math.sin(self.time * z_rotation_speed)
        self.time += delta
        pass
