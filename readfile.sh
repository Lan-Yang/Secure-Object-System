#! /bin/bash
# read userfile.txt
# set up users and groups
while read line
do 
	a=($line)
	num=${#a[*]}
	echo $a
	echo $num
	if [ $num -lt 2 ]
	then
		continue
	else
		username=${a[0]}
		groupname=${a[1]}
		groupadd $groupname
		useradd -g $groupname $username
		for (( i = 0;$i < ($num-2);i=$i+1 )) ;
		do
			g[$i]=${a[$i+2]};
			groupadd ${g[$i]};
			usermod $username -a -G ${g[$i]}; 
			echo ${g[$i]};
		done	
	echo $username:$username | chpasswd	
	fi
done < $1
