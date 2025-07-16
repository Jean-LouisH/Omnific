import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_entity_start(self):
        self.transform = omnific.get_transform()

    def on_fixed_update(self):
        self.transform.rotate_y(30 * omnific.get_time_delta() * -1.0)
    
