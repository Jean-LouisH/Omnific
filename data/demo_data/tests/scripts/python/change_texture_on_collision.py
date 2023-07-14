import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_start(self):
        image = omnia.load_image("event_bus/images/collision.png")
        model_container = omnia.get_component("ModelContainer")
        model_container.add_textured_cube(image)
        model_container.change_to_model(0)

    def on_output(self):
        event_bus = omnia.get_scene_tree().get_event_bus()
        model_container = omnia.get_component("ModelContainer")

        if omnia.get_scene_tree().get_collision_registry().is_colliding("Floor", "CollidingCube"):
            model_container.change_to_model(1)
        else:
            model_container.change_to_model(0)
