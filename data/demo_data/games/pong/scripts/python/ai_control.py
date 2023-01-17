import omnia
import math
import constants

class omnia_script:

    def __init__(self):
        self.enable_ai_mode = True
        pass

    def on_logic(self):
        inp = omnia.get_input()

        if inp.is_on_release("p"):
            omnia.get_logger().write("P2 enabled")
            self.enable_ai_mode = not self.enable_ai_mode

        if self.enable_ai_mode:
            pass
            transform = omnia.get_component("Transform")
            scene_tree = omnia.get_scene_tree()
            ball_entity = scene_tree.get_entity_by_name("Ball")
            ball_physics_body = scene_tree.get_component("PhysicsBody", ball_entity.get_id())
            ball_transform = scene_tree.get_component("Transform", ball_entity.get_id())
            paddle_physics_body = omnia.get_component("PhysicsBody")

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
            physics_body = omnia.get_component("PhysicsBody")

            if inp.is_pressed("up"):
                physics_body.accelerate_y(constants.acceleration, constants.maximum_speed)
            elif inp.is_pressed("down"):
                physics_body.accelerate_y(-constants.acceleration, -constants.maximum_speed)
            else:
                physics_body.decelerate(constants.deceleration)
            pass
            
        pass