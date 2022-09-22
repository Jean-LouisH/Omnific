import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_start(self):
        scene_tree = omnia.get_scene_tree()
        ball_entity = scene_tree.get_entity_by_name("Ball")
        ball_physics_body = scene_tree.get_component("PhysicsBody", ball_entity.get_id())
        ball_start_velocity = [-2.0, 1.0]

        ball_physics_body.linear_velocity.x = ball_start_velocity[0]
        ball_physics_body.linear_velocity.y = ball_start_velocity[1]


    def on_logic(self):
        pass
    
