from ast import arg
import os
#import random

import omnific
import sys

class omnific_script:

    def __init__(self):
        self.input_string = ""
        self.playlist = []
        self.cached_images = {}
        self.current_image_index = -1
        self.slideshow_delay = 5.0
        self.is_slideshow_playing = False
        self.is_slideshow_shuffled = False
        pass

    def on_entity_start(self):
        self.app_name = "Omnific Image Viewer"
        window = omnific.get_window()
        window.change_title(self.app_name)
        window.resize(640,640)
        scene = omnific.get_scene()
        self.slideshow_timer = scene.get_component_from_entity_by_name("Timer", "Slideshow Timer")
        self.image_canvas = scene.get_component_from_entity_by_name("ImageCanvas", "Image")
        self.image_canvas_entity = scene.get_entity_by_name("Image")
        self.start_message_label = scene.get_component_from_entity_by_name("Label", "Start Message")
        self.slideshow_button = scene.get_component_from_entity_by_name("Button", "Slideshow Button")
        self.slideshow_button_entity = scene.get_entity_by_name("Slideshow Button")
        args = omnific.get_command_line_arguments()
        if len(args) > 1:
            arg = args[1]
            if omnific.get_file_access().exists(arg):
                self.open_image(arg)

    def on_input(self):
        inputs = omnific.get_inputs()
        window = omnific.get_window()

        if inputs.is_left_mouse_button_double_clicked():
            window.toggle_windowed_fullscreen()
            self.scale_image_to_window()

        if inputs.is_on_release("right"):
            self.shift_image_index(1)
        elif inputs.is_on_release("left"):
            self.shift_image_index(-1)


        mouse_position = inputs.get_mouse_position()
        slideshow_button_position = self.slideshow_button_entity.get_transform().translation
        if len(self.playlist) <= 1 or abs(mouse_position.y - slideshow_button_position.y) < window.get_window_size().y - 100:
            self.slideshow_button.hide()
        else:
            self.slideshow_button.show()

    def on_update(self):
        if self.image_canvas.has_image():
            self.start_message_label.hide()

        if self.slideshow_button.is_clicked and omnific.get_inputs().is_left_mouse_button_on_press():
            if len(self.playlist) > 0:
                self.is_slideshow_playing = not self.is_slideshow_playing
            pass
        pass

    def on_late_update(self):
        file_drop_events = omnific.query_events("file_dropped_on_window")
        if len(file_drop_events) > 0:
            image_filepath = file_drop_events[0].get_parameters().strings["drop_file_path"]
            if omnific.get_file_access().exists(image_filepath):
                self.start_message_label.set_text("Loading: " + image_filepath)
                self.open_image(image_filepath)

        if self.is_slideshow_playing:
            if self.slideshow_timer.is_finished():
                self.slideshow_timer.start(self.slideshow_delay)
                shift_amount = 0
                if self.is_slideshow_shuffled:
                    #shift_amount = random.randint(1, len(self.playlist) - 1)
                    pass
                else:
                    shift_amount = 1
                self.shift_image_index(shift_amount)
        pass

    def add_other_files_in_directory_to_playlist(self, image_filepath):
        self.current_image_index = 0
        print("image filepath: " + image_filepath)
        directory = os.path.dirname(image_filepath)
        for filename in os.listdir(directory):
            if filename.endswith((".jpg", ".png")):
                filepath = os.path.join(directory, filename)
                print("Adding to playlist: " + filepath)

                if filepath == image_filepath:
                    self.current_image_index = len(self.playlist)
                    print("Current image index: " + str(self.current_image_index))
                self.playlist.append(filepath)
        pass

    def shift_image_index(self, shift_amount):
        if self.image_canvas.has_image():
            self.current_image_index = (self.current_image_index + shift_amount) % len(self.playlist)
            self.set_and_scale_image(self.playlist[self.current_image_index])
            #self.set_and_cache_image(self.playlist[self.current_image_index])

    def set_and_cache_image(self, image_filepath):
        if image_filepath not in self.cached_images:
            self.cached_images[image_filepath] = omnific.Image(image_filepath)
        self.image_canvas.set_image(self.cached_images[image_filepath])

    def open_image(self, image_filepath):
        self.add_other_files_in_directory_to_playlist(image_filepath)
        self.set_and_scale_image(self.playlist[self.current_image_index])

    def set_and_scale_image(self, image_filepath):
        image = omnific.Image(image_filepath)
        self.image_canvas.set_image(image)
        filename = omnific.get_file_access().get_file_name(image_filepath)
        image_width = image.get_width()
        image_height = image.get_height()
        omnific.get_window().change_title(self.app_name + " - " + " [" + str(self.current_image_index + 1) + 
                                             "/" + str(len(self.playlist)) + "] " + filename + 
                                             " (" + str(image_width) + "x" + str(image_height) + ")")
        self.scale_image_to_window()

    def scale_image_to_window(self):
        if self.image_canvas.has_image():
            image = self.image_canvas.get_image()
            image_width = image.get_width()
            image_height = image.get_height()
            window = omnific.get_window()
            window_width = window.get_window_size().x
            window_height = window.get_window_size().y

            if image_width > image_height:
                self.image_canvas_entity.get_transform().set_xy_scale(window_width / image_width)
            else:
                self.image_canvas_entity.get_transform().set_xy_scale(window_height / image_height)
