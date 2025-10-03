#!/usr/bin/env python3 
import os 
import sys 
import shutil 
import subprocess
from pathlib import Path
class BuildSystem:
    def __init__(self):
        self.build_dir = Path("release")
    def clear_bin_files(self):
        print('Cleaning temporary files...''')
        for item in Path(".").rglob("*"):
            if item.is_file():
                if item.suffix not in ['.so','.rl']:
                    item.unlink()
                elif item.is_dir():
                    if item.name in ['CMakeFiles','.cmake']:
                        shutil.rmtree(item,ignore_errors=True)
    def run_command(self, cmd, cwd=None):
        print(f"Execution: {' '.join(cmd)} ...")
        result = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"Error: {result.stderr}")
            return False
        return True 
    def build_project(self, source_dir):
        print('Building project...')
        cmake_cmd = ['cmake', '-DCMAKE_CXX_COMPILER=clang++', str(source_dir)]
        if not self.run_command(cmake_cmd):
            return False
        build_cmd = ['make','-j',str(os.cpu_count())]
        if not self.run_command(build_cmd):
            return False
        self.clear_bin_files()
        return True
    def clone_and_build_mathplot(self):
        print('Clone and build MathPlot...')
        mathplot_dir = Path('MathPlot')
        if not mathplot_dir.exists():
            clone_cmd = ['git','clone','git@github.com:yaroslavsin11092004/MathPlot.git']
            if not self.run_command(clone_cmd):
                return False
        if not self.build_project(mathplot_dir):
            return False
        shutil.rmtree(mathplot_dir,ignore_errors=True)
        return True
    def main(self):
        print('Start building...')
        self.build_dir.mkdir(exist_ok=True)
        os.chdir(self.build_dir)
        if len(sys.argv) < 2:
            print("Использование: python build.py [-t|-p|-c|-a|-l|-as]")
            return
        option = sys.argv[1]
        projects = { 
            '-t' : Path('../TasksAlgoritm'),
            '-p' : Path('../Proxy'),
            '-c' : Path('../CoreAlgoritm'),
            '-a' : Path('../AdamAlgoritm'),
            '-as' : 'all'
        }
        if option == '-l':
            success = self.clone_and_build_mathplot()
        elif option == '-as':
            success = all(self.build_project(proj) for proj in [
                Path('../TasksAlgoritm'),
                Path('../Proxy'),
                Path('../CoreAlgoritm'),
                Path('../AdamAlgoritm')
            ])
        elif option in projects:
            success = self.build_project(projects[option])
        else:
            print(f"Unknown option: {option}")
            return
        if success:
            print('Building finish success!')
        else:
            print('Building finish with error!')
            sys.exit(1)

if __name__ == '__main__':
    BuildSystem().main()
