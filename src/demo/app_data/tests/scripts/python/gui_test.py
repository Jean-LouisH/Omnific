import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_update(self):
        omnific.get_component("GUI").set_to_label("Blah.")
        pass
