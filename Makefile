
winappid:	winappid.o
	cc -o $@ $^ -L/usr/lib/w32api -lole32 -luuid

%.o:	%.c
	cc -c $^

