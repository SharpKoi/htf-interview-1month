FROM ubuntu:24.04

ARG BOOST_VERSION=1.87.0
ARG NUM_JOBS=8

ENV DEBIAN_FRONTEND=noninteractive

WORKDIR /app

ENV LC_ALL=en_US.UTF-8
ENV LANG=en_US.UTF-8
ENV LANGUAGE=en_US.UTF-8

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    g++ \
    wget \
    git \
    curl \
    language-pack-en \
    locales \
    locales-all \
    vim \
    gdb \
    cmake \
    libtool \
    ca-certificates \
    lsb-release \
    libssl-dev \
    nlohmann-json3-dev

RUN wget -P /tmp https://apache.jfrog.io/artifactory/arrow/$(lsb_release --id --short | tr 'A-Z' 'a-z')/apache-arrow-apt-source-latest-$(lsb_release --codename --short).deb && \
    apt-get install -y -V /tmp/apache-arrow-apt-source-latest-$(lsb_release --codename --short).deb && \
    apt-get update && apt-get install -y \
    libarrow-dev \
    libparquet-dev

RUN cd /tmp && \
    BOOST_VERSION_MOD=$(echo $BOOST_VERSION | tr . _) && \
    wget https://archives.boost.io/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION_MOD}.tar.bz2 && \
    tar --bzip2 -xf boost_${BOOST_VERSION_MOD}.tar.bz2 && \
    cd boost_${BOOST_VERSION_MOD} && \
    ./bootstrap.sh --prefix=/usr/local && \
    ./b2 install && \
    rm -rf /tmp/*

RUN apt-get clean && rm -rf /tmp/*

COPY . .

RUN mkdir -p build
RUN cd build && cmake .. && make -j$(nproc)

CMD ["./build/orderbook"]