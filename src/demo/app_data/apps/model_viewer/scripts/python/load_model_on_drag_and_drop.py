import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_late_update(self):
        file_drop_events = omnific.query_events("file_dropped_on_window")
        if len(file_drop_events) > 0:
            gltf_filepath = file_drop_events[0].get_parameters().strings["drop_file_path"]
            model_holder_entity = omnific.get_entity()
            omnific.get_scene().remove_entity(model_holder_entity.child_ids[0])
            omnific.get_scene().merge_another_scene_to_parent_entity(omnific.Scene(gltf_filepath), model_holder_entity.get_id())
        pass

