import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_update(self):
        transform = omnific.get_transform()
        transform.rotate_x(1.0)
        transform.rotate_y(2.0)
