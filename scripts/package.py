import os
import os.path
import zipfile

class Project:
    def __init__(self, name, dirs):
        self.name = name;
        self.dirs = dirs;

current_dir = os.path.abspath(os.path.dirname(__file__))
projects_dir = os.path.join(os.pardir, os.path.dirname(current_dir))
project_file_dirs = [ "include", "include_p", "source" ]
projects = [
    Project("cmd_line", [ "cmd_line" ]),
    Project("cmd_line_tests", [ "cmd_line", "cmd_line.tests" ]),
    Project("cmd_line_example", [ "cmd_line", "cmd_line.example" ])
]

for project in projects:
    files = [ ]
    for project_dir in project.dirs:
        project_dir = os.path.join(projects_dir, project_dir)
        for project_file_dir in project_file_dirs:
            file_dir = os.path.join(project_dir, project_file_dir)
            if os.path.isdir(file_dir):
                dir_items = os.listdir(file_dir)
                for dir_item in dir_items:
                    if not dir_item.startswith("main"):
                        item_path = os.path.join(file_dir, dir_item)
                        if os.path.isfile(item_path):
                            files.append((item_path, dir_item))

    zout = zipfile.ZipFile(os.path.join(current_dir, project.name + ".zip"), "w")
    for file in files:
        zout.write(file[0], arcname = file[1])
    zout.close()
