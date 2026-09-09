#!/bin/bash
mkdir -p backup
v=$(ls -d backup/v* 2>/dev/null | wc -l)
v=$((v + 1))
mkdir -p backup/v$v

for file in *.c; do
    cp $file backup/v$v/
    echo "Copied $file"
done

echo "Total versions: $(ls -d backup/v* | wc -l)"
echo "Total size: $(du -sh backup | cut -f1)"