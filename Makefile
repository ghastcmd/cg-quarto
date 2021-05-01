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
includes = $(addprefix -I,$(inc_dir)) $(addprefix -L,$(lib_dir))

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

defines = $(addprefix -D,$(def_d))
libs    = $(addprefix -l,$(libs_d))
flags   = 

build: ; $(SS)$(MAKE) -s --no-print-directory -j 4 compile

run: build ; $(SS)$(target)

compile: $(gch) $(target) | $(dep_dir)

$(gch): $(pch)
	$(call fmt,Compiling the precompiled header)
	$(SS)$(CC) -c $< -o $@

$(target): $(object)
	$(call fmt,Compiling $(target))
	$(SS)$(CC) $^ -o $@ $(libs) $(flags)

vpath %.cpp $(src)
$(obj)/%.o: %.cpp $(dep_dir)/%.d
	$(call fmt,Compiling $< into $@)
	$(SS)$(CC) $(make_dep) -c $< -o $@ $(defines) $(includes)

$(depend):
include $(depend)

$(obj): ; $(SS)mkdir $@
	$(call fmt,Creating $@ directory)

$(dep_dir): | $(obj) ; $(SS)mkdir $@
	$(call fmt,Creating $@ directory)

clean: ; $(SS)rm -f $(object) $(target)
	$(call fmt,Cleaning the entire $(obj) folder)

clean_pch: ; $(SS)rm -f $(gch)
	$(call fmt,Deleting the precompiled header)
