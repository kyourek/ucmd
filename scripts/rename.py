import os
import os.path

def rename(path, prev, curr):
    parent, child = os.path.split(path)
    
    new_child = child.replace(prev, curr)
    if not new_child == child:
        path = os.path.join(parent, new_child)
        os.rename(os.path.join(parent, child), path)
    
    if os.path.isfile(path):
        with open(path, "r+") as f:
            content = f.read()
            f.seek(0)
            f.write(content.replace(prev, curr))
    
    if os.path.isdir(path):
        for item in os.listdir(path):
            item_path = os.path.join(path, item)
            if os.path.isdir(item_path) or os.path.isfile(item_path):
                rename(item_path, prev, curr)

current_dir = os.path.abspath(os.path.dirname(__file__))
projects_dir = os.path.join(os.pardir, os.path.dirname(current_dir))
projects = [ "cmd_line", "cmd_line.example", "cmd_line.tests" ]

for project in projects:
    project_dir = os.path.join(projects_dir, project)
    rename(project_dir, "uc_", "uc_")
    rename(project_dir, "UC_", "UC_")
    rename(project_dir, "cmd_line", "cmd_line")
    rename(project_dir, "CMD_LINE", "CMD_LINE")
