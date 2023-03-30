#!/bin/bash

generateTest(){
    printf "c\n140.77.182.127\n$2\nmartinbot" > generate.txt
    printf $1 >> generate.txt
    printf "\n" >> generate.txt
}
export -f generateTest


clientProcess() {
    generateTest $1 $2
    cat generate.txt - | ./bin/coinche
}

export -f clientProcess

serveurProcess() {
    printf "s\n$1\n" > generate.txt
    cat generate.txt - | ./bin/coinche
}


export -f serveurProcess

# Open a new terminal window and execute the function
loop(){
    echo $1
    make
    gnome-terminal -- bash -c "serveurProcess $1; exec bash"

    count=1

    while [ $count -le 4 ];
    do
    gnome-terminal -- bash -c "clientProcess $count $1; exec bash"
    ((count++))
    done
}

loop $1