include defs.mk

#Files
DGENCONFIG  := docs.config

SOURCES     := $(wildcard $(SRCDIR)/*.cc) 
OBJECTS     := $(patsubst %.cc, $(BUILDDIR)/%.o, $(notdir $(SOURCES)))

#Defauilt Make
all: directories $(TARGETDIR)/$(TARGET) examples

#Remake
remake: cleaner all

#Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

tests:
	cd test && $(MAKE) 

examples: $(EXAMPLE_DIRS)

example: $(TARGETDIR)/$(TARGET) 
	$(MAKE) -C $(ROOT_DIR)/examples/$(name) 

$(EXAMPLE_DIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

docs: docs/index.html tutorials/intro.md

docs/index.html: $(SOURCES) README.md docs.config examples/*/*.cc
	$(DGEN) $(DGENCONFIG)

#Clean only Objects
clean: $(EXAMPLE_DIRS)
	@$(RM) -rf $(BUILDDIR)/*.o $(TARGETDIR)/$(TARGET)
	cd test && $(MAKE) clean

wc:
	wc -l */*.cc */*.hh */*/*.cc */*/*.hh | sort

#Link
$(TARGETDIR)/$(TARGET): $(OBJECTS) 
	/usr/bin/ar rvs $@ $(OBJECTS) $(SYNOBJECTS)
	
#Compile
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) 
	$(CC) $(CFLAGS) $(INCDIR) -c -fPIC -o $@ $<

.PHONY: $(EXAMPLE_DIRS) 
