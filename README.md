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
values, and sends them back to the client as a response.

The current version is based on Qt5 library and uses it, in particular, for:
- qmake build system
- network interactions
- thread management
- containers

This project was developed on and for Ubuntu 16.04, but will probably work on other operating
systems as well.

---------------------------------------------------------------------------------------------------
## Building and installing
