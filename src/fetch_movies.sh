#!/usr/bin/env bash
set -euo pipefail

# -----------------------------
# 1) Activate Miniconda env
# -----------------------------
source "$HOME/miniconda3/bin/activate"

# (optional) activate specific env
# conda activate myenv

# -----------------------------
# 2) Output folder
# -----------------------------
OUTDIR=./movie_json
mkdir -p "$OUTDIR"

# -----------------------------
# 3) Loop → fetch → insert
# -----------------------------
for id in {71650..71651}; do
    url="https://yts.lt/api/v2/movie_details.json?movie_id=$id"
    out="$OUTDIR/movie_${id}.json"

    echo "Fetching $id -> $out"
    
    # download JSON
    if ! curl -sSf "$url" -o "$out"; then
        echo "❌ failed to fetch $id"
        continue
    fi

    # pass JSON to python script
    if ! python3 insert_movie.py "$out"; then
        echo "❌ insert failed for movie $id"
    else
        echo "✔ inserted $id"
    fi
done
