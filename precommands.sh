#! /bin/bash
# The owner of the repository is a unprivileged one
#userdel ly2331
useradd ly2331
#echo ly2331:ly2331 | chpasswd
# Set up an empty repository to store objects
mkdir lanyang
chown ly2331 lanyang
chgrp ly2331 lanyang
chmod 700 lanyang
