import os
import sys
import shutil
import tarfile
import platform
import subprocess as sub
from zipfile import ZipFile 
from distutils.util import get_platform
import subprocess as sub

try:
    from urllib.request import urlopen # Python 3.x
except ImportError:
    from urllib2 import urlopen # Python 2


libraries = ['SDL2', 'SDL2_mixer', 'SDL2_ttf', 'SDL2_image', 'SDL2_gfx']

sdl2_urls = {
    'SDL2': 'https://www.libsdl.org/release/SDL2-{0}{1}',
    'SDL2_mixer': 'https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-{0}{1}',
    'SDL2_ttf': 'https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-{0}{1}',
    'SDL2_image': 'https://www.libsdl.org/projects/SDL_image/release/SDL2_image-{0}{1}',
    'SDL2_gfx': 'https://github.com/a-hurst/sdl2gfx-builds/releases/download/{0}/SDL2_gfx-{0}{1}'
}

libversions = {
    '2.0.20': {
        'SDL2': '2.0.20',
        'SDL2_mixer': '2.0.4',
        'SDL2_ttf': '2.0.18',
        'SDL2_image': '2.0.5',
        'SDL2_gfx': '1.0.4'
    },
    '2.0.18': {
        'SDL2': '2.0.18',
        'SDL2_mixer': '2.0.4',
        'SDL2_ttf': '2.0.15',
        'SDL2_image': '2.0.5',
        'SDL2_gfx': '1.0.4'
    },
    '2.0.16': {
        'SDL2': '2.0.16',
        'SDL2_mixer': '2.0.4',
        'SDL2_ttf': '2.0.15',
        'SDL2_image': '2.0.5',
        'SDL2_gfx': '1.0.4'
    },
    '2.0.14': {
        'SDL2': '2.0.14',
        'SDL2_mixer': '2.0.4',
        'SDL2_ttf': '2.0.15',
        'SDL2_image': '2.0.5',
        'SDL2_gfx': '1.0.4'
    },
    '2.0.12': {
        'SDL2': '2.0.12',
        'SDL2_mixer': '2.0.4',
        'SDL2_ttf': '2.0.15',
        'SDL2_image': '2.0.5',
        'SDL2_gfx': '1.0.4'
    },
    '2.0.10': {
        'SDL2': '2.0.10',
        'SDL2_mixer': '2.0.4',
        'SDL2_ttf': '2.0.15',
        'SDL2_image': '2.0.5',
        'SDL2_gfx': '1.0.4'
    },
    '2.0.9': {
        'SDL2': '2.0.9',
        'SDL2_mixer': '2.0.4',
        'SDL2_ttf': '2.0.14',
        'SDL2_image': '2.0.4',
        'SDL2_gfx': '1.0.4'
    },
    '2.0.8': {
        'SDL2': '2.0.8',
        'SDL2_mixer': '2.0.2',
        'SDL2_ttf': '2.0.14',
        'SDL2_image': '2.0.3',
        'SDL2_gfx': '1.0.4'
    },
    '2.0.7': {
        'SDL2': '2.0.7',
        'SDL2_mixer': '2.0.2',
        'SDL2_ttf': '2.0.14',
        'SDL2_image': '2.0.2',
        'SDL2_gfx': '1.0.4'
    },
    '2.0.6': {
        'SDL2': '2.0.6',
        'SDL2_mixer': '2.0.1',
        'SDL2_ttf': '2.0.14',
        'SDL2_image': '2.0.1',
        'SDL2_gfx': '1.0.4'
    },
    '2.0.5': {
        'SDL2': '2.0.5',
        'SDL2_mixer': '2.0.1',
        'SDL2_ttf': '2.0.14',
        'SDL2_image': '2.0.1',
        'SDL2_gfx': '1.0.4'
    }
}


def getDLLs(platform_name, version):
    
    dlldir = os.path.join('dlls')
    for d in ['temp', dlldir]:
        if os.path.isdir(d):
            shutil.rmtree(d)
        os.mkdir(d)
    
    if 'macosx' in platform_name:
        
        for lib in libraries:
            
            mountpoint = '/tmp/' + lib
            dllname = lib + '.framework'
            dllpath = os.path.join(mountpoint, dllname)
            dlloutpath = os.path.join(dlldir, dllname)
            
            # Download disk image containing library
            libversion = libversions[version][lib]
            dmg = urlopen(sdl2_urls[lib].format(libversion, '.dmg'))
            outpath = os.path.join('temp', lib + '.dmg')
            with open(outpath, 'wb') as out:
                out.write(dmg.read())
            
            # Mount image, extract framework, then unmount
            sub.check_call(['hdiutil', 'attach', outpath, '-mountpoint', mountpoint])
            shutil.copytree(dllpath, dlloutpath, symlinks=True)
            sub.call(['hdiutil', 'unmount', mountpoint])

    elif platform_name in ['win32', 'win-amd64']:
        
        suffix = '-win32-x64.zip' if platform_name == 'win-amd64' else '-win32-x86.zip'
        
        for lib in libraries:
            
            # Download zip archive containing library
            libversion = libversions[version][lib]
            dllzip = urlopen(sdl2_urls[lib].format(libversion, suffix))
            outpath = os.path.join('temp', lib + '.zip')
            with open(outpath, 'wb') as out:
                out.write(dllzip.read())
            
            # Extract dlls and license files from archive
            with ZipFile(outpath, 'r') as z:
                for name in z.namelist():
                    if name[-4:] == '.dll':
                        z.extract(name, dlldir)
                        
    else:

        suffix = '.tar.gz' # source code
        gfxsrc = 'http://www.ferzkopp.net/Software/SDL2_gfx/SDL2_gfx-{0}.tar.gz'
        cfgurl = 'https://git.savannah.gnu.org/gitweb/?p=config.git;a=blob_plain;f={0};hb=HEAD'
        basedir = os.getcwd()
        arch = os.uname()[-1]

        libdir = os.path.join(basedir, 'sdlprefix')
        if os.path.isdir(libdir):
            shutil.rmtree(libdir)
        os.mkdir(libdir)

        # Pre-fetch updated config.guess and config.sub scripts (needed for gfx on ARM & PPC)
        cfgfiles = {}
        cfgnames = ['config.guess', 'config.sub']
        for name in cfgnames:
            cfgfiles[name] = urlopen(cfgurl.format(name)).read()

        for lib in libraries:

            libversion = libversions[version][lib]
            print('\n======= Downloading {0} {1} =======\n'.format(lib, libversion))
            
            # Download tar archive containing source
            liburl = sdl2_urls[lib].format(libversion, suffix)
            if lib == 'SDL2_gfx':
                liburl = gfxsrc.format(libversion)
            srctar = urlopen(liburl)
            outpath = os.path.join('temp', lib + suffix)
            with open(outpath, 'wb') as out:
                out.write(srctar.read())
            
            # Extract source from archive
            sourcepath = os.path.join('temp', lib + '-' + libversion)
            with tarfile.open(outpath, 'r:gz') as z:
                z.extractall(path='temp')

            # Update config.guess & config.sub files, if they exist
            for name in cfgnames:
                filepath = os.path.join(sourcepath, name)
                if os.path.exists(filepath):
                    os.remove(filepath)
                    with open(filepath, 'wb') as out:
                        out.write(cfgfiles[name])

            # Build the library
            print('======= Compiling {0} {1} =======\n'.format(lib, libversion))
            buildcmds = [
                ['./configure', '--prefix={0}'.format(libdir)],
                ['make'],
                ['make', 'install']
            ]
            if lib == 'SDL2_gfx' and not arch in ['i386', 'x86_64']:
                buildcmds[0].append('--disable-mmx')
            os.chdir(sourcepath)
            for cmd in buildcmds:
                p = sub.Popen(cmd, stdout=sys.stdout, stderr=sys.stderr)
                p.communicate()
                if p.returncode != 0:
                    raise RuntimeError("Error building {0}".format(lib))
            os.chdir(basedir)

            # Copy built library to dll folder and reset working dir
            print('\n======= {0} {1} built sucessfully =======\n'.format(lib, libversion))
            for f in os.listdir(os.path.join(libdir, 'lib')):
                if f == "lib{0}.so".format(lib):
                    fpath = os.path.join(libdir, 'lib', f)
                    if os.path.islink(fpath):
                        fpath = os.path.realpath(fpath)
                    shutil.copy(fpath, dlldir)

        print("Installed binaries:")
        print(os.listdir(dlldir))
            

if __name__ == '__main__':
    version = os.getenv('PYSDL2_DLL_VERSION')
    if version:
        getDLLs(get_platform(), version)
