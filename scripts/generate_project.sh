#!/bin/bash

PREMAKE_DIR=/vendor/premake/bin

cd .. 
$PWD/$PREMAKE_DIR/premake5 gmake2
