import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_late_update(self):
        file_drop_events = omnific.query_events("file_dropped_on_window")
        if len(file_drop_events) > 0:
            gltf_filepath = file_drop_events[0].get_parameters().strings["drop_file_path"]
            model_holder_entity = omnific.get_scene_layer().get_entity_by_name("ModelHolder")
            omnific.get_scene_layer().remove_entity(model_holder_entity.child_ids[0])
            omnific.get_scene_layer().merge_another_scene_layer_to_parent_entity(omnific.SceneLayer(gltf_filepath), model_holder_entity.get_id())
        pass

