# -1. TODOs

- [x] Create function definitions and emission
- [ ] Create containerized (configurable/better iterface) emmit
- [ ] Create Expression emission

# 0. This is the plan for the build system revamp

- [x] OS detection
- [ ] Generate makefile from config
  - [ ] Generate help
  - [ ] Generate clean
- [ ] Build:
  - [ ] PCH generation
  - [ ] Int. generation
  - [ ] Dependency check
  - [ ] Wildcards
  - [ ] Multiple includes
  - [ ] Compartimentalize source files by project
- [ ] Libs:
  - [ ] Include libs in the cmd for compiler
  - [ ] Check all the intricacies of libraries
    - [ ] test with libraries that will be needed
- [ ] Use multiple compilers:
  - [ ] GCC
  - [ ] CLANG
  - [ ] CL
- [ ] Define formatted print with makefiles
- [ ] Performance profilling

# 1. Things inside the makefile

format print
os detection
compiler selection
pch location ::
	generate pch

source detection ::

object translation detection ::

dependency files detection ::
	build and inclued

include directories
lib directories ::
	include lib directive

make dependency string for compiler

declare dependencies related to platform :: (ifeq ($(OS)...)
	target name
	defines
	libs include
	dependency dir

defines declaration
libs declaration

debug flags
optimization flags
dist flags
another flags

activate flags with command

build function
run function
compile function

target generation function
object generation function

dependency generation function

dependency dir generation function

clean objs function
clean pch function
clean all function

# 2. Resources for research to build better the build system

* [Designing a Common Build System](https://alesnosek.com/blog/2018/05/03/designing-a-common-build-system/)
* [13 Things That Makes a Good Build System](https://dzone.com/articles/the-13-things-that-make-a-good-build-system)
* [An overview of build systems (mostly for C++ projects) | by Julien Jorge](https://julienjorge.medium.com/an-overview-of-build-systems-mostly-for-c-projects-ac9931494444)
* [11 Great Tools for a Monorepo in 2021 | Bits and Pieces](https://blog.bitsrc.io/11-tools-to-build-a-monorepo-in-2021-7ce904821cc2)


## 2.0. Comments about given sites

### 2.0.0. Design a Common Build System

### 2.0.1. 13 Things That Makes a Good Build System

### 2.0.2. An overview of build systems (mostly for C++ projects) | by Julien Jorge

### 2.0.3. 11 Great Tools for a Monorepo in 2021 | Bits and Pieces

# 3. References

Nothign