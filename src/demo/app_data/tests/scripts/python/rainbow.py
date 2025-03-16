import omnific

class omnific_script:

    def __init__(self):
        self.red = 0.0
        self.green = 0.33
        self.blue = 0.67
        pass

    def on_update(self):
        delta = omnific.get_time_delta()
        colour_change = 0.1
        
        model = omnific.get_component("Model")
        
        model.shader_parameters.set_float_uniform("red", self.red)
        model.shader_parameters.set_float_uniform("green", self.green)
        model.shader_parameters.set_float_uniform("blue", self.blue)

        self.red += colour_change * delta
        if (self.red > 1.0):
            self.red = 0.0
        self.green += colour_change *delta
        if (self.green > 1.0):
            self.green = 0.0
        self.blue += colour_change * delta
        if (self.blue > 1.0):
            self.blue = 0.0
        pass
