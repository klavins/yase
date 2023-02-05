include potato_defs.mk

SOURCES     := module.cc container.cc sine.cc oscillator.cc event_manager.cc biquad.cc saw.cc anti_alias.cc envelope.cc player.cc gain.cc transform.cc echo.cc iir_filter.cc delay.cc fadable_delay.cc sum.cc

OBJECTS     := $(patsubst %.cc, $(BUILDDIR)/%.o, $(notdir $(SOURCES)))

all: $(TARGETDIR)/$(TARGET)

potato_audio.o: potato_audio.cc potato_audio.hh
	$(CC) $(CFLAGS) $(INCDIR) -c -fPIC -o $(BUILDDIR)/potato_audio.o potato_audio.cc

$(TARGETDIR)/$(TARGET): $(OBJECTS) potato_audio.o
	mkdir -p lib
	ar rvs $@ $(OBJECTS) build/potato_audio.o

#Compile
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	mkdir -p build
	$(CC) $(CFLAGS) $(INCDIR) -c -fPIC -o $@ $<

