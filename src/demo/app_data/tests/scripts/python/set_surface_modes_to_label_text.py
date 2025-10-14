import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_entity_start(self):
        entity = omnific.get_entity()
        gui = omnific.get_component("GUI")
        renderable = omnific.get_scene().get_component("Renderable", entity.parent_id)
        gui.set_to_label(renderable.get_surface_mode_string())
        print("Surface label: " + renderable.get_surface_mode_string())
        pass
