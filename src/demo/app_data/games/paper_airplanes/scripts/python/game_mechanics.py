import omnific
import random
import math
import constants

class omnific_script:

    def __init__(self):
        pass

    def reset_ball(self):
        scene_tree = omnific.get_scene_tree()
        ball_entity = scene_tree.get_entity_by_name("Ball")
        ball_physics_body = scene_tree.get_component("PhysicsBody", ball_entity.get_id())
        ball_transform = scene_tree.get_component("Transform", ball_entity.get_id())
        player_z_direction = random.choice([-1.0, 1.0])
        ball_physics_body.linear_velocity.x = random.uniform(8.0, 10.0) * player_x_direction
        ball_physics_body.linear_velocity.z = random.uniform(5.0, 8.0)
        ball_transform.translation.x = 0.0
        ball_transform.translation.y = 2.0
        ball_transform.translation.z = -2.5

    def on_entity_start(self):
        omnific_script.reset_ball(self)


    def on_update(self):
        scene_tree = omnific.get_scene_tree()
        ball_entity = scene_tree.get_entity_by_name("Ball")
        ball_physics_body = scene_tree.get_component("PhysicsBody", ball_entity.get_id())
        ball_transform = scene_tree.get_component("Transform", ball_entity.get_id())
        front_post_transform = scene_tree.get_component("Transform", scene_tree.get_entity_by_name("Front Post").get_id())
        back_post_transform = scene_tree.get_component("Transform", scene_tree.get_entity_by_name("Back Post").get_id())
        collisions = scene_tree.get_collision_registry()
        ball_bounce_increase = 1.5

        #Bouncing the ball off the table
        if collisions.is_colliding("Ball", "Table"):
            ball_physics_body.linear_velocity.z *= -1.0

        #When the ball goes past the posts
        #if ball_transform.translation.z < back_post_transform.translation.z or ball_transform.translation.z > front_post_transform.translation.z:
        #    omnific_script.reset_ball(self)
        #    ball_speed = math.sqrt(ball_physics_body.linear_velocity.x * ball_physics_body.linear_velocity.x + 
        #      ball_physics_body.linear_velocity.z * ball_physics_body.linear_velocity.z)

        #    if ball_transform.translation.x < left_post_transform.translation.x:
        #        scene_tree.get_event_bus().publish("AI won", {"ball_speed": ball_speed})
        #    if ball_transform.translation.x > right_post_transform.translation.x:
        #        scene_tree.get_event_bus().publish("Player won", {"ball_speed": ball_speed})
        
        #When the ball collides with the paddles
        #if ((collisions.is_colliding("Ball", "Paddle1") and ball_physics_body.linear_velocity.x < 0.0) or 
        #(collisions.is_colliding("Ball", "Paddle2") and ball_physics_body.linear_velocity.x > 0.0)):
        #    ball_physics_body.linear_velocity.x *= -1.0 * ball_bounce_increase
        #    if abs(ball_physics_body.linear_velocity.x) > constants.ball_maximum_speed:
        #        if ball_physics_body.linear_velocity.x < 0.0:
        #            ball_physics_body.linear_velocity.x = -constants.ball_maximum_speed
        #        elif ball_physics_body.linear_velocity.x > 0.0:
        #            ball_physics_body.linear_velocity.x = constants.ball_maximum_speed
        
        pass
    
