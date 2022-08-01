#Flags, Libraries and Includes
include ../../defs.mk

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
	$(CC) $(CFLAGS) $(INCDIR) $< $(LIBDIR) $(LIBS) -lcurses -o $@

.PHONY: directories remake clean cleaner $(BUILDDIR) $(TARGETDIR)