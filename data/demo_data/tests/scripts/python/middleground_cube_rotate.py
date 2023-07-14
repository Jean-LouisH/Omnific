import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_logic(self):
        transform = omnia.get_component("Transform")
        transform.rotate_x(1.0)
        transform.rotate_y(2.0)
