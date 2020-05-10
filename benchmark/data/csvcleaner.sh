#!/bin/bash



echo "Time        UID       PID    %usr %system  %guest   %wait    %CPU   CPU  minflt/s  majflt/s     VSZ     RSS   %MEM  Command" > cipher.csv.work
less $1 | grep -v "#" | grep -v "Linux" >> cipher.csv.work
sed -i '/^[[:space:]]*$/d' cipher.csv.work
#sed -i -e 's/\./,/g' cipher.csv.work
tr -s " " < cipher.csv.work > cipher.csv.work2
#sed -i -e 's/ /;/g' cipher.csv.work2
sed -i -e 's/ /,/g' cipher.csv.work2
cp cipher.csv.work2 $1
rm *.work*
