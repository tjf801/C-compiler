#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  ./ccompiler.exe "$input" > tmp.s || exit
  gcc -static -o tmp.exe tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    : #echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 '{return 0;}'
assert 42 '{return 42;}'
assert 8 '{return 3+5;}'
assert 3 '{return 7-4;}'
assert 21 '{return 5+20-4;}'
assert 41 '{return 12 + 34 - 5 ;}'
assert 32 '{return 2*16;}'
assert 60 '{return 2*2*3*5;}'
assert 5 '{return 32/6;}'
assert 47 '{return 5+6*7;}'
assert 15 '{return 5*(9-6);}'
assert 4 '{return (3+5)/2;}'
assert 10 '{return -10+20;}'
assert 10 '{return - -10;}'
assert 10 '{return - - +10;}'

assert 0 '{return 0==1;}'
assert 1 '{return 42==42;}'
assert 1 '{return 0!=1;}'
assert 0 '{return 42!=42;}'

assert 1 '{return 0<1;}'
assert 0 '{return 1<1;}'
assert 0 '{return 2<1;}'
assert 1 '{return 0<=1;}'
assert 1 '{return 1<=1;}'
assert 0 '{return 2<=1;}'

assert 1 '{return 1>0;}'
assert 0 '{return 1>1;}'
assert 0 '{return 1>2;}'
assert 1 '{return 1>=0;}'
assert 1 '{return 1>=1;}'
assert 0 '{return 1>=2;}'

assert 3 '{a = 3; return a;}'
assert 5 '{a = 4; a = 2; return a+3;}'
assert 4 '{a = 1; a = a + 3; return a;}'
assert 6 '{a = b = 3; return a + b;}'
assert 12 '{a = 4; z = 3; return a*z;}'

echo Compiler Passed All Tests!