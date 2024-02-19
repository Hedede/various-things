LD_FLAGS=-fuse-ld=gold -lparser -lfcgi -lfcgi++ 
build:
	g++ -std=c++17 $(LD_FLAGS) -g -O2 main.c++ -o main.cgi
	sudo install -g http -o http main.cgi /srv/http
	#sudo systemctl restart lighttpd
	-rm main.cgi
