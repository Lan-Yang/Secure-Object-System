#! /bin/bash
# read userfile.txt
# set up users and groups
while read line
do 
	a=($line)
	num=${#a[*]}
	if [ $num -lt 2 ]
	then
		continue
	else
		username=${a[0]}
		groupname=${a[1]}
		userdel -r $username
		groupadd $groupname
		useradd -g $groupname $username
		echo $username:$username | chpasswd
		mkdir /home/$username
		chown $username /home/$username
		for (( i = 0;$i < ($num-2);i=$i+1 )) ;
		do
			g[$i]=${a[$i+2]};
			groupadd ${g[$i]};
			usermod $username -a -G ${g[$i]}; 
		done	
	fi
done < $1
