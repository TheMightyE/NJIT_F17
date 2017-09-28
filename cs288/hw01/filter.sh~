#!/bin/bash

function average() {
    a=(`ls -l $1`)
    len=${#a[@]}
    size=()
    file=()
    sum=0
    
    for ((i=6; i<$len; i+=9)); do
	size+=("${a[i]}")
	((sum+=${a[i]}))
    done

    for ((i=10; i<$len; i+=9)); do
	file+=("${a[i]}")
    done
    
    k=${#size[@]}
    ((avg=$sum/$k))
    
    for ((i=0; i<$k; i++)); do
	if [ "${size[i]}" -gt "$avg" ]; then
	    echo "${file[i]}"
	fi
    done
    
    
}

function main() {
    average $1

}

main $1
