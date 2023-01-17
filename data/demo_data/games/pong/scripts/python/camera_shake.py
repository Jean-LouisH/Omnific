import omnia
import random

class omnia_script:

    def __init__(self):
        pass

    def on_logic(self):
        transform = omnia.get_component("Transform")
        scene_tree = omnia.get_scene_tree()
        event_bus = scene_tree.get_event_bus()
        collisions = scene_tree.get_collision_registry()
        win_events = event_bus.query("Player won") + (event_bus.query("AI won"))

        if len(win_events) > 0:
            ball_speed = win_events[0].get_parameters().numbers["ball_speed"]
            omnia.get_logger().write("ball_speed " + str(ball_speed))
            transform.translation.x = ball_speed * random.choice([-1.0, 1.0]) * 0.5
            transform.translation.y = ball_speed * random.choice([-1.0, 1.0]) * 0.5
        
        transform.translation.x *= 0.25
        transform.translation.y *= 0.25
        pass
    
