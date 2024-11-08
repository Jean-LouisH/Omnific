import omnific
import math

class omnific_script:

    def __init__(self):
        self.x = 0
        pass

    def on_logic(self):
        renderable = omnific.get_component("RenderableComponent")

        #self.x += 0.1
        #ambient_strength = 0.5 + (math.sin(self.x) / 2.0)
        ambient_strength = 0.5
        renderable.shader_parameters.set_float_uniform("ambient_strength", ambient_strength)
        pass
