import omnific
import sys

class omnific_script:

    def __init__(self):
        self.input_string = ""
        self.playlist = []
        pass

    def on_entity_start(self):
        omnific.get_window().change_title("Omnific Audio Player")
        scene = omnific.get_scene()
        self.audio_source = omnific.get_component("AudioSource")
        self.audio_source.is_capturing_waveform = True
        self.audio_source.waveform_capture_sample_count = int(omnific.get_window().get_window_size().x * 4.0)
        self.playback_time_label = scene.get_component_from_entity_by_name("Label", "Playback Time")
        self.playback_length_label = scene.get_component_from_entity_by_name("Label", "Playback Length")
        self.oscilloscope_plot_canvas = scene.get_component_from_entity_by_name("PlotCanvas", "Oscilloscope")
        self.start_message_label = scene.get_component_from_entity_by_name("Label", "Start Message")
        self.play_pause_button = scene.get_component_from_entity_by_name("Button", "Play/Pause Button")
        self.previous_button = scene.get_component_from_entity_by_name("Button", "Previous Button")
        self.next_button = scene.get_component_from_entity_by_name("Button", "Next Button")
        args = omnific.get_command_line_arguments()
        if len(args) > 1:
            for arg in args[1:]:
                if omnific.get_file_access().exists(arg):
                    self.playlist.append(arg)
                    self.audio_source.add_audio_clip(omnific.AudioClip(arg))
            if len(self.playlist) > 0:
                self.audio_source.play_audio_clip(self.playlist[0])

    def on_input(self):
        pass

    def on_update(self):
        active_audio_clip = self.audio_source.get_active_audio_clip()
        if active_audio_clip is not None:
            current_playback_time = self.audio_source.get_current_playback_time()
            playback_length = self.audio_source.get_playback_length()
            self.playback_time_label.set_text(str(int(current_playback_time // 60)).zfill(2) + ":" + str(int(current_playback_time % 60)).zfill(2))
            self.playback_length_label.set_text(str(int(playback_length // 60)).zfill(2) + ":" + str(int(playback_length % 60)).zfill(2))
            self.oscilloscope_plot_canvas.set_plot_points(self.audio_source.get_current_waveform(), omnific.Colour(0, 0, 0), omnific.Colour(255, 255, 255))
            self.start_message_label.hide()
        if self.play_pause_button.is_clicked:
            if self.audio_source.is_playing():
                self.audio_source.pause()
            else:
                self.audio_source.resume()
        pass

    def on_late_update(self):
        file_drop_events = omnific.query_events("file_dropped_on_window")
        if len(file_drop_events) > 0:
            audio_filepath = file_drop_events[0].get_parameters().strings["drop_file_path"]
            if omnific.get_file_access().exists(audio_filepath):
                self.start_message_label.set_text("Loading: " + audio_filepath)
                self.audio_source.add_audio_clip(omnific.AudioClip(audio_filepath))
                self.playlist.append(audio_filepath)
                if not self.audio_source.is_playing():
                    self.audio_source.play_audio_clip(audio_filepath)
        pass