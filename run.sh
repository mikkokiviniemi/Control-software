#!/bin/bash

gnome-terminal -- build/main

gnome-terminal -- build/src/log

gnome-terminal -- build/src/text_ui

cd Sim/build/
./simulation 


