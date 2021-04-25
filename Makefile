src = src
obj = bin

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

ifeq ($(dos),Windows)
	target:=$(target).exe
endif

CC = g++ -std=c++2a

pch = src/include/pch.h
gch = $(pch:.h=.h.gch)

source = $(wildcard $(src)/*.cpp)
object = $(patsubst %,$(obj)/%.o,$(basename $(notdir $(source))))

inc_dir = src/include
lib_dir = libs
includes = $(addprefix -I,$(inc_dir)) $(addprefix -L,$(lib_dir))

ifeq ($(dos),Windows)
	def_d = FREEGLUT_STATIC
else
ifeq ($(dos),linux)
	def_d = 
endif
endif

defines = $(addprefix -D,$(def_d))
flags = 

ifeq ($(dos),Windows_NT)
	libs_d = freeglut_static glu32 gdi32 opengl32 winmm
else
ifeq ($(dos),Linux)
	libs_d = GL GLU glut
endif
endif

libs = $(addprefix -l,$(libs_d))

build:
	$(SS)$(MAKE) -s --no-print-directory -j 4 compile

run: build
	$(SS)$(target)

compile: bin $(gch) $(target)

$(gch): $(pch)
	$(call fmt,Compiling the precompiled header)
	$(SS)$(CC) -c $< -o $@

$(target): $(object)
	$(call fmt,Compiling $(target))
	$(SS)$(CC) $^ -o $@ $(includes) $(libs) $(flags)

vpath %.cpp $(src)
bin/%.o: %.cpp
	$(call fmt,Compiling $< into $@)
	$(SS)$(CC) -c $< -o $@ $(defines) $(includes) $(libs)

bin:
	$(call fmt,Creating $(obj) directory)
	$(SS)mkdir $(obj)

clean:
	$(call fmt,Cleaning the entire $(obj) folder)
	$(SS)rm -f $(wildcard $(obj)/*)

clean_pch:
	$(call fmt,Deleting the precompiled header)
	$(SS)rm -f $(gch)
