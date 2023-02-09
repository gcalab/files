#!/usr/bin/env bash

./main.py extract metadata
./main.py transform geoclimate
./main.py transform metadata
./main.py count substitutions
./main.py count sequences
./main.py extract features
./main.py count geotemporal
./main.py visualize