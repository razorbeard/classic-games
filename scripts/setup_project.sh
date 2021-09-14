#!/bin/bash

while true; do
    read -p "SFML development files will be installed if not found. Proceed ?" yn
    case $yn in
        [Yy]* ) echo "Asking permission..."; break;;
	[Nn]* ) echo "Permission denied. Aborting setup."; exit;;
	* ) echo "Please answer yes or no.";;
    esac
done

{
    APT_CMD=$(which apt)
    DNF_CMD=$(which dnf)
    PACMAN_CMD=$(which pacman)
    ZYPPER_CMD=$(which zypper)
} &> /dev/null

if [[ ! -z $APT_CMD ]]; then
    sudo apt-get update && sudo apt-get install libsfml-dev
elif [[ ! -z $DNF_CMD ]]; then
    sudo dnf update && sudo dnf install SFML-devel
elif [[ ! -z $PACMAN_CMD ]]; then
    sudo pacman -Syu sfml
elif [[ ! -z $ZYPPER_CMD ]]; then
    sudo zypper install sfml2-devel
else
    echo "Package manager not found."
    echo "Check https://pkgs.org to find SFML development files for your Linux distribution."
fi

VENDOR_DIR=../vendor
PREMAKE_DIR=$VENDOR_DIR/premake/bin
PREMAKE_APP_URL="https://github.com/premake/premake-core/releases/download/v5.0.0-alpha16/premake-5.0.0-alpha16-linux.tar.gz"
PREMAKE_LICENSE_URL="https://raw.githubusercontent.com/premake/premake-core/master/LICENSE.txt"

if [ ! -d $PREMAKE_DIR ]; then
    mkdir -p $PREMAKE_DIR
fi

echo "Downloading Premake..."
wget -q $PREMAKE_APP_URL -O - | tar -xzf - -C $PREMAKE_DIR 

echo "Downloading Premake's license..."
wget -q $PREMAKE_LICENSE_URL -P $PREMAKE_DIR

echo
echo "Setup finished. Launch the script file 'generate_project.sh' to build the application."
