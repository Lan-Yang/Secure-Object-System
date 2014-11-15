# Makefile

# List the object files in one place
OBJ=objgetacl objput objget objlist objsetacl objtestacl

#options
CXXFLAGS=-c -Wall -Werror -std=c++0x
LDFLAGS=-lssl -lcrypto

# The first target is the default if you just say "make".  In this
# case, "build" relies on "sample", because I want the executable to be
# called "sample"

build:	$(OBJ)
	@cat /dev/null > userfile.txt

# "sample" requires a set of object files
# $@ is a special variable: the target of the operation, in this case sample
# $? is the
.o: .cpp header.h
	g++ $(CXXFLAGS) -o $@ $?
objgetacl: objgetacl.o functions.o 
	g++ objgetacl.o functions.o -o $@ $(LDFLAGS)
objput: objput.o functions.o
	g++ objput.o functions.o -o $@ $(LDFLAGS)
objget: objget.o functions.o
	g++ objget.o functions.o -o $@ $(LDFLAGS)
objlist: objlist.o functions.o
	g++ objlist.o functions.o -o $@ $(LDFLAGS)
objsetacl: objsetacl.o functions.o
	g++ objsetacl.o functions.o -o $@ $(LDFLAGS)
objtestacl: objtestacl.o functions.o
	g++ objtestacl.o functions.o -o $@ $(LDFLAGS)
# Before testing, we must compile.  
# Lines preceeded by @ aren't echoed before executing
# Execution will stop if a program has a non-zero return code;
# precede the line with a - to override that
test:	clean build exec
	@echo "------------"
	su u1 -c "./objput -k abc doc < testfile"
	@echo "------------"
	su u1 -c "./objget -k abc doc"
	
	
.PHONY: exec

 exec: clean build
	@chmod 775 ./
	@chmod +x precommands.sh
	@chmod +x readfile.sh
	@./precommands.sh
	@chown ly2331 $(OBJ)
	@chgrp ly2331 $(OBJ)
	@chmod 701 $(OBJ)
	@chmod u+s $(OBJ)
	@touch /home/lanyang/user_object
	@chown ly2331 /home/lanyang/user_object
	@chgrp ly2331 /home/lanyang/user_object
	@chmod 700 /home/lanyang/user_object
ifneq "$(strip $(userfile))" ""
	@echo init to $(userfile)
	@cat $(userfile) > userfile.txt
else
	@echo default_init
	@echo "u1 g1 g3" > userfile.txt
	@echo "u2 g1 g4" >> userfile.txt
endif
	@chown ly2331 userfile.txt
	@chgrp ly2331 userfile.txt
	@chmod 400 userfile.txt
	@./readfile.sh userfile.txt

.PHONY: clean
clean:
	-rm -f $(OBJ) *.core *.o *~ *.*~ .*~ /home/lanyang/*-* /home/lanyang/*_* userfile.txt
	-rmdir /home/lanyang
	-rm /home/objput /home/objget /home/objlist /home/objsetacl /home/objgetacl /home/objtestacl
