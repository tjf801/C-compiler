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

assert 0 0
assert 42 42
assert 8 '3+5'
assert 3 '7-4'
assert 21 '5+20-4'
assert 41 ' 12 + 34 - 5 '
assert 32 '2*16'
assert 60 '2*2*3*5'
assert 5 '32/6'
assert 47 '5+6*7'
assert 15 '5*(9-6)'
assert 4 '(3+5)/2'
assert 10 '-10+20'
assert 10 '- -10'
assert 10 '- - +10'

echo Compiler Passed All Tests!