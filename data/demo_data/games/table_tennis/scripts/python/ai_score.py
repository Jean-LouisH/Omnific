import omnia

class omnia_script:

    def __init__(self):
        self.score = 0
        pass

    def on_late(self):
        if omnia.get_scene_tree().get_event_bus().query_count("AI won") > 0:
            self.score += 1

        omnia.get_component("GUI").set_text("AI: " + str(self.score))
        pass
    
