#!/bin/bash
dir=obj

if [[ ! -e $dir ]]; then
    mkdir $dir
    mkdir $dir/Browser
    mkdir $dir/CSS
    mkdir $dir/CSS/Selectors
    mkdir $dir/DOM
    mkdir $dir/JS
    mkdir $dir/Layout
    mkdir $dir/Parser
    mkdir $dir/Terminal
    echo "Setup: C++ object directory's created (obj)"
    echo "Setup: done"
elif [[ ! -p $dir ]]; then
    echo "Setup: $dir root directory already exists"
    echo "Setup: abort"
fi
