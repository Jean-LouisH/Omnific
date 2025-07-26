import omnific
import constants

class omnific_script:

    def __init__(self):
        pass

    def on_fixed_update(self):
        if len(omnific.query_events_with_string_parameter("entity_is_off_collision", "collider_name", "Ball")) > 0:
            omnific.get_component("AudioSource").play()
        pass
    
