include ../../defs.mk

#The Target Binary Program
EX_TARGET      := monophonic

#The Directories, Source, Includes, Objects, Binary and Resources
EX_SRCDIR      := .
EX_BUILDDIR    := .
EX_TARGETDIR   := ./bin

#Flags, Libraries and Includes
INC         := $(INCDIR) 

#Files
SOURCES     := $(wildcard $(EX_SRCDIR)/*.cc) 
OBJECTS     := $(patsubst %.cc, $(EX_BUILDDIR)/%.o, $(notdir $(SOURCES)))

#Defauilt Make
all: directories $(EX_TARGETDIR)/$(EX_TARGET) 

#Remake
remake: cleaner all

#Make the Directories
directories:
	@mkdir -p $(EX_TARGETDIR)

#Clean only Objects
clean:
	@$(RM) -rf $(EX_BUILDDIR)/*.o $(EX_TARGETDIR)/$(EX_TARGET)

#Link
$(EX_TARGETDIR)/$(EX_TARGET): $(OBJECTS)
	$(CC) $(LIBS) $(LIBDIR) $(OBJECTS) -o $@
	
#Compile
$(EX_BUILDDIR)/%.o: $(EX_SRCDIR)/%.$(SRCEXT) 
	$(CC) $(CFLAGS) $(INC) -c -fPIC -o $@ $<
