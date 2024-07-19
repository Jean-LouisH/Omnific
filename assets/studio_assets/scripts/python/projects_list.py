import xml.etree.ElementTree as ET

def __init__(self):
    self.editor_settings_path = ""
    self.project_list_file = "project_list.xml"

def add_to_list(path):
    # Is filepath valid?
    # Write to file
    pass

def remove_from_list(path):
    # Is filepath present?
    # Remove from file
    pass

def read_projects_list():
    file_path_list = []
    xml_tree = ET.parse(self.project_list_file)
    root = xml_tree.getroot()

    for file_path in iter('file_path'):
        file_path_list.add(file_path.text)

    # Get other configuration info
    # return structure representing all project metadata
    
    pass
    
