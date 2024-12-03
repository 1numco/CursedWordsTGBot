#!/bin/bash -e

docker run --rm -it -v $(pwd):/app -w /app tgbot_base:1.0 /bin/bash -c "./build-local.sh"
ls 
docker build -t "tgbot:1.0" --build-arg BASE_IMAGE_VERSION=1.0 -f target_images/Dockerfile.CursedWordsTgBot .