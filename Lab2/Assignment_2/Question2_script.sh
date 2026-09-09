#!/bin/bash
echo "File: $1"
echo "Lines: $(wc -l < $1)"
echo "Words: $(wc -w < $1)"
echo "Characters: $(wc -m < $1)"
echo "Longest Line:"
awk '{ if (length($0) > max) {max = length($0); line = $0; nr = NR} } END { print "Line " nr " -> \"" line "\"" }' $1
echo "Most Frequent Word:"
tr -c '[:alnum:]' '[\n*]' < $1 | tr '[:upper:]' '[:lower:]' | grep -v '^$' | sort | uniq -c | sort -nr | head -n 1