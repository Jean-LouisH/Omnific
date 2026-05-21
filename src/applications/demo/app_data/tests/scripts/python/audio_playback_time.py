import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_entity_start(self):
        self.label = omnific.get_component("Label")
        self.audio_source = omnific.get_component("AudioSource")

    def on_update(self):
        self.label.set_text(
            str('%.2f' % self.audio_source.get_current_playback_time()) + " / " + str('%.2f' % self.audio_source.get_active_audio_clip().get_playback_length())
            )
        pass
