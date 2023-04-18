import omnia
import constants

class omnia_script:

    def __init__(self):
        pass

    def on_logic(self):
        physics_body = omnia.get_component("PhysicsBody")
        inp = omnia.get_input()

        control_stick_strength = inp.get_axis("left_axis_x")

        if inp.is_pressed(["d", "dpad_right"]):
            physics_body.accelerate_x(constants.acceleration, constants.maximum_speed)
        elif inp.is_pressed(["a", "dpad_left"]):
            physics_body.accelerate_x(-constants.acceleration, constants.maximum_speed)
        elif abs(control_stick_strength) > 0.01:
            physics_body.accelerate_x(constants.acceleration * control_stick_strength, constants.maximum_speed)
        else:
            physics_body.decelerate_x(constants.deceleration)
        pass
    
