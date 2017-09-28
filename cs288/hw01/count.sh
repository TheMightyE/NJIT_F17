#!/bin/bash

function count() {
	local commands=(`ls /bin`)
	local len=${#commands[@]}
	local count=0
	local evaluated=''

	for ((i=0; i<$len; i++)); do

		if [ "$evaluated" != "${commands[i]:0:1}" ]; then
			evaluated=${commands[i]:0:1}
			for ((j=1; j<$len; j++)); do
				if [ "$evaluated" = "${commands[j]:0:1}" ]; then
					((count++))
				fi	
			done
		else
			continue
		fi

		echo "$evaluated $count"
		count=0
	
	done


}


function main() {
	count

}

main
