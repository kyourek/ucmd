from os import path, listdir

def get_files(dir):
    files = [ ]
    for item in listdir(dir):
        item_path = path.join(dir, item)
        if path.isfile(item_path):
            files.append(item_path)
    return files
        
current_dir = path.dirname(path.realpath(__file__))
project_dir = path.dirname(path.dirname(current_dir))

cmd_line_dir = path.join(project_dir, "cmd_line")
cmd_line_tests_dir = path.join(project_dir, "cmd_line.tests")
cmd_line_example_dir = path.join(project_dir, "cmd_line.example")

for dir in [ cmd_line_dir, cmd_line_tests_dir, cmd_line_example_dir ]:
    source_dir = path.join(dir, "source")
    include_dir = path.join(dir, "include")
    include_p_dir = path.join(dir, "include_p")
    
    files = get_files(source_dir)
    
    print files
    