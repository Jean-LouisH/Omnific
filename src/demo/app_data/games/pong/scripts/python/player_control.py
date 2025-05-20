import omnific
import constants

class omnific_script:

    def __init__(self):
        self.physics_body = None
        pass

    def on_entity_start(self):
        self.physics_body = omnific.get_component("PhysicsBody")
        pass

    def on_fixed_update(self):
        inputs = omnific.get_inputs()
        control_stick_strength = inputs.get_axis("left_axis_y")

        if inputs.is_pressed(["w", "dpad_up"]):
            self.physics_body.accelerate_y(constants.acceleration, constants.maximum_speed)
        elif inputs.is_pressed(["s", "dpad_down"]):
            self.physics_body.accelerate_y(-constants.acceleration, constants.maximum_speed)
        elif abs(control_stick_strength) > 0.01:
            self.physics_body.accelerate_y(constants.acceleration * control_stick_strength, constants.maximum_speed)
        else:
            self.physics_body.decelerate_y(constants.deceleration)
        pass
    
