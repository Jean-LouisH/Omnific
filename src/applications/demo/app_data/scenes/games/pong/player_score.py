import omnific
import constants

class omnific_script:

    def __init__(self):
        self.score = 0
        pass

    def on_entity_start(self):
        self.label = omnific.get_component("Label")
        pass

    def on_update(self):
        if self.score > 10:
            event_parameters = omnific.EventParameters()
            event_parameters.strings = {"winner": "Player 1"}
            omnific.publish_event(omnific.Event(constants.game_set_event, event_parameters), True)
            self.score = 0

    def on_late_update(self):
        if omnific.has_event("Player won"):
            self.score += 1

        self.label.set_text("Player 1: " + str(self.score))
        pass
    
