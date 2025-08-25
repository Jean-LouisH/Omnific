import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_entity_start(self):
        image = omnific.load_image("event_bus/images/collision.png")
        model = omnific.get_component("Model")
        model.add_textured_cube(image)
        model.change_to_model(0)

    def on_output(self):
        event_bus = omnific.get_event_bus()
        model = omnific.get_component("Model")

        if omnific.get_scene().get_collision_registry().is_colliding("Floor", "CollidingCube"):
            model.change_to_model(1)
        else:
            model.change_to_model(0)
