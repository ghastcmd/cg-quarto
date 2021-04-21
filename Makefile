src = src
obj = bin

target = bin/binary
SS = @

define fmt
	$(SS)echo 	$(1)
endef

ifeq ($(OS),Windows_NT)
	target:=$(target).exe
endif

CC = g++

pch = src/include/pch.h
gch = $(pch:.h=.h.gch)

source = $(wildcard $(src)/*.cpp)
object = $(patsubst %,$(obj)/%.o,$(basename $(notdir $(source))))

inc_dir = src/include vendor/glut
includes = $(addprefix -I,$(inc_dir))

flags = -Isrc/include

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
	$(SS)$(CC) $^ -o $@ $(flags) $(includes)

vpath %.cpp $(src)
bin/%.o: %.cpp
	$(call fmt,Compiling $< into $@)
	$(SS)$(CC) -c $< -o $@ $(includes)

bin:
	$(call fmt,Creating $(obj) directory)
	$(SS)mkdir $(obj)

clean:
	$(call fmt,Cleaning the entire $(obj) folder)
	$(SS)rm -Force $(wildcard $(obj)/*)

clean_pch:
	$(call fmt,Deleting the precompiled header)
	$(SS)rm -f $(gch)