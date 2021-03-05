
leonardo:
	cp -r src/leonardo-firm .
	cp src/chef/chef.h leonardo-firm/chef.h
	cp src/chef/chef.c leonardo-firm/chef.cpp
	tar -cf leonardo-firm.tar leonardo-firm
	rm -r leonardo-firm

clean:
	rm leonardo-firm.tar
