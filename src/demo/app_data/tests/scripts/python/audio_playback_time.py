import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_entity_start(self):
        self.gui = omnific.get_component("GUI")
        self.audio_source = omnific.get_component("AudioSource")

    def on_fixed_update(self):
        self.gui.set_to_label(
            str(self.audio_source.get_current_playback_time()) + " / " + str(self.audio_source.get_active_audio().get_playback_length())
            )
        pass
