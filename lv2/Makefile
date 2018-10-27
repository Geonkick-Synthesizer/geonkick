DEFINES = -DFGEON_LOG_LEVEL_TRACE -DFGEON_LOG_FUNCTION
BUNDLE = geonkick.lv2
INSTALL_DIR = /usr/local/lib/lv2
TARGET =

$(BUNDLE): manifest.ttl geonkick.so
	rm -rf $(BUNDLE)
	mkdir $(BUNDLE)
	cp manifest.ttl geonkick.ttl geonkick.so $(BUNDLE)

geonkick.so: geonkick.c
	gcc -Wall -shared -fPIC -DPIC $(DEFINES) geonkick.c -I/usr/include/ -o geonkick.so

install: $(BUNDLE)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	cp -R $(BUNDLE) $(INSTALL_DIR)

uninstall:
	rm -fr /usr/local/lib/lv2/$(BUNDLE)

clean:
	rm -rf $(BUNDLE) geonkick.so
	rm *~
