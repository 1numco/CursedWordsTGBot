#!/bin/bash -e

source ./set_versions.sh

docker run --rm -it -v $(pwd):/cwbot -w /cwbot cwbot_base:${COMMON_IMAGES_VERSION} /bin/bash -c "./build-local.sh"

docker build -t "cwbot:${TARGET_IMAGES_VERSION}" --build-arg COMMON_IMAGES_VERSION=${COMMON_IMAGES_VERSION} -f target_images/Dockerfile.CursedWordsTgBot .