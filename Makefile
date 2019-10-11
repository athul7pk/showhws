INCLUDES = inc
all:
		gcc src/*.c -o out/showhws -I ${INCLUDES} -g
clean:
		rm -rf out/showhws
