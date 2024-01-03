# This is the plan for the build system revamp

* Firstly create the config data:
  * How to crete such a config?
    - [ ] Libs:
      * Include libs in the cmd for compiler
      * Check all the intricacies of libraries
        * test with libraries that will be needed
    - [ ] Use multiple compilers:
      - [ ] GCC
      - [ ] CLANG
      - [ ] CL
    - [ ] Define formatted print with makefiles
    - [ ] OS detection
    - [ ] Performance profilling
    - [ ] Build:
      - [ ] PCH generation
      - [ ] Int. generation
      - [ ] Dependency check
      - [ ] Wildcards
      - [ ] Multiple includes
      - [ ] Compartimentalize source files by project
    - [ ] Implementing cleaning