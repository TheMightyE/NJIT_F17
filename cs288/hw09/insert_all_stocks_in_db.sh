#!/bin/bash
IFS=$'\n'
function main(){
    prog_name="extract_data.py"
    folder="stocks"
    html_files=(`ls -1 $folder`)
    
    for ((i=0; i<${#html_files[@]}; i++)); do
	echo "Inserting ${html_files[i]}"
	
	# run python program
	`python $prog_name $folder/${html_files[$i]}`
	
	#clean up: remove the generated .xhtml files
	`rm $folder/*.x*`

    done
    
}

main
