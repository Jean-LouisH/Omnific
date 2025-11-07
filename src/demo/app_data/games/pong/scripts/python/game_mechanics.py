import omnific
import random
import math
import constants

class omnific_script:

    def __init__(self):
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
        self.message_board_entity = scene.get_entity_by_name("MessageBoard")
        self.message_board_gui = scene.get_component("GUI", self.message_board_entity.get_id())
        omnific_script.reset_ball(self)
        #omnific.get_component("AudioSource").play_infinitely()

        omnific.publish_event(omnific.Event(constants.press_start_wait_event), True)
        self.message_board_gui.set_to_label("Press 'Enter' / 'Start'")
        pass

    def on_update(self):
        inputs = omnific.get_inputs()

        if omnific.has_event(constants.press_start_wait_event):
            if inputs.is_pressed(["enter", "button_menu"]):
                omnific.remove_continuous_event(constants.press_start_wait_event, "")
                omnific.publish_event(omnific.Event(constants.game_is_playing_event), True)
                self.message_board_gui.hide()
                omnific_script.reset_ball(self)
        pass

    def on_fixed_update(self):

        if omnific.has_event(constants.game_is_playing_event):
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
                self.ball_physics_body.linear_velocity.x *= -ball_bounce_increase
                self.ball_physics_body.linear_velocity.y = random.choice([-1.0, 1.0]) * random.uniform(3.0, 30.0)
                if abs(self.ball_physics_body.linear_velocity.x) > constants.ball_maximum_speed:
                    if self.ball_physics_body.linear_velocity.x < 0.0:
                        self.ball_physics_body.linear_velocity.x = -constants.ball_maximum_speed
                    elif self.ball_physics_body.linear_velocity.x > 0.0:
                        self.ball_physics_body.linear_velocity.x = constants.ball_maximum_speed
            
        pass
    
    def on_late_update(self):
        for event in omnific.query_events(constants.game_set_event):
            winner = event.get_parameters().strings["winner"]
            self.message_board_gui.set_to_label(winner + " is the winner! Press 'Enter' / 'Start' to play again.")
            self.message_board_gui.show()
            omnific.publish_event(omnific.Event(constants.press_start_wait_event), True)
            omnific.remove_continuous_event(constants.game_is_playing_event, "")
            omnific.remove_continuous_event(constants.game_set_event, "")
