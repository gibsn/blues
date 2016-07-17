INCLUDE_DIR = include
LIBRARY_DIR = lib
EXTERNALS_BASEDIR = ../externals

ARGVPARSER_LOCATION = $(EXTERNALS_BASEDIR)/Argvparser
ARGVPARSER_HEADERS = argvparser.h
ARGVPARSER_LIBRARY = libargvparser.a

Argvparser:
	make -C $(ARGVPARSER_LOCATION) -f Makefile
	cp $(ARGVPARSER_LOCATION)/argvparser.h $(INCLUDE_DIR)
	cp $(ARGVPARSER_LOCATION)/$(ARGVPARSER_LIBRARY) $(LIBRARY_DIR)

.PHONY: Argvparser

clean:
	make -C $(ARGVPARSER_LOCATION) -f Makefile $@

.PHONY: clean