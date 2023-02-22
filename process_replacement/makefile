.PHOONY:all
all:mycode printfenv

mycode:mycode.cc
	g++ -o $@ $^
printfenv:printfenv.cc
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -f mycode printfenv
