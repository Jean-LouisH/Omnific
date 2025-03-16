import omnific

class omnific_script:

    def __init__(self):
        self.score = 0
        pass

    def on_late_update(self):
        if omnific.has_event("Player won"):
            self.score += 1

        omnific.get_component("GUI").set_to_label("Player: " + str(self.score))
        pass
    
