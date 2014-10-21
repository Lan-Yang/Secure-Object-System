# Makefile

# List the object files in one place
OBJ=objgetacl objput objget objlist objsetacl objtestacl

#options
CXXFLAGS=-c -Wall -Werror -std=c++0x
#LDFLAGS=

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
	g++ objgetacl.o functions.o -o $@
objput: objput.o functions.o
	g++ objput.o functions.o -o $@ 
objget: objget.o functions.o
	g++ objget.o functions.o -o $@
objlist: objlist.o functions.o
	g++ objlist.o functions.o -o $@
objsetacl: objsetacl.o functions.o
	g++ objsetacl.o functions.o -o $@ 
objtestacl: objtestacl.o functions.o
	g++ objtestacl.o functions.o -o $@
# Before testing, we must compile.  
# Lines preceeded by @ aren't echoed before executing
# Execution will stop if a program has a non-zero return code;
# precede the line with a - to override that
test:	build exec
	@echo "------------"
	-su u1 -c "./objput doc < testfile"
	@echo "------------"
	-su u1 -c "./objgetacl doc"
	@echo "------------"
	-su u1 -c "./objsetacl doc < newacl2"
	@echo "------------"
	-su u1 -c "./objgetacl doc"
	@echo "------------"
	-su u1 -c "./objput doc2 < a"
	@echo "------------"
	-su u2 -c "./objput doc < testfile3"
	@echo "------------"
	-su u1 -c "./objget doc"
	@echo "------------"
	-su u1 -c "./objget doc2 | wc -c"
	@echo "------------"
	-su u2 -c "./objget doc"
	@echo "------------"
	-su u2 -c "./objget u1+doc"
	@echo "------------"
	-su u1 -c "./objget do@"
	@echo "------------"
	-su u1 -c "./objlist"
	@echo "------------"
	-su u1 -c "./objlist -l"
	@echo "------------"
	-su u1 -c "./objgetacl doc"
	@echo "------------"
	-su u2 -c "./objgetacl doc"
	@echo "------------"
	-su u2 -c "./objgetacl u1+doc"
	@echo "------------"
	-su u1 -c "./objtestacl -a r doc"
	@echo "------------"
	-su u1 -c "./objtestacl -a x doc"
	@echo "------------"
	-su u2 -c "./objtestacl -a r u1+doc"
	@echo "------------"
	-su u1 -c "./objsetacl doc < newacl"
	@echo "------------"
	-su u1 -c "./objgetacl doc"
	@echo "------------"
	-su u2 -c "./objgetacl u1+doc"
	@echo "------------"
	-su u2 -c "./objsetacl u1+doc < newacl2"
	@echo "------------"
	-su u2 -c "./objgetacl u1+doc"
	@echo "------------"
	-su u1 -c "./objput doc < testfile"
	
.PHONY: exec

 exec: build
	@chmod +x precommands.sh
	@chmod +x readfile.sh
	@./precommands.sh
	@chown ly2331 $(OBJ)
	@chgrp ly2331 $(OBJ)
	@chmod 701 $(OBJ)
	@chmod u+s $(OBJ)
	@touch lanyang/user_object
	@chown ly2331 lanyang/user_object
	@chgrp ly2331 lanyang/user_object
	@chmod 700 lanyang/user_object
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
	rm -f $(OBJ) *.core *.o *~ *.*~ .*~ lanyang/*-* lanyang/*_*
	rmdir lanyang

