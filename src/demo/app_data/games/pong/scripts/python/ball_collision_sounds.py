import omnific
import constants

class omnific_script:

    def __init__(self):
        pass

    def on_update(self):
        if len(omnific.query_events_with_string_parameter("entity_is_colliding", "collider_name", "Ball")) > 0:
            omnific.get_component("AudioSource").play()
        pass
    
