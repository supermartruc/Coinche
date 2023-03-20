#!/bin/bash

generateTest(){
    printf "c\n192.168.1.79\nmartin" > generate.txt
    printf $1 >> generate.txt
    printf "\n" >> generate.txt
}
export -f generateTest

# Define a function
clientProcess() {
    generateTest $1
    cat generate.txt - | ./bin/coinche
}

export -f clientProcess

serveurProcess() {
    printf "s\n" > generate.txt
    cat generate.txt - | ./bin/coinche
}


export -f serveurProcess

# Open a new terminal window and execute the function
loop(){
    make
    gnome-terminal -- bash -c "serveurProcess; exec bash"

    count=1

    while [ $count -le $1 ];
    do
    gnome-terminal -- bash -c "clientProcess $count; exec bash"
    ((count++))
    done
}

loop 4