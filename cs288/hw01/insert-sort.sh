#!/bin/bash

function sort() {
    local a=($@)
        
    for ((i=1; i<${#a[@]}; i++)); do
	j=$i-1
	k=${a[i]}

	while (($j>=0 && ${a[j]}>$k)); do
	    a[j+1]=${a[j]}
	    ((j--))
	done

	a[j+1]=$k	
	
    done

    echo "${a[@]}"
    
}

function main() {
    sort $@
}

main $@

