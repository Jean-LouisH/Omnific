import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_fixed_update(self):
        degree_per_second = 30.0
        transform = omnific.get_transform()
        transform.rotate_z(degree_per_second * omnific.get_time_delta())
        pass
