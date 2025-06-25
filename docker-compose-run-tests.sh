#!/bin/bash
set -e

docker compose up -d running_bot
sleep 10  # Дать время боту инициализироваться

docker compose run --rm generator
sleep 15  # Важно: дать время боту обработать сообщения!

echo "Запуск checker..."
docker compose run --rm checker

# Остановка после проверки
RUNNING_BOT_CONTAINER=$(docker compose ps -q running_bot)
docker kill --signal=SIGINT $RUNNING_BOT_CONTAINER
docker compose down