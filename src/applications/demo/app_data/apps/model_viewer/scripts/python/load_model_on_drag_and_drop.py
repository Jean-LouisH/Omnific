import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_entity_start(self):
        self.scene = omnific.get_scene()
        self.model_holder_entity = omnific.get_entity()
        self.camera_entity = self.scene.get_entity_by_name("Camera")
        self.camera = self.scene.get_component_from_entity_by_name("Camera", "Camera")

    def on_input(self):
        if (omnific.get_inputs().is_on_press("p")):
            self.camera.toggle_wireframe_mode()

    def on_late_update(self):
        file_drop_events = omnific.query_events("file_dropped_on_window")
        if len(file_drop_events) > 0:
            gltf_filepath = file_drop_events[0].get_parameters().strings["drop_file_path"]
            self.scene.remove_entity(self.model_holder_entity.child_ids[0])
            new_model = omnific.Scene(gltf_filepath)
            new_model_dimensions = new_model.get_dimensions()
            largest_dimension = max(new_model_dimensions.x, new_model_dimensions.y, new_model_dimensions.z)
            self.camera_entity.get_transform().translation.z = largest_dimension * 2
            self.camera.far_plane = largest_dimension * 10
            self.scene.merge_another_scene_to_parent_entity(new_model, self.model_holder_entity.get_id())
        pass

