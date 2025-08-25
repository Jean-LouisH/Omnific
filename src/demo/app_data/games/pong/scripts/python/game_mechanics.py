import omnific
import random
import math
import constants

class omnific_script:

    def __init__(self):
        self.is_song_playing = False
        self.ball_entity = None
        self.ball_physics_body = None
        self.ball_transform = None
        self.left_post_transform = None
        self.right_post_transform = None
        pass

    def reset_ball(self):
        player_x_direction = random.choice([-1.0, 1.0])
        self.ball_physics_body.linear_velocity.x = random.uniform(12.0, 14.0) * player_x_direction
        self.ball_physics_body.linear_velocity.y = random.uniform(5.0, 8.0)
        self.ball_transform.translation.x = 0.0
        self.ball_transform.translation.y = 1.0
        self.ball_transform.translation.z = 0.0
        pass

    def on_entity_start(self):
        scene = omnific.get_scene()
        self.ball_entity = scene.get_entity_by_name("Ball")
        self.ball_physics_body = scene.get_component("PhysicsBody", self.ball_entity.get_id())
        self.ball_transform = self.ball_entity.get_transform()
        self.left_post_transform = scene.get_entity_by_name("LeftGoalPost").get_transform()
        self.right_post_transform = scene.get_entity_by_name("RightGoalPost").get_transform()
        omnific_script.reset_ball(self)
        #omnific.get_component("AudioSource").play_infinitely()
        pass

    def on_fixed_update(self):

        ball_bounce_increase = 1.5

        #Bouncing the ball off the walls
        if ((omnific.has_continuous_event("entity_is_colliding", "Ball_and_UpperBoundary") and self.ball_physics_body.linear_velocity.y > 0.0) or 
        (omnific.has_continuous_event("entity_is_colliding", "Ball_and_LowerBoundary") and self.ball_physics_body.linear_velocity.y < 0.0)):
            self.ball_physics_body.linear_velocity.y *= -1.0


        #When the ball goes past the posts
        if self.ball_transform.translation.x < self.left_post_transform.translation.x or self.ball_transform.translation.x > self.right_post_transform.translation.x:
            ball_speed = math.sqrt(self.ball_physics_body.linear_velocity.x * self.ball_physics_body.linear_velocity.x + 
              self.ball_physics_body.linear_velocity.y * self.ball_physics_body.linear_velocity.y)
            
            event_parameters = omnific.EventParameters()
            event_parameters.numbers = {"ball_speed": ball_speed}

            if self.ball_transform.translation.x < self.left_post_transform.translation.x:
                omnific.publish_event(omnific.Event("AI won", event_parameters), False)
            if self.ball_transform.translation.x > self.right_post_transform.translation.x:
                omnific.publish_event(omnific.Event("Player won", event_parameters), False)

            omnific_script.reset_ball(self)
        
        #When the ball collides with the paddles
        if ((omnific.has_continuous_event("entity_is_colliding", "Ball_and_Paddle1") and self.ball_physics_body.linear_velocity.x < 0.0) or 
        (omnific.has_continuous_event("entity_is_colliding", "Ball_and_Paddle2") and self.ball_physics_body.linear_velocity.x > 0.0)):
            self.ball_physics_body.linear_velocity.x *= -1.0 * ball_bounce_increase
            self.ball_physics_body.linear_velocity.y = (self.ball_physics_body.linear_velocity.y / abs(self.ball_physics_body.linear_velocity.y)) * random.uniform(5.0, 16.0)
            if abs(self.ball_physics_body.linear_velocity.x) > constants.ball_maximum_speed:
                if self.ball_physics_body.linear_velocity.x < 0.0:
                    self.ball_physics_body.linear_velocity.x = -constants.ball_maximum_speed
                elif self.ball_physics_body.linear_velocity.x > 0.0:
                    self.ball_physics_body.linear_velocity.x = constants.ball_maximum_speed
        
        pass
    
