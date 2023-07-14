import omnia
import math

class omnia_script:

    def __init__(self):
        self.x = 0
        pass

    def on_logic(self):
        delta = omnia.get_time_delta()
        xVibrationSpeed = 10.0
        
        transform = omnia.get_component("Transform")
        #transform.translate_x(math.sin(self.x * xVibrationSpeed * delta))
        transform.translate_x(math.sin(self.x * xVibrationSpeed) * delta)
        self.x += delta
        pass
