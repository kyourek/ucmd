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

    def get_files(self):
        files = []
        for dir in self.dirs:
            dir = os.path.join(self.root, dir)
            for file_dir in [ 'include', 'include_p', 'source' ]:
                file_dir = os.path.join(dir, file_dir)
                if not os.path.isdir(file_dir):
                    continue
                dir_items = os.listdir(file_dir)
                for dir_item in dir_items:
                    if dir_item.startswith('main'):
                        continue
                    item_path = os.path.join(file_dir, dir_item)
                    if not os.path.isfile(item_path):
                        continue
                    files.append(ProjectFile(item_path, dir_item))
        return files

    def zip_files(self):
        fout = os.path.join(script_dir, self.name + '.zip')
        zout = zipfile.ZipFile(fout, 'w')
        with zout:
            files = self.get_files()
            for file in files:
                zout.write(file.path, arcname=file.name)
            zout.close()

projects = [
    Project('ucmd', ['ucmd']),
    Project('ucmdtests', ['ucmd', 'ucmdtests']),
    Project('ucmdexample', ['ucmd', 'ucmdexample'])]

def main():
    for project in projects:
        project.zip_files()

if __name__ == '__main__':
    main()