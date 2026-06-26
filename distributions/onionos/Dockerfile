FROM debian:buster-slim

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Etc/UTC

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime \
    && echo $TZ > /etc/timezone \
    && sed -i 's|deb.debian.org/debian-security|archive.debian.org/debian-security|g; s|deb.debian.org/debian|archive.debian.org/debian|g' /etc/apt/sources.list \
    && sed -i '/buster-updates/d' /etc/apt/sources.list \
    && printf 'Acquire::Check-Valid-Until "false";\n' > /etc/apt/apt.conf.d/99archive \
    && apt-get -y update \
    && apt-get -y install --no-install-recommends \
        build-essential \
        ca-certificates \
        cmake \
        file \
        libtool \
        make \
        pkg-config \
        tar \
        wget \
        xz-utils \
        zip \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /root

COPY distributions/onionos/support .
RUN ./setup-toolchain.sh
RUN ./setup-sdl2.sh
RUN mkdir -p /opt/onionos \
    && cp setup-env.sh /opt/onionos/setup-env.sh \
    && chmod 0644 /opt/onionos/setup-env.sh \
    && cat setup-env.sh >> .bashrc

VOLUME /workspace
WORKDIR /workspace

CMD ["/bin/bash"]
