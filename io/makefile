.PHONY:all
all:mycode mybuffer

mycode:mycode.cc
	g++ -o $@ $^
mybuffer:mybuffer.cc
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -f mycode
