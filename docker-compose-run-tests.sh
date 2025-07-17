#!/bin/bash

set -e

docker compose down --remove-orphans

docker compose up -d running_bot

docker compose run --rm generator  # Ожидаем завершения

RUNNING_BOT_CONTAINER=$(docker compose ps -q running_bot)
docker kill --signal=SIGINT $RUNNING_BOT_CONTAINER

echo "Запуск checker..."
docker compose run --rm checker

# Остановка всех сервисов
echo "Остановка всех сервисов..."
docker compose down