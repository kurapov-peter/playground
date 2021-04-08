from ubuntu:20.04
# https://grigorkh.medium.com/fix-tzdata-hangs-docker-image-build-cdb52cc3360d
ENV TZ=Asia/Dubai
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update -y && apt-get install -y --no-install-recommends python git cmake
RUN apt-get install -y build-essential
ENV GIT_SSL_NO_VERIFY=1

RUN git clone https://github.com/kurapov-peter/playground.git
