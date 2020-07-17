#!/bin/bash
# Author : Serg Kolo
# Date: April 11,2015
# Description: Open gnome-terminal with 4 tabs, and focus on tab with particular name
#
# set -x
echo "Which window to focus ? "
read FOCUS

gnome-terminal --tab -t $1  --tab -t $2  --tab -t  $3 --tab -t $4

if [ $? -eq 0 ]; then
    sleep 0.5
    WINID=$(xprop -root | awk '/_NET_ACTIVE_WINDOW/ && /0x/ {print $5}' | cut -d'x' -f2)


    while [ $(wmctrl -l | grep $WINID | awk '{print $4}') != $FOCUS ]; do
        xdotool key Ctrl+Page_Up
    done

fi