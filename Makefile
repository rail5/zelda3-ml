TARGET_EXEC:=zelda3
ROM:=tables/zelda3.sfc

SRCS:=$(wildcard src/*.c snes/*.c) third_party/gl_core/gl_core_3_1.c third_party/opus-1.3.1-stripped/opus_decoder_amalgam.c
EXTRA_SRCS:=$(wildcard src/ext/*.cpp)

OBJS:=$(SRCS:%.c=%.o)
EXTRA_OBJS:=$(EXTRA_SRCS:%.cpp=%.opp) # .opp for C++ object files to distinguish them from .o C object files

PYTHON:=/usr/bin/env python3

CFLAGS:=$(if $(CFLAGS),$(CFLAGS),-O2 -Werror) -I . -MMD -MP
CFLAGS:=${CFLAGS} $(shell sdl2-config --cflags) -DSYSTEM_VOLUME_MIXER_AVAILABLE=0

IMGUI_IFLAGS:=$(shell pkg-config --cflags imgui)
IMGUI_LDFLAGS:=$(shell pkg-config --libs imgui)

CXXFLAGS:=$(CFLAGS) $(IMGUI_IFLAGS) -std=c++23 -O2 -s -Wall -Wextra

LDFLAGS:=$(IMGUI_LDFLAGS)

CC:=gcc
CXX:=g++

ifeq (${OS},Windows_NT)
    WINDRES:=windres
    RES:=zelda3.res
    SDLFLAGS:=-Wl,-Bstatic $(shell sdl2-config --static-libs)
else
    SDLFLAGS:=$(shell sdl2-config --libs) -lm
endif

.PHONY: all clean clean_obj clean_gen

all: $(TARGET_EXEC) zelda3_assets.dat

$(TARGET_EXEC): $(OBJS) $(EXTRA_OBJS) $(RES)
	$(CXX) $^ -o $@ $(LDFLAGS) $(SDLFLAGS)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.opp : %.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $< -o $@

venv:
	@$(PYTHON) -m venv venv
	@./venv/bin/python3 -m pip install -r requirements.txt


$(RES): src/platform/win32/zelda3.rc
	@echo "Generating Windows resources"
	@$(WINDRES) $< -O coff -o $@

zelda3_assets.dat: venv
	@echo "Extracting game resources"
	$(PYTHON) assets/restool.py --extract-from-rom


clean: clean_obj clean_gen clean_venv

clean_venv:
	@rm -rf venv

clean_obj:
	@$(RM) $(OBJS) $(TARGET_EXEC)

clean_gen:
	@$(RM) $(RES) zelda3_assets.dat tables/zelda3_assets.dat tables/*.txt tables/*.png tables/sprites/*.png tables/*.yaml
	@rm -rf tables/__pycache__ tables/dungeon tables/img tables/overworld tables/sound

ifeq ($(filter clean%,$(MAKECMDGOALS)),)
-include $(shell find . -name '*.d' 2>/dev/null)
endif
