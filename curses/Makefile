all: pages

pages: pages.c page.c
	gcc pages.c page.c -g -Wall -Wextra -o pages -lncurses -lpanel

pad: pad.c
	gcc pad.c -g -o pad -lncurses

padscroll: padscroll.c
	gcc padscroll.c -g -o padscroll -lncurses

scroll: scroll.c
	gcc scroll.c -g -o scroll -lncurses

basic: basic.c
	gcc basic.c -g -o basic -lncurses

mouse-keys: mouse-keys.c
	gcc mouse-keys.c -g -o mouse-keys -lcurses

panels: panels.c
	gcc panels.c -g -o panels -lcurses -lpanel

box: box.c
	gcc box.c -g -o box -lcurses
