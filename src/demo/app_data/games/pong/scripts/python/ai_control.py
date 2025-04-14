import omnific
import math
import constants

class omnific_script:

    def __init__(self):
        self.enable_ai_mode = True
        pass

    def on_fixed_update(self):
        inputs = omnific.get_inputs()

        if inputs.is_on_release("p"):
            omnific.get_logger().write("P2 enabled")
            self.enable_ai_mode = not self.enable_ai_mode

        if self.enable_ai_mode:
            pass
            transform = omnific.get_transform()
            scene_layer = omnific.get_scene_layer()
            ball_entity = scene_layer.get_entity_by_name("Ball")
            ball_physics_body = scene_layer.get_component("PhysicsBody", ball_entity.get_id())
            ball_transform = ball_entity.get_transform()
            paddle_physics_body = omnific.get_component("PhysicsBody")

            #AI logic to determine where to move as the ball approaches
        
            #Based on an older project: Suprannua, this is a placeholder
            #until better AI code is determined.

            position_angle_with_ball = math.atan2(ball_transform.translation.y - transform.translation.y,
                                                  ball_transform.translation.x - transform.translation.x)

            if (ball_physics_body.linear_velocity.x > 0):
                paddle_physics_body.accelerate_y(constants.acceleration * math.sin(position_angle_with_ball), constants.maximum_speed)
            else:
                paddle_physics_body.decelerate(constants.deceleration)

        else:
            physics_body = omnific.get_component("PhysicsBody")

            if inputs.is_pressed("up"):
                physics_body.accelerate_y(constants.acceleration, constants.maximum_speed)
            elif inputs.is_pressed("down"):
                physics_body.accelerate_y(-constants.acceleration, -constants.maximum_speed)
            else:
                physics_body.decelerate(constants.deceleration)
            pass
            
        pass