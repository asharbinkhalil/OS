#! /usr/bin/bash
#Script to print user information whocurrently login, current date and time
clear
echo "Hello $USER"
#echoinducenextlineattheend
#\crestrictoutputonsamelineandusedwithflage
echo "Todayis\c";date
#printthelinecount
echo "Number of lines ,total words and characters:";
ls -l| wc
exit
