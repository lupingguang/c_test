all: monitor test
PHONY : all
monitor: inotify_epoll.c
	gcc -o monitor inotify_epoll.c
test: test.c
	gcc -o test test.c
