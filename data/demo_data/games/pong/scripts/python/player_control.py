import omnia

class omnia_script:

    def __init__(self):
        pass

    def accelerate(acceleration, maximum_speed):
        physics_body = omnia.get_component("PhysicsBody")
        physics_body.linear_velocity.y += acceleration
        if abs(physics_body.linear_velocity.y) > abs(maximum_speed):
            physics_body.linear_velocity.y = maximum_speed * (abs(physics_body.linear_velocity.y) / physics_body.linear_velocity.y)

    def on_input(self):
        inp = omnia.get_input()
        if inp.is_on_release("f1"):
            omnia.reload_active_scene()

    def on_logic(self):
        physics_body = omnia.get_component("PhysicsBody")
        inp = omnia.get_input()

        acceleration = 5
        maximum_speed = 30
        deceleration = 1.5

        control_stick_strength = inp.get_axis("left_axis_y")

        if inp.is_pressed(["w", "dpad_up"]):
            omnia_script.accelerate(acceleration, maximum_speed)
        elif inp.is_pressed(["s", "dpad_down"]):
            omnia_script.accelerate(-acceleration, maximum_speed)
        elif abs(control_stick_strength) > 0.01:
            omnia_script.accelerate(acceleration * control_stick_strength, maximum_speed)
        else:
            physics_body.linear_velocity.y /= deceleration
        pass
    
