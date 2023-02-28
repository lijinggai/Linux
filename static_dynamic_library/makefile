libmymath.so:mysub.o myadd.o
	gcc -shared -o $@ $^
%.o:%.c
	gcc -fPIC -c $<
.PHONY:clean
clean:
	rm -rf *.o libmymath.so  output_dynamic
.PHONY:output
output:
	mkdir output_dynamic
	cp *.h output_dynamic
	cp libmymath.so output_dynamic













#libmymath.a:mysub.o myadd.o
#	ar -rc $@ $^
#%.o:%.c
#	gcc -c $<
#.PHONY:clean
#clean:
#	rm -rf *.o libmymath.a  output
#.PHONY:output
#output:
#	mkdir output
#	cp *.h output
#	cp libmymath.a output
