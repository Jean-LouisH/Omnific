import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_fixed_update(self):
        omnific.get_transform().rotate_y(1.0 * omnific.get_time_delta())
