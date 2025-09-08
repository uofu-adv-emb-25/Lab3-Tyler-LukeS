# CMake generated Testfile for 
# Source directory: /home/tyler/ece6785/labs/lab0/test
# Build directory: /home/tyler/ece6785/labs/lab0/build/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(simulate_mytest "RENODE-NOTFOUND" "--disable-xwt" "--port" "-2" "--pid-file" "renode.pid" "--console" "-e" "\$ELF=@/home/tyler/ece6785/labs/lab0/build/test/mytest.elf; \$WORKING=@/home/tyler/ece6785/labs/lab0; include @/home/tyler/ece6785/labs/lab0/test/simulate.resc; start")
set_tests_properties(simulate_mytest PROPERTIES  _BACKTRACE_TRIPLES "/home/tyler/ece6785/labs/lab0/test/CMakeLists.txt;39;add_test;/home/tyler/ece6785/labs/lab0/test/CMakeLists.txt;0;")
