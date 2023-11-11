mkdir -p build
cd build
clang++ ../trace.cpp -c -fPIC -I `llvm-config --includedir` -o trace.o
clang++ trace.o -fPIC -shared -o libtrace.so
clang++ ../../life.cpp -c -o life.o -Xclang -load -Xclang ./libtrace.so -O2 -flegacy-pass-manager
clang++ ../../sim.cpp ../print_instr.cpp life.o -o IR_traces -lSDL2
python3 ../stat.py
mkdir -p images
cp ../*.png images/
rm -rf ../*.png