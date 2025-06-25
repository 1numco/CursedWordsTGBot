#!/bin/bash
set -e

echo "Запуск running_bot..."
docker compose up -d running_bot

# Ожидание готовности сервера и бота
echo "Ожидание инициализации (30 секунд)..."
sleep 5

echo "Запуск generator..."
docker compose run --rm generator

echo "Ожидание обработки сообщений (20 секунд)..."
sleep 5

echo "Запуск checker..."
docker compose run --rm checker

echo "Остановка тестовых контейнеров..."
docker compose stop running_bot
docker compose rm -f running_bot generator checker