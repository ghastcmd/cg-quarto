src = src
obj = bin
dep_dir := $(obj)/deps

target = bin/binary
SS = @

define fmt
	$(SS)echo 	$(1)
endef

ifeq ($(OS),Windows_NT)
	dos = Windows
else
	dos = $(shell uname -s)
endif

CC = g++ -std=c++2a

pch = include/pch.h
gch = $(pch:.h=.h.gch)

source = $(wildcard $(src)/*.cpp)
object = $(patsubst %,$(obj)/%.o,$(basename $(notdir $(source))))
depend = $(patsubst %,$(dep_dir)/%.d,$(basename $(notdir $(source))))

inc_dir  = include
lib_dir  = libs
include_lib = $(addprefix -L,$(lib_dir))
includes = $(addprefix -I,$(inc_dir))

make_dep = -MMD -MT $@ -MP -MF $(dep_dir)/$*.d

ifeq ($(dos),Windows)
	target  := $(target).exe
	def_d    = FREEGLUT_STATIC
	libs_d   = freeglut_static glu32 gdi32 opengl32 winmm
	dep_dir := $(subst /,\,$(dep_dir))
else
ifeq ($(dos),Linux)
	def_d  = 
	libs_d = GL GLU glut
endif
endif

defines    = $(addprefix -D,$(def_d))
libs       = $(addprefix -l,$(libs_d))
# re add -Wall -Wextra -Werror to flags
flags      = -g
dist_flags = -static-libgcc -static-libstdc++
opt_flags  = -O3

test_flags = -DREADER_TEST

test:
	$(eval flags:=$(flags) $(test_flags))
	$(call fmt,Added flags: $(test_flags))

optimize:
	$(eval flags:=$(flags) $(opt_flags))
	$(call fmt,Added flags: $(opt_flags))

dist:
	$(eval flags:=$(flags) $(dist_flags))
	$(call fmt,Added flags: $(dist_flags))

build: ; $(SS)$(MAKE) flags="$(flags)" -s --no-print-directory -j compile

run: build ; $(SS)$(target)

compile: $(dep_dir) $(gch) $(target)

$(gch): $(pch)
	$(call fmt,Compiling the precompiled header)
	$(SS)$(CC) -c $< -o $@ $(includes) $(flags)

$(target): $(object)
	$(call fmt,Compiling $(target))
	$(SS)$(CC) $^ -o $@ $(include_lib) $(libs) $(flags)

vpath %.cpp $(src)
$(obj)/%.o: %.cpp $(dep_dir)/%.d
	$(call fmt,Compiling $< into $@)
	$(SS)$(CC) $(make_dep) -c $< -o $@ $(defines) $(includes) $(flags)

$(depend):
include $(depend)

$(obj): $(obj)
	$(call fmt,Creating $@ directory)
	$(SS)mkdir $@

$(dep_dir):
	$(call fmt,Creating $@ directory)
ifeq ($(dos),Windows)
	$(SS)mkdir $@
else
	$(SS)mkdir -p $@
endif

clean:
ifeq ($(dos),Windows)
	-$(SS)del /f /q $(subst /,\,$(object) $(target))
else
	$(SS)rm -f $(object) $(target)
endif
	$(call fmt,Cleaning the entire $(obj) folder)

clean_pch:
ifeq ($(dos),Windows)
	-$(SS)del /f /q $(subst /,\,$(gch))
else
	$(SS)rm -f $(gch)
endif
	$(call fmt,Deleting the precompiled header)
