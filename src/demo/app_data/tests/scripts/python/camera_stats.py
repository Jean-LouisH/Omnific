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
        euler_rotation = self.transform.get_rotation_in_degrees_euler_angles()
        print("t.x: " + str(self.transform.translation.x) + 
              " t.y: " + str(self.transform.translation.y) + 
              " t.z: " + str(self.transform.translation.z) + 
              " r.x: " + str(euler_rotation.x) + 
              " r.y: " + str(euler_rotation.y) + 
              " r.z: " + str(euler_rotation.z)
              )
        pass
