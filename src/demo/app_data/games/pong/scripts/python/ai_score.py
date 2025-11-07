import omnific
import constants

class omnific_script:

    def __init__(self):
        pass

    def on_entity_start(self):
        self.score = 0
        self.gui = omnific.get_component("GUI")
        self.ai_mode = True
        pass

    def on_update(self):
        if self.score > 10:
            event_parameters = omnific.EventParameters()
            event_parameters.strings = {"winner": "AI"}
            omnific.publish_event(omnific.Event(constants.game_set_event, event_parameters), True)
            self.score = 0

    def on_late_update(self):
        if omnific.has_event("AI won"):
            self.score += 1

        if omnific.get_inputs().is_on_release("p"):
            self.ai_mode = not self.ai_mode

        player_name = "AI"
        if not self.ai_mode:
            player_name = "Player 2"

        self.gui.set_to_label(player_name + ": " + str(self.score))
        pass
    
