import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_logic(self):
        renderable = omnia.get_component("RenderableComponent")
        renderable.shader_parameters.float_uniforms["ambientStrength"] = 1.0
        pass
