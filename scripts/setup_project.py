import sys
import os
import subprocess
from importlib import util
from pathlib import Path
from zipfile import ZipFile

# Detect Python version (3.6 or higher required for requests module)
if sys.version is not None:
    majorVersion = sys.version_info.major
    minorVersion = sys.version_info.minor
    
    if majorVersion < 3 or (majorVersion == 3 and minorVersion < 6):
        sys.exit("Python version too low, expected version 3.6 or higher.\n"
        "Please upgrade your current version before pursuing the setup.\n")

# Ask user agreement for installing the needed prerequisites
prerequisites = [
    "Python's requests module",
    "SFML 2.5.1 headers and libraries",
    "Premake5"
    ]

print("For this project, the following prerequisites will be installed if not found :")
print(*["-> {0}".format(i) for i in prerequisites], sep = '\n')

permissionGranted = False
while not permissionGranted:
    reply = input("\nProceed ? (y/n): ")
    if reply == 'n':
        sys.exit("Permission denied. Aborting...\n")
    permissionGranted = (reply == 'y')

# Install Python's requests module
if util.find_spec("requests") is None:
    print("Installing package 'requests'")
    print("="*60)

    subprocess.check_call("python -m pip install --user requests")

import requests
os.chdir('./../') # Go back to root folder

def download_file(filepath, url):
    fileName = os.path.basename(filepath)
    absolutePath = os.path.abspath(filepath)
    fileLocation = os.path.dirname(absolutePath)
    os.makedirs(fileLocation, exist_ok = True)

    print("Downloading {0:s}...".format(fileName), end = " ", flush = True)
    with requests.get(url, stream = True) as response:
        with open( absolutePath, "wb") as f:
            for chunk in response.iter_content(chunk_size = 10*1024):
                f.write(chunk)

    print("Done.\nFile location : {0:s}\n".format(fileLocation))

# Install Premake
premakeVersion    = "5.0.0-alpha16"
premakeZipUrls    = f"https://github.com/premake/premake-core/releases/download/v{premakeVersion}/premake-{premakeVersion}-windows.zip"
premakeLicenseUrl = "https://raw.githubusercontent.com/premake/premake-core/master/LICENSE.txt"
premakeDirectory  = "./vendor/premake/bin"

if (not Path(f"{premakeDirectory}/premake5.exe").exists()):
    print(f"Installing Premake v{premakeVersion}")
    print("="*60)

    premakePath = f"{premakeDirectory}/premake-{premakeVersion}-windows.zip"
    premakeAbsolutePath = os.path.abspath(premakePath)
    download_file(premakePath, premakeZipUrls)

    # Extract the executable and delete zip file
    print("Extracting all the content...", end = " ", flush = True)
    with ZipFile(premakeAbsolutePath, 'r') as zipObj:
        zipObj.extractall(os.path.dirname(premakeAbsolutePath))
    print("Done.\n")
    os.remove(premakeAbsolutePath)

    # Download the license file
    premakeLicensePath = f"{premakeDirectory}/LICENSE.txt"
    download_file(premakeLicensePath, premakeLicenseUrl)

# Install SFML headers and libraries
sfmlVersion    = "2.5.1"
sfmlZipUrls    = f"https://github.com/SFML/SFML/releases/download/{sfmlVersion}/SFML-{sfmlVersion}-windows-vc15-64-bit.zip"
sfmlDirectory  = f"./vendor/SFML-{sfmlVersion}"

if (not Path(sfmlDirectory).exists()):
    print(f"Installing SFML-{sfmlVersion}")
    print("="*60)

    sfmlPath = f"./vendor/SFML-{sfmlVersion}-windows-vc15-64-bit.zip"
    sfmlAbsolutePath = os.path.abspath(sfmlPath)
    download_file(sfmlPath, sfmlZipUrls)

    print("Extracting only headers, librairies and license...", end = " ", flush = True)
    with ZipFile(sfmlAbsolutePath, 'r') as zipObj:
        listOfFileNames = zipObj.namelist()
        
        # We only need headers, librairies and the license
        for fileName in listOfFileNames:
            if fileName.endswith((".lib", ".pdb")) or any(str in fileName for str in ["license", "include/"]):
                zipObj.extract(fileName, "./vendor")
    
    print("Done.\n")
    os.remove(sfmlAbsolutePath)

print("Setup finished. Launch the script file 'generate_project.bat' to generate Visual Studio project files.\n")