import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_entity_start(self):
        entity = omnific.get_entity()
        label = omnific.get_component("Label")
        renderable = omnific.get_scene().get_component("Renderable", entity.parent_id)
        label.set_text(renderable.get_surface_mode_string())
        print("Surface label: " + renderable.get_surface_mode_string())
        pass
