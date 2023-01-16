# YASE on Le Potato

To get yase working on Le Potato, do the following. Note that this has only been tested with 

> Armbian_22.11.1_Lepotato_jammy_current_5.19.17

```bash
	git clone https://github.com/klavins/yase.git
	cd yase
	sudo apt install librtmidi-dev
	sudo apt-get install nlohmann-json3-dev
	sudo apt-get install libfftw3-dev
	sudo apt-get install libasound2-dev
	cd potato
	make
	cd BigSound
	make
	./bin/big_sound
```

The Makefile in this directory only includes some yase modules -- basiscally enough to get a simple example working. More modules can be added. The only ones that will definitely not work are audio.cc (which is replaced here with potato_audio.cc) and wavetable.cc. The latter depends on AudioFile.cc which does not seem to have an apt-get package, so will need to be compiled. 
