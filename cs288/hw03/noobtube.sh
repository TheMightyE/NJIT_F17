#!/bin/bash

declare -a titles
declare -a durations
declare -a views
declare -a users
IFS=$'\n'

function get_titles(){
    local e="dir=\"ltr\(.*\)</a><s"
    local e2=">.*</"
    local a=(`grep -o $e index.html | grep -o $e2`)
    local b=()
    for ((i=0; i<${#a[@]}; i++)); do
	b+=("${a[i]//>/}")
    done

    for ((i=0; i<${#b[@]}; i++)); do
	titles+=("${b[i]//<\//}")
    done
    
    for ((i=0;i<${#titles[@]};i++)); do
        titles[i]=${titles[i]//,/ COMMA}
    done
}

function get_views() {
    local file=$1
    local e="\([0-9,]\+\)\sviews"
    local e2="\([0-9,]\+\)"
    local a=(`grep -o $e $file | grep -o $e2`)
    local b=()
    for ((i=0; i<${#a[@]}; i++)); do
       views+=("${a[i]//,/}")
    done
    #echo "${views[@]}"
}

function get_duration(){
    local file=$1
    local e="Duration:\s\([0-9]\+:[0-9]\+\)"
    local e2="[0-9]\+:[0-9]\+"
    durations=(`grep -o $e $1 | grep -o $e2`)
   #echo "${durations[@]}"
    
}

function get_user(){
    local file=$1
    local e="data-ytid\(.*\)views"
    local e2=">\(.*\)</a"
    local e3=">.*<"
    a=(`grep -o $e index.html | grep  -o $e2 | grep -o $e3`)
    local b=()
    for ((i=0; i<${#a[@]}; i++)); do
	b+=("${a[i]//>/}")
    done

    for ((i=0; i<${#b[@]}; i++)); do
	users+=("${b[i]//</}")
    done
     
}

function concat_results(){
    echo "User, Duration, Views, Title" | paste -d "," > results.csv
    for ((i=0;i<${#titles[@]};i++)); do
	echo "${users[i]}, ${durations[i]}, ${views[i]}, ${titles[i]}" | paste -d "," >> results.csv
    done
    
}


function main(){
    get_views $1
    get_titles $1
    get_duration $1
    get_user $1
    concat_results
    
}


main $1
