if [ -f "a.out" ]; then
    rm a.out
fi

g++ $1 -I/usr/local/include -L/usr/local/lib -ladios2_cxx11
if [ -f "a.out" ]; then
    ./a.out
    rm a.out
fi