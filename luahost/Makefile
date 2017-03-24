src = main.c glue.c tabletest.c
luahost: $(src)
	gcc $(src) -o luahost -llua -lpthread

clean:
	rm luahost
