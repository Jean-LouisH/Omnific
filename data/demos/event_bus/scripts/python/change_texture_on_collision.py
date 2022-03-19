import omnia

def on_start():
    scene_api = omnia.get_scene_api()
    image = scene_api.load_asset("Image", "event_bus/images/collision.png")
    model_container = scene_api.get_component("ModelContainer")
    model_container.add_textured_cube(image)

def on_output():
    scene_api = omnia.get_scene_api()
    event_bus = scene_api.get_scene_tree().get_event_bus()
    model_container = scene_api.get_component("ModelContainer")
    collision_events = event_bus.query("collision_detected")

    omnia.get_log_api().write(str(collision_events))

    model_container.set_model_index(0)

    #for collision in collision_events:
    #    omnia.get_log_api().write(str(collision))
    #    collision_strings = collision.get_parameters.strings
    #    if (collision_strings["first_entity_name"] == "CollidingCube" and
    #        collision_strings["second_entity_name"] == "Floor"):
    #        model_container.set_model_index(1)
