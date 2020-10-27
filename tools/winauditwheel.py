# An attempt to emulate auditwheel for Windows
# Author: Dmytro Kalpakchi, 2020
import os
import glob
import sys
import platform
import argparse
import shutil
import sysconfig
import zipfile
import hashlib
from pathlib import Path

import pefile
from machomachomangler.pe import redll

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--package', type=str, help='The name of your package (to be used in name mangling)')
    parser.add_argument('-v', '--version', type=str, help='The version of your package (to be used in name mangling)')
    args = parser.parse_args()

    if platform.system().lower() != 'windows':
        print('Error: This tool only supports Windows')
        sys.exit(1)

    path_info = sysconfig.get_paths()
    arch = os.environ["PYTHON_ARCH"]

    print("winauditwheel started...")

    # 1. Find DLL names
    dlls = set()
    pyd_files = glob.glob(os.path.join('build', 'lib*', args.package, '*.pyd'))
    print("Checking .pyd files:", pyd_files)
    for dll_fname in pyd_files:
        p = pefile.PE(dll_fname)
        for entry in p.DIRECTORY_ENTRY_IMPORT:
            dlls.add(entry.dll)
    print("Found DLL names", dlls)

    # 2. Find the actual DLL files
    search_paths = list([os.environ['BOOST_DIR'], path_info['stdlib']] + sys.path)
    redistribute = {}
    for dll in dlls:
        sdll = dll.decode('ascii')
        if 'KERNEL' in sdll or 'api-ms' in sdll or sdll.startswith('python'): continue

        for d in search_paths:
            for f in glob.glob(os.path.join(d, '**', sdll), recursive=True):
                redistribute[dll] = f

        for d in os.environ['PATH'].split(';'):
            for f in glob.glob(os.path.join(d, sdll)):
                p = pefile.PE(f)
                machine_type = pefile.MACHINE_TYPE[p.FILE_HEADER.Machine]
                if arch == '64' and machine_type != 'IMAGE_FILE_MACHINE_AMD64': continue
                if arch == '32' and machine_type != 'IMAGE_FILE_MACHINE_I386': continue
                redistribute[dll] = f
    print("Found actual DLLs", redistribute)

    # 3. Copy the DLLs to the current folder and change names
    salt = hashlib.sha1(str.encode("{}_{}".format(args.package, args.version))).hexdigest()
    dll_map = {}
    for bfname, src in redistribute.items():
        fname = bfname.decode('ascii')
        fn, ext = os.path.splitext(fname)
        dest = "{}.{}{}".format(fn, salt, ext)
        shutil.copyfile(src, os.path.join('dist', dest))
        dll_map[bfname] = dest.encode("ascii")
    print("DLL name mangling map", dll_map)

    # 4. Repair a wheel
    DIST_DIR = 'dist'
    for whl in glob.glob(os.path.join(DIST_DIR, '*.whl')):
        whl_repair_dir = os.path.join(DIST_DIR, 'whl_repair')
        with zipfile.ZipFile(whl, 'r') as zip_ref:
            zip_ref.extractall(whl_repair_dir)

        for pyd_file in glob.glob(os.path.join(whl_repair_dir, '**', '*.pyd'), recursive=True):
            fn, ext = os.path.splitext(pyd_file)

            with open(pyd_file, "rb") as f:
                buf = f.read()

            new_buf = redll(buf, dll_map)

            with open(pyd_file, "wb") as f:
                f.write(new_buf)

        with zipfile.ZipFile(whl, 'w') as new_whl:
            for folder, subfolders, filenames in os.walk(whl_repair_dir):
                for filename in filenames:
                    #create complete filepath of file in directory
                    file_path = os.path.join(folder, filename)
                    # Add file to zip
                    new_whl.write(file_path, os.path.join(folder.replace(whl_repair_dir, ''), filename))
            whl_path = Path(whl)
            for dll_file in glob.glob(os.path.join(whl_path.parent, '*.dll')):
                new_whl.write(dll_file, os.path.join(args.package, os.path.basename(dll_file)))

        # Clean up everything except a new wheel
        empty_folders = []
        for folder, subfolders, filenames in os.walk(DIST_DIR):
            for filename in filenames:
                if os.path.splitext(filename)[1] != '.whl':
                    file_path = os.path.join(folder, filename)
                    os.remove(file_path)
            if folder != DIST_DIR:
                empty_folders.append(folder)

        empty_folders.reverse()
        for fd in empty_folders:
            os.rmdir(fd)

    print("winauditwheel finished...")
