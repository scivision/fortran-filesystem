#!/usr/bin/env python3
"""
shows that pathlib.Path.iterdir is single-level, non-recursive
"""

from pathlib import Path
import argparse

p = argparse.ArgumentParser()
p.add_argument("path", help="top-level directory to get subdirectories of")
args = p.parse_args()

top = Path(args.path)

dirs = (d for d in top.iterdir() if d.is_dir())
for d in dirs:
    print(d)
