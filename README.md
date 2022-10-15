# my simple concolic execution demo

this is a concolic executor on minimal C, currently only support:

- [x] int (with no bound)
- [x] bool
- [x] + operator of int
- [x] == operator of int

this project is a (bad and minimal) re-implementation in C++ of the [cs6.858's symbolic lab](https://github.com/lxs137/MIT6.858/tree/master/lab3)

## try it!
```
git clone https://github.com/chujDK/concolic-cpp.git
cd concolic-cpp && mkdir build && cd build
cmake ..
make
./concolic-cpp-test
```

this will concolic execute this function

```c
int testf(int a, int b) {
  if (a == b) {
    return a + b;
  } else {
    if (a == 1) {
      return a + 4;
    } else {
      return b;
    }
  }
}
```

and produce out put like

```
[+] returned:  {symbolic: (+ a b); concrete: 0} 
explored:  [(= a b)]
 
[+] setting  a  =  1 
[+] setting  b  =  0 
[+] returned:  {symbolic: (+ a 4); concrete: 5} 
explored:  [(not (= a b)),(= a 1)]
 
[+] setting  b  =  0 
[+] setting  a  =  0 
[+] returned:  {symbolic: (+ a b); concrete: 0} 
explored:  [(= a b)]
 
[+] setting  a  =  2 
[+] setting  b  =  3 
[+] returned:  {symbolic: b; concrete: 3} 
explored:  [(not (= a b)),(not (= a 1))]
 
[+] setting  b  =  2 
[+] setting  a  =  1 
[+] returned:  {symbolic: (+ a 4); concrete: 5} 
explored:  [(not (= a b)),(= a 1)]
```

you can see it explored all the paths