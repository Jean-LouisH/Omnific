import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_start(self):
        pass

    def on_logic(self):
        if omnific.get_input().is_on_release("f5"):
            omnific.reload_active_scene()
        pass
