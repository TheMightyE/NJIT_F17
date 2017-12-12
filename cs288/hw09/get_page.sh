#!/bin/bash

function download_page(){
    local url=$1
    if [ -z "$url" ]; then
	url="http://www.wsj.com/mdc/public/page/2_3021-activnyse-actives.html"
    fi
    
    # download page for one hour
    for ((i=0; i<60; i++)); do
	timestamp
	t="${time}.html"
	`wget -O $t $url`
	sleep 60s

    done

}

function timestamp(){
    time=`date +"%Y-%m-%d_%H-%M-%S"`
}

function main(){
    download_page $1
}


main $1
