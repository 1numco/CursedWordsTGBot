#!/bin/bash
set -e

echo "Запуск running_bot..."
docker compose up -d running_bot

# Ожидание готовности сервера и бота
echo "Ожидание инициализации (30 секунд)..."
sleep 30

# Сбор логов для диагностики
docker compose logs grpc_server > grpc_logs.txt
docker compose logs running_bot > bot_logs.txt
echo "=== Логи grpc_server ==="
cat grpc_logs.txt
echo "=== Логи running_bot ==="
cat bot_logs.txt

echo "Запуск generator..."
docker compose run --rm generator

echo "Ожидание обработки сообщений (20 секунд)..."
sleep 20

echo "Запуск checker..."
docker compose run --rm checker

echo "Остановка тестовых контейнеров..."
docker compose stop running_bot
docker compose rm -f running_bot generator checker