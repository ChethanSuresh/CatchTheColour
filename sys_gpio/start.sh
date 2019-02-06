#!/bin/bash

while true
do
  /home/chethan/CatchTheColour/sys_gpio/catch_the_colour
  for (( i=0; i<10; i++ ))
  do
    echo "Game restarting in...$i"
    sleep 1 
  done
  clear
  echo "GAME RESTART"
done 
