#Architecture
ARCH := $(shell uname -m)

#Compilers
CC          := g++ -std=c++20 -Wno-psabi

#Directories
YASE        := ../..
JSON        := ../../../json
MIDI        := ../../../rtmidi

#The Directories, Source, Includes, Objects, Binary and Resources
SRCEXT      := cc

#Flags, Libraries and Includes
CFLAGS      := -ggdb
LIB         := -lpthread -lyase -lportaudio -lrtmidi
INCLUDE		:= -I$(YASE)/include -I$(JSON)/include -I$(JSON)/include/nlohmann
LIBDIR		:= -L$(YASE)/lib -L$(MIDI)
ELMALIB		:= $(YASE)/lib/libyase.a

#Files
TARGETDIR	 := ./bin
SOURCES      := $(wildcard *.cc)
TARGETS		 := $(patsubst %.cc,%,$(wildcard *.cc))
FULL_TARGETS := $(addprefix $(TARGETDIR)/, $(TARGETS))

#Default Make
all: dirs $(FULL_TARGETS)

dirs: $(TARGETDIR)
	@mkdir -p $(TARGETDIR)

#Clean only Objects
clean:
	@$(RM) -rf $(TARGETDIR)

# Compile
$(TARGETDIR)/%: %.cc
	$(CC) $(CFLAGS) $(INCLUDE) $< $(LIBDIR) $(LIB) -o $@

.PHONY: directories remake clean cleaner $(BUILDDIR) $(TARGETDIR)