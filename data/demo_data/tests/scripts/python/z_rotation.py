import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_logic(self):
        degree_per_second = 30.0
        input = omnia.get_input()
        transform = omnia.get_component("Transform")
        transform.rotate_z(degree_per_second * omnia.get_time_delta())
        pass
