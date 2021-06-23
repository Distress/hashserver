FROM ubuntu:16.04
EXPOSE 50000
RUN apt-get update && apt-get install -y qt5-default build-essential
WORKDIR /hashserver
COPY . .
RUN mkdir build && cd build \
    && qmake CONFIG+=build_tests -config release ../ \
    && make \
    && make check \
    && make install
CMD ["/usr/local/hashserver/bin/hashserver"]