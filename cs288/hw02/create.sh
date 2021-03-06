#!/bin/bash

function depth_first(){
    d=$1
    b=$2
    seed=$3

    open=("$seed")
    closed=()
    for ((i=0; i<$d; i++)); do
	temp_open=()
	
	for x in ${open[@]}; do
	    temp=()
	    for ((j=0; j<$b; j++)); do
		temp=(${temp[@]} "$x/$j")
		mkdir -p $x/$j
		
	    done
	    temp_open=(${temp[@]} ${temp_open[@]})
	    echo "${temp[@]}"
		     
	done
	closed=(${closed[@]} $x)
	open=(${temp_open[@]})

    done

}

function breadth_first(){
    d=$1
    b=$2
    seed=$3

    open=("$seed")
    closed=()
    for ((i=0; i<$d; i++)); do
	temp_open=()
	
	for x in ${open[@]}; do
	    temp=()
	    for ((j=0; j<$b; j++)); do
		temp=(${temp[@]} "$x/$j")
		mkdir -p $x/$j
		
	    done
	    temp_open=(${temp_open[@]} ${temp[@]})
	     #echo "${temp[@]}"
		     
	done
	closed=(${closed[@]} $x)
	open=(${temp_open[@]})

    done
    
    
}

function main(){
    d=$1
    b=$2
    seed=$3
    tree_type=$4

    if [ "$tree_type" = "depth" ]; then
	depth_first $1 $2 $3
    fi

    if [ "$tree_type" = "breadth" ]; then
	breadth_first $1 $2 $3
    fi
}

main $1 $2 $3 $4


