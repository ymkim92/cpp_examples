# install boost

If you see something like this:
```
(base) ykim@ykim-SBT:~/devel/cpp/cpp_examples/boost$ g++ signal_hello.cpp -l boost
signal_hello.cpp:1:10: fatal error: boost/signals2.hpp: No such file or directory
    1 | #include <boost/signals2.hpp>
      |          ^~~~~~~~~~~~~~~~~~~~
```

install boost library in your system:
```
sudo apt install libboost-all-dev 
```