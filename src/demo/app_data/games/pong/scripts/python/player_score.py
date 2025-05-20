import omnific

class omnific_script:

    def __init__(self):
        self.score = 0
        self.gui = None
        pass

    def on_entity_start(self):
        self.gui = omnific.get_component("GUI")
        pass

    def on_late_update(self):
        if omnific.has_event("Player won"):
            self.score += 1

        self.gui.set_to_label("Player: " + str(self.score))
        pass
    
