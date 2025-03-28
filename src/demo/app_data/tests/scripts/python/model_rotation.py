import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_update(self):
        movement_speed = 1.0
        input = omnific.get_inputs()
        transform = omnific.get_transform()

        if input.is_pressed(["up"]):
            transform.rotate_x(movement_speed)
        if input.is_pressed(["down"]):
            transform.rotate_x(-movement_speed)
        if input.is_pressed(["left"]):
            transform.rotate_y(-movement_speed)
        if input.is_pressed(["right"]):
            transform.rotate_y(movement_speed)
        pass
