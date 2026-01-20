import glob
import os

def __init__(self):
    self.local_files = []
    os.chdir(".")

def scan_current_directory():
    for file in glob.glob("*.*"):
        local_files.append(file)