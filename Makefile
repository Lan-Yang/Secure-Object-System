# Makefile

# List the object files in one place
OBJ=objgetacl objput objget objlist objsetacl objtestacl

# The first target is the default if you just say "make".  In this
# case, "build" relies on "sample", because I want the executable to be
# called "sample"

build:	$(OBJ)
	@cat /dev/null > user_object

# "sample" requires a set of object files
# $@ is a special variable: the target of the operation, in this case sample
# $? is the
objgetacl: objgetacl.o functions.o
	g++ objgetacl.o functions.o -o objgetacl
objgetacl.o: objgetacl.cpp header.h
	g++ -Wall -c -o objgetacl.o objgetacl.cpp 
objput: objput.o functions.o
	g++ objput.o functions.o -o objput
objput.o: objput.cpp header.h
	g++ -Wall -c -o objput.o objput.cpp 
objget: objget.o functions.o
	g++ objget.o functions.o -o objget
objget.o: objget.cpp header.h
	g++ -Wall -c -o objget.o objget.cpp
objlist: objlist.o functions.o
	g++ objlist.o functions.o -o objlist
objlist.o: objlist.cpp header.h
	g++ -Wall -c -o objlist.o objlist.cpp 
objsetacl: objsetacl.o functions.o
	g++ objsetacl.o functions.o -o objsetacl
objsetacl.o: objsetacl.cpp header.h
	g++ -Wall -c -o objsetacl.o objsetacl.cpp 
objtestacl: objtestacl.o functions.o
	g++ objtestacl.o functions.o -o objtestacl
objtestacl.o: objtestacl.cpp header.h
	g++ -Wall -c -o objtestacl.o objtestacl.cpp 
function.o: functions.cpp header.h
	g++ -Wall -c -o functions.o functions.cpp
# Before testing, we must compile.  
# Lines preceeded by @ aren't echoed before executing
# Execution will stop if a program has a non-zero return code;
# precede the line with a - to override that
test:	build exec
	@echo "------------"
	-./objput -u u1 -g g1 doc < testfile
	@echo "------------"
	-./objput -u u1 -g g3 doc2 < testfile2
	@echo "------------"
	-./objput -u u2 -g g1 doc < testfile3
	@echo "------------"
	-./objget -u u1 -g g1 doc
	@echo "------------"
	-./objget -u u2 -g g1 doc
	@echo "------------"
	-./objget -u u2 -g g1 u1+doc
	@echo "------------"
	-./objget -u u@ -g g1 doc
	@echo "------------"
	-./objget -u u1 -g g@ doc
	@echo "------------"
	-./objget -u u1 -g g@ do@
	@echo "------------"
	-./objlist -u u1
	@echo "------------"
	-./objlist -u u1 -l
	@echo "------------"
	-./objgetacl -u u1 -g g1 doc
	@echo "------------"
	-./objgetacl -u u2 -g g1 doc
	@echo "------------"
	-./objgetacl -u u2 -g g1 u1+doc
	@echo "------------"
	-./objtestacl -u u1 -g g3 -a r doc
	@echo "------------"
	-./objtestacl -u u2 -g g1 -a x doc
	@echo "------------"
	-./objtestacl -u u2 -g g1 -a r u1+doc
	@echo "------------"
	-./objsetacl -u u1 -g g1 doc < newacl
	@echo "------------"
	-./objgetacl -u u1 -g g1 doc
	@echo "------------"
	-./objgetacl -u u2 -g g1 u1+doc
	@echo "------------"
	-./objsetacl -u u2 -g g1 u1+doc < newacl2
	@echo "------------"
	-./objgetacl -u u2 -g g1 u1+doc
	@echo "------------"
	cat u1-doc-acl

.PHONY: exec
 
ifneq "$(strip $(userfile))" ""
exec: build
	@echo init to $(userfile)
	@cat $(userfile) > user_group
else
exec: build
	@echo default_init
	@echo "u1 g1 g3" > user_group
	@echo "u2 g1 g4" >> user_group
endif

.PHONY: clean
clean:
	rm -f $(OBJ) *.core *.o *~ *.*~ .*~ *-*

