# Hash server

// Copyright 2021-present Georgiy Brun Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

---------------------------------------------------------------------------------------------------
## License

The whole contents of this package, including all C/C++ source code, is licensed as Open Source
under the terms of Mozilla Public License 2.0: www.mozilla.org/MPL/2.0/, with the possible
exception of certain files which may be licensed under the terms of other open-source licenses
explicitly referenced in those files.

See the license text in [LICENSE](LICENSE) file located in the root directory of this repository.

---------------------------------------------------------------------------------------------------
## Introduction

Hash server is a simple TCP server that receives strings from clients, calculates their hash 
values, and sends them back to the client as a response. Each line must end with the '\n ' character.

The current version is based on Qt5 library and uses it, in particular, for:
- qmake build system
- network interactions
- thread management
- containers

This project was developed on and for Ubuntu 16.04, but will probably work on other operating
systems as well.

---------------------------------------------------------------------------------------------------
## Building and installing

The repository contains git submodule (for hash algorithm), so for cloning you should use 
`git clone --recurse-submodules` or execute `git submodule update --init `  after classic clone.
Build process is based on the qmake tool.

There are three possible options:

- The first one is to use `build.sh` a script that will install dependencies, build server, 
build and run tests, make `.deb` package in the root of  `./build` directory.  After installing 
this package, systemd `hashserver` service will be enabled, so you can simply start it with 
`sudo systemctl start hashserver` command. Default port number is 50000, 
inactive session timeout is 1min. Parameters will be saved in the standard path for QSettings. 
In addition, a simple test script will be installed in `/usr/local/hashserver/bin`.

- The second one is to use `Dockerfile`. After building the image and running it with mapping 
50000 port, you will be able to test the server, for example, with telnet or netcat utility.

- Or you can do everything manually:

  ```bash
  sudo apt-get -y install build-essential qt5-default # to install dependencies
  git clone --recurse-submodules git@github.com:Distress/hashserver.git
  cd hashserver
  mkdir build && cd build
  qmake CONFIG+=build_tests -config release ../ # skip build_tests if you don't need them
  make
  make check # to run tests
  make package # to build .deb package
  sudo dpkg -i ./*.deb # to install builded package
  ```
