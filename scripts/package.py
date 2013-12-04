import os
import os.path
import zipfile

script_dir = os.path.abspath(os.path.dirname(__file__))

class ProjectFile(object):

    def __init__(self, path, name):
        self.path = path
        self.name = name

class Project(object):
    
    def __init__(self, name, dirs):

        self.name = name
        self.dirs = dirs
        self.root = os.path.join(os.pardir, os.path.dirname(script_dir))

    def get_config_files(self):
        
        files = [ ]
        items = [
            os.path.join("ucmd", "config", "uc_common.h"),
            os.path.join("ucmd", "config", "embedded", "uc_config.h"),
            os.path.join("ucmd", "config", "embedded", "default", "uc_embedded.h")
        ]
                 
        for item in items:
            file_path = os.path.join(self.root, item)
            file_name = os.path.basename(file_path)
            files.append(ProjectFile(file_path, file_name))
    
        return files

    def get_project_files(self):

        files = self.get_config_files()
        for dir in self.dirs:
            dir = os.path.join(self.root, dir)
            for file_dir in [ "include", "include_p", "source" ]:
                file_dir = os.path.join(dir, file_dir)
                if os.path.isdir(file_dir):
                    dir_items = os.listdir(file_dir)
                    for dir_item in dir_items:
                        if not dir_item.startswith("main"):
                            item_path = os.path.join(file_dir, dir_item)
                            if os.path.isfile(item_path):
                                files.append(ProjectFile(item_path, dir_item))
        
        return files

    def zip_files(self):
        zout = zipfile.ZipFile(os.path.join(script_dir, self.name + ".zip"), "w")
        files = self.get_project_files()
        for file in files:
            zout.write(file.path, arcname = file.name)
        zout.close()

projects = [
    Project("ucmd", [ "ucmd" ]),
    Project("ucmd_tests", [ "ucmd", "ucmd.tests" ]),
    Project("ucmd_example", [ "ucmd", "ucmd.example" ])
]

for project in projects:
    project.zip_files()
