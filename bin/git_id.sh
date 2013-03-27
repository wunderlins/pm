#!/usr/bin/env bash

ver=$(git log --max-count=1 "$1" | {
	a="`cat /dev/stdin;`"; 
	echo "$a" | awk -F' ' '/^commit/ { printf "%s\t", $2 }'; 
	echo "$a" | awk -F' ' '/^Date/ { print $2" "$3" "$4" "$5" "$6" "$7 }';  
})

if [[ x"$1" != x"$2" ]]; then
	cp "$1" "$2"
fi

sed -i -e 's/\$Id\$/'"$ver"'/' "$2"
