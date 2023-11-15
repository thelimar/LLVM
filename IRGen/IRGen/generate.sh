mkdir -p build
cd build
clang++ `llvm-config --cppflags --ldflags --libs` -g -O0 ../generator.cpp -lSDL2
./a.out > generated.ll
diff -I -W --width=200 --minimal --color -y generated.ll ../../life.ll > output.txt