import grpc
from concurrent import futures
import torch
from transformers import AutoTokenizer, AutoModelForSequenceClassification
import toxicity_classifier_pb2
import toxicity_classifier_pb2_grpc
import logging
import sys

# Импорт health-сервисов
from grpc_health.v1 import health, health_pb2, health_pb2_grpc

logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    handlers=[logging.StreamHandler(sys.stdout)]
)
logger = logging.getLogger(__name__)

class ToxicityClassifierServicer(toxicity_classifier_pb2_grpc.ToxicityClassifierServicer):
    def __init__(self):
        self.model_name = 's-nlp/russian_toxicity_classifier'
        try:
            logger.info("Initializing model...")
            self.tokenizer = AutoTokenizer.from_pretrained(
                self.model_name,
                local_files_only=False
            )
            self.model = AutoModelForSequenceClassification.from_pretrained(
                self.model_name,
                local_files_only=False
            )
            logger.info("Model ready")
            
            # Проверка размерности выходов
            test_input = self.tokenizer("test", return_tensors="pt")
            with torch.no_grad():
                test_output = self.model(**test_input).logits
            self.num_labels = test_output.shape[-1]
            logger.info(f"Model outputs: {self.num_labels} logits")

        except Exception as e:
            logger.critical(f"Initialization failed: {e}")
            raise

    def ClassifyMessage(self, request, context):
        try:
            # Токенизация
            inputs = self.tokenizer(
                request.message,
                return_tensors="pt",
                truncation=True,
                padding=True,
                max_length=512
            )
            
            # Предсказание
            with torch.no_grad():
                logits = self.model(**inputs).logits

            # Обработка выходов модели
            if self.num_labels == 1:  # Бинарная классификация
                prob = torch.sigmoid(logits).item()
            elif self.num_labels == 2:  # Два класса
                prob = torch.softmax(logits, dim=1)[0][1].item()
            else:
                raise ValueError("Unsupported model output format")

            logger.info(f"Processed: '{request.message}' | Prob: {prob:.4f}")
            
            return toxicity_classifier_pb2.MessageResponse(
                toxicity_probability=prob
            )

        except Exception as e:
            logger.error(f"Error: {str(e)}")
            context.set_code(grpc.StatusCode.INTERNAL)
            context.set_details(f"Processing error: {str(e)}")
            return toxicity_classifier_pb2.MessageResponse()

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    # Регистрируем основной сервис
    toxicity_classifier_pb2_grpc.add_ToxicityClassifierServicer_to_server(
        ToxicityClassifierServicer(), server
    )
    
    # Добавляем health check сервис
    health_servicer = health.HealthServicer()
    # Здесь пустая строка обозначает общий статус сервера; можно задать имя сервиса
    health_servicer.set("", health_pb2.HealthCheckResponse.SERVING)
    health_pb2_grpc.add_HealthServicer_to_server(health_servicer, server)
    
    server.add_insecure_port('[::]:50051')
    logger.info("Starting server on port 50051")
    server.start()
    server.wait_for_termination()

if __name__ == '__main__':
    serve()