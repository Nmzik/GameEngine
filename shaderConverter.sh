#!/bin/bash
filename=$1.spv
./glslangValidator -V -l $1 $2 -o $filename
./spirv-cross $filename --msl --output result.msl
