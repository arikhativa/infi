#!/usr/bin


is_it_there(){
	find . -name $1 | wc -l
}


while [ "$1" != "" ]; do
	VAR=$(is_it_there $1)

	if [ $VAR = "1" ]; then 
		cp $1 kk
	fi
	shift
done
