import omnia

def on_start():
    scene_api = omnia.get_scene_api()
    image = scene_api.load_image("event_bus/images/collision.png")
    model_container = scene_api.get_component("ModelContainer")
    model_container.add_textured_cube(image)
    model_container.set_model_index(0)

def on_output():
    scene_api = omnia.get_scene_api()
    event_bus = scene_api.get_scene_tree().get_event_bus()
    model_container = scene_api.get_component("ModelContainer")
    collision_events = event_bus.query("entity_is_colliding")

    model_container.set_model_index(0)

    for collision in collision_events:
        collision_strings = collision.get_parameters().strings
        if (collision_strings["first_entity_name"] == "Floor" and
            collision_strings["second_entity_name"] == "CollidingCube"):
            model_container.set_model_index(1)
