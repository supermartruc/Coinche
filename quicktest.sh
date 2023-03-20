#!/bin/bash

generateTest(){
    printf "c\n140.77.183.80\n$2\nmartinbot" > generate.txt
    printf $1 >> generate.txt
    printf "\n" >> generate.txt
}
export -f generateTest

# Define a function
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
    echo $2
    make
    gnome-terminal -- bash -c "serveurProcess $2; exec bash"

    count=1

    while [ $count -le $1 ];
    do
    gnome-terminal -- bash -c "clientProcess $count $2; exec bash"
    ((count++))
    done
}

loop 4 $1