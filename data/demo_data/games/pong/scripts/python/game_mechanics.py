import omnia
import random

class omnia_script:

    def __init__(self):
        pass

    def reset_ball(self):
        scene_tree = omnia.get_scene_tree()
        ball_entity = scene_tree.get_entity_by_name("Ball")
        ball_physics_body = scene_tree.get_component("PhysicsBody", ball_entity.get_id())
        ball_transform = scene_tree.get_component("Transform", ball_entity.get_id())
        player_x_direction = random.choice([-1.0, 1.0])
        ball_physics_body.linear_velocity.x = random.uniform(8.0, 10.0) * player_x_direction
        ball_physics_body.linear_velocity.y = random.uniform(5.0, 8.0)
        ball_transform.translation.x = 0.0
        ball_transform.translation.y = 1.0
        ball_transform.translation.z = 0.0

    def on_start(self):
        omnia_script.reset_ball(self)


    def on_logic(self):
        scene_tree = omnia.get_scene_tree()
        ball_entity = scene_tree.get_entity_by_name("Ball")
        ball_physics_body = scene_tree.get_component("PhysicsBody", ball_entity.get_id())
        ball_transform = scene_tree.get_component("Transform", ball_entity.get_id())
        collisions = scene_tree.get_collision_registry()
        ball_bounce_increase = 1.5

        if ((collisions.is_colliding("Ball", "UpperBoundary") and ball_physics_body.linear_velocity.y > 0.0) or 
        (collisions.is_colliding("Ball", "LowerBoundary") and ball_physics_body.linear_velocity.y < 0.0)):
            ball_physics_body.linear_velocity.y *= -1.0

        if collisions.is_colliding("Ball", "LeftGoalPost") or collisions.is_colliding("Ball", "RightGoalPost"):
            omnia_script.reset_ball(self)
            if collisions.is_colliding("Ball", "LeftGoalPost"):
                scene_tree.get_event_bus().publish("AI won")
            if collisions.is_colliding("Ball", "RightGoalPost"):
                scene_tree.get_event_bus().publish("Player won")
        
        if ((collisions.is_colliding("Ball", "Paddle1") and ball_physics_body.linear_velocity.x < 0.0) or 
        (collisions.is_colliding("Ball", "Paddle2") and ball_physics_body.linear_velocity.x > 0.0)):
            ball_physics_body.linear_velocity.x *= -1.0 * ball_bounce_increase
        
        pass
    
