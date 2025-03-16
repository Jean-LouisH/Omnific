import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_entity_start(self):
        pass

    def on_update(self):
        if omnific.get_input().is_on_release("f5"):
            omnific.reload_active_scene()
        pass
