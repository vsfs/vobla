Vobla: Vsfs's Open-source Base LibrAry.
=====

[![Build Status](https://travis-ci.org/vsfs/vobla.png?branch=master)](https://travis-ci.org/vsfs/vobla) [![Coverage Status](https://coveralls.io/repos/vsfs/vobla/badge.png?branch=master)](https://coveralls.io/r/vsfs/vobla)

Vsfs Open-sourced Base C++ Library

## Dependencies

 - g++ 4.7
 - autoconf
 - automake
 - libtool
 - boost >= 1.48
 - glog
 - gtest 1.6
   - Download gtest 1.6.0 from
     http://googletest.googlecode.com/files/gtest-1.6.0.zip and unzip it to `test/` directory.
 - libssl-dev

### Install dependencies on specific platform

```sh
# Ubuntu 12.10
sudo apt-get install g++ autoconf automake libtool libboost1.50-all-dev libssl-dev libgoogle-glog-dev
```

### Compile

```sh
$ git clone git://github.com/vsfs/vobla.git
$ cd vobla
# Download gtest
$ cd test
$ wget http://googletest.googlecode.com/files/gtest-1.6.0.zip
$ unzip gtest-*.zip
$ cd gtest-*
$ ./configure && make

$ cd ../..
$ ./bootstrap
$ ./configure
$ make
# Run unit tests
$ make check
```
