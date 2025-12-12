#!/usr/bin/env bash
set -e

# activate miniconda
source "$HOME/miniconda3/bin/activate"

# run python script
python3 query.py
