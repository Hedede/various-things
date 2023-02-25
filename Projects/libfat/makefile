#AWLIB_PATH=/home/shared/devel/Projects/awlib-u

#AWLIB_INC=-I$(AWLIB_PATH)/include
OBJECTS = source/directory_iterator.o source/filesystem.o source/filesystem_impl.o source/entry.o
LIBS = -lawio -lstdc++fs
%.o: %.c++
	g++ -fPIC -I./include  -g -std=c++17 -c $< -o $@
libfat.so: $(OBJECTS)
	g++ -fPIC -g -std=c++17 $^ $(LIBS) -shared -o $@
clean:
	find -name '*.o' -exec rm '{}' ';'
