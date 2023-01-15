import omnia
import constants

class omnia_script:

    def __init__(self):
        pass

    def on_input(self):
        inp = omnia.get_input()
        if inp.is_on_release("f1"):
            omnia.reload_active_scene()

    def on_logic(self):
        physics_body = omnia.get_component("PhysicsBody")
        inp = omnia.get_input()

        control_stick_strength = inp.get_axis("left_axis_y")

        if inp.is_pressed(["w", "dpad_up"]):
            physics_body.accelerate_y(constants.acceleration, constants.maximum_speed)
        elif inp.is_pressed(["s", "dpad_down"]):
            physics_body.accelerate_y(-constants.acceleration, constants.maximum_speed)
        elif abs(control_stick_strength) > 0.01:
            physics_body.accelerate_y(constants.acceleration * control_stick_strength, constants.maximum_speed)
        else:
            physics_body.decelerate_y(constants.deceleration)
        pass
    
