FROM --platform=linux/amd64 ubuntu:22.04 as builder

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential libfreetype6-dev

COPY . /repo
WORKDIR /repo
RUN ./configure --without-freetype --prefix=/install
RUN make -j8
RUN make install

RUN ./configure --without-freetype
RUN make -j8
RUN make install

WORKDIR /repo/tools/agar-disasm
RUN ./configure --prefix=/install
RUN make
RUN make install
ENV LD_LIBRARY_PATH=/repo/core

FROM ubuntu:22.04 as package
COPY --from=builder /install /install
ENV LD_LIBRARY_PATH=/install/lib
