import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_logic(self):
        omnific.get_component("Transform").rotate_y(1.0)
