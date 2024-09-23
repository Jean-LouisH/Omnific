import omnific

class omnific_script:

    def __init__(self):
        self.red = 0.0
        self.green = 0.33
        self.blue = 0.67
        pass

    def on_logic(self):
        delta = omnific.get_time_delta()
        colourChange = 0.1
        
        renderable = omnific.get_component("Model")
        
        renderable.shader_parameters.set_float_uniform("red", self.red)
        renderable.shader_parameters.set_float_uniform("green", self.green)
        renderable.shader_parameters.set_float_uniform("blue", self.blue)

        self.red += colourChange * delta
        if (self.red > 1.0):
            self.red = 0.0
        self.green += colourChange *delta
        if (self.green > 1.0):
            self.green = 0.0
        self.blue += colourChange * delta
        if (self.blue > 1.0):
            self.blue = 0.0
        pass
