import omnific

class omnific_script:

    def __init__(self):
        self.score = 0
        
        pass

    def on_late_update(self):
        if omnific.get_scene_tree().get_event_bus().query_count("Player won") > 0:
            self.score += 1

        omnific.get_component("GUI").set_text("Player: " + str(self.score))
        pass
    
