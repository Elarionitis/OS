#!/bin/bash
target=$((RANDOM % 50 + 1))
attempts=0
guess=0

while [ $guess -ne $target ]; do
    read -p "Enter guess: " guess
    attempts=$((attempts + 1))
    
    if [ $guess -gt $target ]; then
        echo "Too High"
    elif [ $guess -lt $target ]; then
        echo "Too Low"
    else
        echo "Correct!"
    fi
done

if [ $attempts -le 3 ]; then echo "Expert"
elif [ $attempts -le 6 ]; then echo "Good"
else echo "Keep Practising"
fi