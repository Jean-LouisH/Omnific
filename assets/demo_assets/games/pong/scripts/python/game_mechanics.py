import omnific
import random
import math
import constants

class omnific_script:

    def __init__(self):
        pass

    def reset_ball(self):
        scene_layer = omnific.get_scene_layer()
        ball_entity = scene_layer.get_entity_by_name("Ball")
        ball_physics_body = scene_layer.get_component("PhysicsBody", ball_entity.get_id())
        ball_transform = scene_layer.get_component("Transform", ball_entity.get_id())
        player_x_direction = random.choice([-1.0, 1.0])
        ball_physics_body.linear_velocity.x = random.uniform(12.0, 14.0) * player_x_direction
        ball_physics_body.linear_velocity.y = random.uniform(5.0, 8.0)
        ball_transform.translation.x = 0.0
        ball_transform.translation.y = 1.0
        ball_transform.translation.z = 0.0

    def on_start(self):
        omnific_script.reset_ball(self)


    def on_logic(self):
        scene_layer = omnific.get_scene_layer()
        ball_entity = scene_layer.get_entity_by_name("Ball")
        ball_physics_body = scene_layer.get_component("PhysicsBody", ball_entity.get_id())
        ball_transform = scene_layer.get_component("Transform", ball_entity.get_id())
        left_post_transform = scene_layer.get_component("Transform", scene_layer.get_entity_by_name("LeftGoalPost").get_id())
        right_post_transform = scene_layer.get_component("Transform", scene_layer.get_entity_by_name("RightGoalPost").get_id())
        collisions = scene_layer.get_collision_registry()
        ball_bounce_increase = 1.5

        #Bouncing the ball off the walls
        if ((collisions.is_colliding("Ball", "UpperBoundary") and ball_physics_body.linear_velocity.y > 0.0) or 
        (collisions.is_colliding("Ball", "LowerBoundary") and ball_physics_body.linear_velocity.y < 0.0)):
            ball_physics_body.linear_velocity.y *= -1.0

        #When the ball goes past the posts
        if ball_transform.translation.x < left_post_transform.translation.x or ball_transform.translation.x > right_post_transform.translation.x:
            ball_speed = math.sqrt(ball_physics_body.linear_velocity.x * ball_physics_body.linear_velocity.x + 
              ball_physics_body.linear_velocity.y * ball_physics_body.linear_velocity.y)

            if ball_transform.translation.x < left_post_transform.translation.x:
                omnific.publish_event("AI won", {"ball_speed": ball_speed})
            if ball_transform.translation.x > right_post_transform.translation.x:
                omnific.publish_event("Player won", {"ball_speed": ball_speed})

            omnific_script.reset_ball(self)
        
        #When the ball collides with the paddles
        if ((collisions.is_colliding("Ball", "Paddle1") and ball_physics_body.linear_velocity.x < 0.0) or 
        (collisions.is_colliding("Ball", "Paddle2") and ball_physics_body.linear_velocity.x > 0.0)):
            ball_physics_body.linear_velocity.x *= -1.0 * ball_bounce_increase
            ball_physics_body.linear_velocity.y = (ball_physics_body.linear_velocity.y / abs(ball_physics_body.linear_velocity.y)) * random.uniform(5.0, 16.0)
            if abs(ball_physics_body.linear_velocity.x) > constants.ball_maximum_speed:
                if ball_physics_body.linear_velocity.x < 0.0:
                    ball_physics_body.linear_velocity.x = -constants.ball_maximum_speed
                elif ball_physics_body.linear_velocity.x > 0.0:
                    ball_physics_body.linear_velocity.x = constants.ball_maximum_speed
        
        pass
    
