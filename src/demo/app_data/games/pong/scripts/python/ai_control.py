import omnific
import math
import constants

class omnific_script:

    def __init__(self):
        self.enable_ai_mode = True
        self.transform = None
        self.ball_entity = None
        self.ball_physics_body = None
        self.ball_transform = None
        self.paddle_physics_body = None
        pass

    def on_entity_start(self):
        scene = omnific.get_scene()
        self.transform = omnific.get_transform()
        self.ball_entity = scene.get_entity_by_name("Ball")
        self.ball_physics_body = scene.get_component("PhysicsBody", self.ball_entity.get_id())
        self.ball_transform = self.ball_entity.get_transform()
        self.paddle_physics_body = omnific.get_component("PhysicsBody")
        pass

    def on_fixed_update(self):
        inputs = omnific.get_inputs()

        if inputs.is_on_release("p"):
            omnific.get_logger().write("P2 enabled")
            self.enable_ai_mode = not self.enable_ai_mode

        if self.enable_ai_mode:
            #AI logic to determine where to move as the ball approaches
        
            #Based on an older project: Suprannua, this is a placeholder
            #until better AI code is determined.

            position_angle_with_ball = math.atan2(self.ball_transform.translation.y - self.transform.translation.y,
                                                  self.ball_transform.translation.x - self.transform.translation.x)

            if (self.ball_physics_body.linear_velocity.x > 0):
                self.paddle_physics_body.accelerate_y(constants.acceleration * math.sin(position_angle_with_ball), constants.maximum_speed)
            else:
                self.paddle_physics_body.decelerate(constants.deceleration)

        else:
            if inputs.is_pressed("up"):
                self.paddle_physics_body.accelerate_y(constants.acceleration, constants.maximum_speed)
            elif inputs.is_pressed("down"):
                self.paddle_physics_body.accelerate_y(-constants.acceleration, -constants.maximum_speed)
            else:
                self.paddle_physics_body.decelerate(constants.deceleration)
            pass
            
        pass