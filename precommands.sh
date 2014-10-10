#! /bin/bash
# Set up an empty repository to store objects
# The owner of the repository is a unprivileged one
chmod u+s objput objget objlist objgetacl objsetacl objtestacl
useradd ly2331
echo ly2331:ly2331 | chpasswd
mkdir lanyang
chown ly2331 lanyang
chgrp ly2331 lanyang
chmod 700 lanyang
