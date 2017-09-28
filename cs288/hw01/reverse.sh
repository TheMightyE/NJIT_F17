#!/bin/bash

function reverse() {
	local dir=(`ls $1`)
	local rev_dir=()
	local len=${#dir[@]}

	for ((i=len; i>=0; i--)); do
		rev_dir+=(${dir[i]})
	done

	echo "${rev_dir[@]}"

}



function main() {
	reverse $1

}


main $1
