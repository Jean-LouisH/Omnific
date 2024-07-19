import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_start(self):
        image = omnific.load_image("event_bus/images/collision.png")
        model_container = omnific.get_component("ModelContainer")
        model_container.add_textured_cube(image)
        model_container.change_to_model(0)

    def on_output(self):
        event_bus = omnific.get_scene_layer().get_event_bus()
        model_container = omnific.get_component("ModelContainer")

        if omnific.get_scene_layer().get_collision_registry().is_colliding("Floor", "CollidingCube"):
            model_container.change_to_model(1)
        else:
            model_container.change_to_model(0)
