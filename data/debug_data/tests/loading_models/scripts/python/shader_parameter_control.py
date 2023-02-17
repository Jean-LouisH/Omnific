import omnia
import math

class omnia_script:

    def __init__(self):
        self.x = 0
        pass

    def on_logic(self):
        renderable = omnia.get_component("RenderableComponent")

        #self.x += 0.1
        #ambientStrength = 0.5 + (math.sin(self.x) / 2.0)
        ambientStrength = 0.5
        renderable.shader_parameters.set_float_uniform("ambientStrength", ambientStrength)
        pass
