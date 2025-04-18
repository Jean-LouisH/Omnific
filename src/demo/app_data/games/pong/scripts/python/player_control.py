import omnific
import constants

class omnific_script:

    def __init__(self):
        pass

    def on_fixed_update(self):
        physics_body = omnific.get_component("PhysicsBody")
        inputs = omnific.get_inputs()

        control_stick_strength = inputs.get_axis("left_axis_y")

        if inputs.is_pressed(["w", "dpad_up"]):
            physics_body.accelerate_y(constants.acceleration, constants.maximum_speed)
        elif inputs.is_pressed(["s", "dpad_down"]):
            physics_body.accelerate_y(-constants.acceleration, constants.maximum_speed)
        elif abs(control_stick_strength) > 0.01:
            physics_body.accelerate_y(constants.acceleration * control_stick_strength, constants.maximum_speed)
        else:
            physics_body.decelerate_y(constants.deceleration)
        pass
    
