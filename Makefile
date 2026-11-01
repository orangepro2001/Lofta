# All compiling happens on egg: tools/egg.sh syncs this tree there and runs these targets.
CC      ?= cc
MINGW   := x86_64-w64-mingw32-gcc
WINDRES := x86_64-w64-mingw32-windres
AR_WIN  := x86_64-w64-mingw32-ar

LUA_DIR  := third_party/lua-5.4.9/src
RL_DIR   := third_party/raylib-6.2
MONO_DIR := third_party/monocypher-4.0.2
FONT_BDF := third_party/fusion-pixel/fusion-pixel-12px-proportional-zh_hans.bdf

CFLAGS   := -std=c99 -Wall -Wextra -Werror -Wvla -O2
INCLUDES := -Isrc/core -Isrc/platform
HOST_INC := $(INCLUDES) -I$(LUA_DIR)
SAN      := -g -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer

# RELEASE=1 stamps the VERSION file into the program; otherwise it is a dev build.
VERSION_NAME := $(shell tr -d ' \r\n' < VERSION)
VERSION_CODE := $(shell tr -d ' \r\n' < VERSION | awk -F. '{print $$1*10000+$$2*100+$$3}')
VERSION_PARTS := $(subst ., ,$(VERSION_NAME))
ifeq ($(RELEASE),1)
VERSION_DEFS := -DLOFTA_VERSION_NAME='"$(VERSION_NAME)"' -DLOFTA_VERSION_CODE=$(VERSION_CODE)
WIN_DIR      ?= build/release
else
VERSION_DEFS :=
WIN_DIR      := build/dev
endif

GEN_DIR     := build/gen
CONTENT_LUA := $(GEN_DIR)/content_data.lua
FONT_DATA   := $(GEN_DIR)/font_data.c
EMBED       := $(GEN_DIR)/scripts_embed.c

CONTENT_SRC := $(shell find content -type f | LC_ALL=C sort)
SCRIPTS_LUA := $(shell find scripts -name '*.lua' | LC_ALL=C sort)
CORE_SRC    := $(wildcard src/core/*.c)
HOST_SRC    := src/platform/lua_host.c src/platform/lua_bind.c src/platform/savefile.c
HEADLESS_SRC:= $(wildcard src/platform/headless/*.c)
RAYLIB_SRC  := $(wildcard src/platform/raylib/*.c)
HEADERS     := $(wildcard src/core/*.h src/platform/*.h src/platform/*/*.h src/update/*.h)
SCENARIOS   := $(wildcard tests/scripts/*.lua)
DEMOS       := $(wildcard tests/demo/*.lua)
TEST_LUA    := $(wildcard tests/lib/*.lua) $(SCENARIOS) $(DEMOS)
LUA_SRC     := $(filter-out $(LUA_DIR)/lua.c $(LUA_DIR)/luac.c,$(wildcard $(LUA_DIR)/*.c))
RELHUB_SRC  := src/update/relhub.c $(MONO_DIR)/monocypher.c $(MONO_DIR)/monocypher-ed25519.c
UPDATE_INC  := -Isrc/update -I$(MONO_DIR)

.PHONY: all win update-client test shots check-layers check-scripts story-check clean

all: test win

$(CONTENT_LUA): $(CONTENT_SRC) tools/storyc.py
	@mkdir -p $(GEN_DIR)
	python3 tools/storyc.py content $@

$(FONT_DATA): $(CONTENT_LUA) $(SCRIPTS_LUA) tools/gen_font.py $(FONT_BDF)
	python3 tools/gen_font.py --bdf $(FONT_BDF) --out $@ $(CONTENT_LUA) $(SCRIPTS_LUA)

$(EMBED): tools/embed_scripts.py $(SCRIPTS_LUA) $(CONTENT_LUA)
	python3 tools/embed_scripts.py $@ scripts $(GEN_DIR) $(SCRIPTS_LUA) $(CONTENT_LUA)

build/san_lua.a: $(LUA_SRC)
	rm -rf build/san_lua && mkdir -p build/san_lua
	cd build/san_lua && for f in $(LUA_SRC); do $(CC) -std=gnu99 -O1 -w $(SAN) -DLUA_USE_POSIX -c ../../$$f || exit 1; done
	ar rcs $@ build/san_lua/*.o

build/win_lua.a: $(LUA_SRC)
	rm -rf build/win_lua && mkdir -p build/win_lua
	cd build/win_lua && for f in $(LUA_SRC); do $(MINGW) -std=gnu99 -O2 -w -c ../../$$f || exit 1; done
	$(AR_WIN) rcs $@ build/win_lua/*.o

build/lofta.ico: tools/gen_icon.py
	@mkdir -p build
	python3 tools/gen_icon.py $@

$(WIN_DIR)/lofta_res.o: src/platform/raylib/lofta.rc src/platform/raylib/lofta.manifest build/lofta.ico VERSION
	@mkdir -p $(WIN_DIR)
	$(WINDRES) -I src/platform/raylib -DVER_MAJOR=$(word 1,$(VERSION_PARTS)) -DVER_MINOR=$(word 2,$(VERSION_PARTS)) \
		-DVER_PATCH=$(word 3,$(VERSION_PARTS)) -DVER_TEXT='\"$(VERSION_NAME)\"' $< -O coff -o $@

win: $(WIN_DIR)/Lofta.exe

$(WIN_DIR)/Lofta.exe: $(CORE_SRC) $(RAYLIB_SRC) $(HOST_SRC) $(HEADERS) $(EMBED) $(FONT_DATA) src/update/update_win.c $(RELHUB_SRC) $(WIN_DIR)/lofta_res.o build/win_lua.a VERSION
	@mkdir -p $(WIN_DIR)
	$(MINGW) $(CFLAGS) $(VERSION_DEFS) $(HOST_INC) $(UPDATE_INC) -Isrc/platform/raylib -I$(RL_DIR)/include -mwindows -static-libgcc \
		$(CORE_SRC) $(FONT_DATA) $(RAYLIB_SRC) $(HOST_SRC) $(EMBED) src/update/update_win.c src/update/relhub.c $(WIN_DIR)/lofta_res.o build/win_lua.a \
		build/win_mono.a -L$(RL_DIR)/lib -lraylib -lopengl32 -lgdi32 -lwinmm -lwinhttp -o $@

# Monocypher builds separately because its own code is not held to our warning flags.
build/win_mono.a: $(MONO_DIR)/monocypher.c $(MONO_DIR)/monocypher-ed25519.c
	rm -rf build/win_mono && mkdir -p build/win_mono
	cd build/win_mono && $(MINGW) -std=c99 -O2 -w -I../../$(MONO_DIR) -c ../../$(MONO_DIR)/monocypher.c ../../$(MONO_DIR)/monocypher-ed25519.c
	$(AR_WIN) rcs $@ build/win_mono/*.o

$(WIN_DIR)/Lofta.exe: build/win_mono.a

$(WIN_DIR)/update_client.exe: tests/update_client.c src/update/update_win.c src/update/relhub.c $(HEADERS) build/win_mono.a
	@mkdir -p $(WIN_DIR)
	$(MINGW) $(CFLAGS) $(VERSION_DEFS) -Isrc/platform $(UPDATE_INC) -static-libgcc tests/update_client.c src/update/update_win.c src/update/relhub.c \
		build/win_mono.a -lwinhttp -o $@

update-client: $(WIN_DIR)/update_client.exe

build/lofta_headless: $(CORE_SRC) $(HEADLESS_SRC) $(HOST_SRC) $(HEADERS) $(EMBED) $(FONT_DATA) build/san_lua.a
	$(CC) $(CFLAGS) $(SAN) $(HOST_INC) -Isrc/platform/headless $(CORE_SRC) $(FONT_DATA) $(HEADLESS_SRC) $(HOST_SRC) $(EMBED) build/san_lua.a -lm -o $@

build/lofta_headless.exe: $(CORE_SRC) $(HEADLESS_SRC) $(HOST_SRC) $(HEADERS) $(EMBED) $(FONT_DATA) build/win_lua.a
	$(MINGW) $(CFLAGS) $(HOST_INC) -Isrc/platform/headless -static-libgcc $(CORE_SRC) $(FONT_DATA) $(HEADLESS_SRC) $(HOST_SRC) $(EMBED) build/win_lua.a -o $@

build/unit: tests/unit.c $(CORE_SRC) $(HEADERS) $(FONT_DATA)
	$(CC) $(CFLAGS) $(SAN) $(INCLUDES) tests/unit.c $(CORE_SRC) $(FONT_DATA) -o $@

build/san_mono.a: $(MONO_DIR)/monocypher.c $(MONO_DIR)/monocypher-ed25519.c
	rm -rf build/san_mono && mkdir -p build/san_mono
	cd build/san_mono && $(CC) -std=c99 -O1 -w $(SAN) -I../../$(MONO_DIR) -c ../../$(MONO_DIR)/monocypher.c ../../$(MONO_DIR)/monocypher-ed25519.c
	ar rcs $@ build/san_mono/*.o

build/unit_update: tests/unit_update.c src/update/relhub.c src/update/relhub.h build/san_mono.a
	$(CC) $(CFLAGS) $(SAN) $(UPDATE_INC) tests/unit_update.c src/update/relhub.c build/san_mono.a -o $@

check-layers:
	sh tools/check_layers.sh

check-scripts:
	python3 tools/check_scripts.py $(SCRIPTS_LUA) $(TEST_LUA)

test: check-layers check-scripts build/unit build/unit_update build/lofta_headless
	build/unit
	build/unit_update tests/fixtures
	@for s in $(SCENARIOS); do build/lofta_headless $$s build/test_shots || exit 1; done

shots: build/lofta_headless
	mkdir -p build/shots
	@for s in $(DEMOS); do build/lofta_headless $$s build/shots || exit 1; done
	python3 tests/check_png.py build/shots/*.png

clean:
	rm -rf build
