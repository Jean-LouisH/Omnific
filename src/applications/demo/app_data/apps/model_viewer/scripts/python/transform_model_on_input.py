import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_entity_start(self):
        self.transform = omnific.get_entity().get_transform()
        self.camera_pivot_transform = omnific.get_scene().get_entity_by_name("CameraPivot").get_transform()
        self.camera_zoom_scale = 5.0
        self.camera_pivot_transform.set_xyz_scale(self.camera_zoom_scale)
        pass

    def on_fixed_update(self):

        rotation_speed = 90
        zoom_speed = 5
        inputs = omnific.get_inputs()
        time_delta = omnific.get_time_delta()

        if inputs.is_pressed(["up", "dpad_up"]):
            self.transform.rotate_x(-rotation_speed * time_delta)
        if inputs.is_pressed(["down", "dpad_down"]):
            self.transform.rotate_x(rotation_speed * time_delta)
        if inputs.is_pressed(["left", "dpad_left"]):
            self.transform.rotate_y(-rotation_speed * time_delta)
        if inputs.is_pressed(["right", "dpad_right"]):
            self.transform.rotate_y(rotation_speed * time_delta)
        if inputs.is_pressed(["q", "left_shoulder_button"]):
            self.transform.rotate_z(rotation_speed * time_delta)
        if inputs.is_pressed(["e", "right_shoulder_button"]):
            self.transform.rotate_z(-rotation_speed * time_delta)
        
        mouse_wheel_velocity = inputs.get_mouse_wheel_velocity().y

        if abs(mouse_wheel_velocity) > 0.5:
            near_zoom_limit = 0.1
            far_zoom_limit = 5.0
            self.camera_zoom_scale += mouse_wheel_velocity * zoom_speed * time_delta
            if self.camera_zoom_scale < near_zoom_limit:
                self.camera_zoom_scale = near_zoom_limit
            elif self.camera_zoom_scale > far_zoom_limit:
                self.camera_zoom_scale = far_zoom_limit
            self.camera_pivot_transform.set_xyz_scale(self.camera_zoom_scale)
            print("Camera pivot scale set at: " + str(self.camera_zoom_scale))

        pass
