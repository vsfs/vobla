Vobla: Vsfs's Open-source Base LibrAry.
=====

[![Build Status](https://travis-ci.org/vsfs/vobla.png?branch=master)](https://travis-ci.org/vsfs/vobla) [![Coverage Status](https://coveralls.io/repos/vsfs/vobla/badge.png?branch=master)](https://coveralls.io/r/vsfs/vobla)

Vsfs Open-sourced Base C++ Library

## Dependencies

 - g++ 4.7
 - autoconf
 - automake
 - autoconf-archive
 - libtool
 - boost > 1.48
   - boost_filesystem
   - boost_system
 - glog
 - libssl-dev
 - gmock 1.7.0 (Optional for test)
   - Download gmock 1.7.0 from
     http://googlemock.googlecode.com/files/gmock-1.7.0.zip and unzip it to `test/` directory.

### Install dependencies on specific platform

~~~~~~~~~~~~~~{.sh}
# Ubuntu 13.04
sudo apt-get install g++ autoconf automake libtool libssl-dev libgoogle-glog-dev libboost-filesystem1.53-dev libboost-system1.53-dev libboost1.53-dev
~~~~~~~~~~~~~~

### Compile

~~~~~~~~~~~~~~{.sh}
$ git clone git://github.com/vsfs/vobla.git
$ cd vobla
# Download google-mock
$ cd test
$ wget http://googlemock.googlecode.com/files/gmock-1.7.0.zip
$ unzip gmock-*.zip
$ cd gmock-*
$ ./configure && make

$ cd ../..
$ ./bootstrap
$ ./configure
$ make
# Run unit tests
$ make check
~~~~~~~~~~~~~~

### Generate API documents

~~~~~~~~~~~~{.sh}
$ make api
$ open api/html/index.html  # On MacOSX
~~~~~~~~~~~~

### Usage

~~~~~~~~~~~~{.sh}
$ export CXXFLAGS=-I/path/to/vobla
$ export LDFLAGS=-L/path/to/vobla/vobla
$ g++ $CXXFLAGS $LDFLAGS -lvobla -o foo
~~~~~~~~~~~~
