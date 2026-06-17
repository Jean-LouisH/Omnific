import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_entity_start(self):
        self.scene = omnific.get_scene()
        self.model_holder_entity = omnific.get_entity()
        self.camera_entity = self.scene.get_entity_by_name("Camera")
        self.camera = self.scene.get_component_from_entity_by_name("Camera", "Camera")
        self.camera_pivot_transform = omnific.get_scene().get_entity_by_name("CameraPivot").get_transform()

    def on_input(self):
        if (omnific.get_inputs().is_on_press("p")):
            self.camera.toggle_wireframe_mode()

    def on_late_update(self):
        file_drop_events = omnific.query_events("file_dropped_on_window")
        if len(file_drop_events) > 0:
            gltf_filepath = file_drop_events[0].get_parameters().strings["drop_file_path"]
            self.scene.remove_entity(self.model_holder_entity.child_ids[0])
            new_model = omnific.Scene(gltf_filepath)

            renderable = new_model.get_renderables_in_order()[0]
            renderable_entity = new_model.get_entity(renderable.get_entity_id())
            renderable_global_translation = new_model.calculate_global_transform(renderable_entity.get_id()).translation
            renderable_dimensions = renderable.get_dimensions()
            print("renderable dimensions: x = " + str(renderable_dimensions.x) + " y = " + str(renderable_dimensions.y) + " z = " + str(renderable_dimensions.z))
            largest_dimension = max(renderable_dimensions.x, renderable_dimensions.y, renderable_dimensions.z)
            print("largest dimension size = " + str(largest_dimension))

            camera_transform = self.camera_entity.get_transform()
            self.camera_pivot_transform.translation.x = renderable_global_translation.x
            self.camera_pivot_transform.translation.y = renderable_global_translation.y #+ renderable_dimensions.y / 2.0
            self.camera_pivot_transform.translation.z = renderable_global_translation.z
            print("Camera pivot located at x = " + str(self.camera_pivot_transform.translation.x) + " y = " + str(self.camera_pivot_transform.translation.y) + " z = "  + str(self.camera_pivot_transform.translation.z))
            self.camera_pivot_transform.set_xyz_scale(largest_dimension)
            print("Camera pivot scale set at: " + str(largest_dimension))
            self.camera.far_plane = largest_dimension * 10
            self.scene.merge_another_scene_to_parent_entity(new_model, self.model_holder_entity.get_id())
        pass

