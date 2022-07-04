import omnia


def on_logic_frame():
    transform = omnia.get_component("Transform")
    transform.rotate_x(1.0)
    transform.rotate_y(2.0)
