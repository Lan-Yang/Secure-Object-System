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
	-su u2 -c "./objput -k 123 u1+doc < testfile"
	-su u2 -c "./objput -k 123 u1+doc2 < testfile"
	@echo "------------"
	su u1 -c "./objgetacl doc"
	@echo "------------"
	su u1 -c "./objsetacl doc < newacl2"
	-su u1 -c "./objsetacl doc < erroracl1"
	-su u1 -c "./objsetacl doc < erroracl2"
	-su u1 -c "./objsetacl doc < erroracl3"
	-su u1 -c "./objsetacl doc < erroracl4"
	-su u1 -c "./objsetacl doc < erroracl5"
	@echo "------------"
	su u1 -c "./objgetacl doc"
	@echo "------------"
	su u1 -c "./objput -k abc bin < binfile"
	su u1 -c "./objget bin | wc -c"
	cat binfile | wc -c
	@echo "------------"
	su u1 -c "./objput -k abc bigbin < bigbinfile"
	su u1 -c "./objget bigbin | wc -c"
	cat bigbinfile | wc -c
	@echo "------------"
	su u2 -c "./objput -k ABC doc < testfile3"
	su u2 -c "./objget doc"
	@echo "------------"
	-su u2 -c "./objget u1+doc"
	@echo "------------"
	-su u1 -c "./objget do@"
	@echo "------------"
	su u1 -c "./objlist"
	@echo "------------"
	su u1 -c "./objlist -l"
	@echo "------------"
	-su u2 -c "./objgetacl u1+doc"
	@echo "------------"
	su u1 -c "./objtestacl -a r doc"
	@echo "------------"
	su u1 -c "./objtestacl -a x doc"
	@echo "------------"
	su u2 -c "./objtestacl -a r u1+doc"
	@echo "------------"
	su u1 -c "./objsetacl doc < newacl"
	su u1 -c "./objgetacl doc"
	@echo "------------"
	-su u2 -c "./objgetacl u1+doc"
	@echo "------------"
	-su u2 -c "./objsetacl u1+doc < newacl2"
	@echo "------------"
	su u2 -c "./objtestacl -a r u1+doc"
	-su u2 -c "./objtestacl -a p u1+doc"
	@echo "------------"
	-su u1 -c "./objtestacl -a w doc"
	-su u1 -c "./objput -k abc doc < testfile"
	
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
