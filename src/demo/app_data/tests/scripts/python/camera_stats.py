import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_entity_start(self):
        self.camera = omnific.get_component("Camera")
        self.transform = omnific.get_transform()
        pass

    def on_update(self):
        print("Camera Statistics")
        print("t.x: " + str(self.transform.translation.x) + 
              " t.y: " + str(self.transform.translation.y) + 
              " t.z: " + str(self.transform.translation.z) + 
              " r.x: " + str(self.transform.rotation.x) + 
              " r.y: " + str(self.transform.rotation.y) + 
              " r.z: " + str(self.transform.rotation.z)
              )
        pass
