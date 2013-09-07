import os
import os.path
import zipfile

compression = zipfile.ZIP_DEFLATED
try:
    import zlib
except:
    compression = zipfile.ZIP_STORED

current_dir = os.path.abspath(os.path.dirname(__file__))
projects_dir = os.path.join(os.pardir, os.path.dirname(current_dir))
projects = [ "cmd_line" ]
project_file_dirs = [ "include", "include_p", "source" ]

files = [ ]

for project in projects:
    project_dir = os.path.join(projects_dir, project)
    for project_file_dir in project_file_dirs:
        file_dir = os.path.join(project_dir, project_file_dir)
        if os.path.isdir(file_dir):
            dir_items = os.listdir(file_dir)
            for dir_item in dir_items:
                item_path = os.path.join(file_dir, dir_item)
                if os.path.isfile(item_path):
                    files.append((item_path, dir_item))

zout = zipfile.ZipFile(os.path.join(current_dir, "cmd_line.zip"), "w")
for file in files:
    zout.write(file[0], arcname = file[1], compress_type = compression)
zout.close()
