import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_logic(self):
        omnia.get_component("Transform").rotate_y(1.0)
