FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y \
    build-essential \
    zlib1g-dev \
    gcc \
    git \
    zstd \
    wget \
    curl \
    make \
    && apt clean

WORKDIR /app

COPY . .

RUN make

ENTRYPOINT ["./data_compressor"]
CMD ["help"]
