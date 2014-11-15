#! /bin/bash
# The owner of the repository is a unprivileged one
#userdel ly2331
useradd ly2331
#echo ly2331:ly2331 | chpasswd
# Set up an empty repository to store objects
mkdir /home/lanyang
chown ly2331 /home/lanyang
chgrp ly2331 /home/lanyang
chmod 700 /home/lanyang
