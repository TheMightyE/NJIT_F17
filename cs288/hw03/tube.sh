#!/bin/bash

function get_titles(){
    local e="dir=\"ltr\(.*\)</a><s"
    local e2=">.*</"
    local a=`grep -o $e index.html | grep -o $e2`
    local b=()
    local c=()
    for ((i=0; i<${#a[@]}; i++)); do
	b+=("${a[i]//>/}")
    done

    for ((i=0; i<${#b[@]}; i++)); do
	c+=("${b[i]//<\//}")
    done

    echo "${c[@]}"

}

function get_views() {
    local file=$1
    local e="\([0-9,]\+\)\sviews"
    local e2="\([0-9,]\+\)"
    local a=(`grep -o $e $file | grep -o $e2`)
    local b=()
    for ((i=0; i<${#a[@]}; i++)); do
       b+=("${a[i]//,/}")
    done
    echo "${b[@]}"

}

function get_duration(){
    local file=$1
    local e="Duration:\s\([0-9]\+:[0-9]\+\)"
    local e2="\([0-9]\+:[0-9]\+"
    local a=(`grep $e $1 | grep $e2`)
    
}


function main(){
    get_views $1
    get_titles $1
}


main $1
