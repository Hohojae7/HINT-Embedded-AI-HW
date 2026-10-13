from threading import Thread
from ultralytics import YOLO


def thread_safe_predict(model, image_path):
    # 각 스레드 안에서 별도의 모델 객체 생성
    model = YOLO(model)

    results = model.predict(
        image_path, device="cpu", imgsz=320
    )

    for r in results:
        print(image_path, r.verbose())



thread1 = Thread(
    target=thread_safe_predict,
    args=("yolo26n.pt", "assets/image.jpg"),
)

thread2 = Thread(
    target=thread_safe_predict,
    args=("yolo26n.pt", "assets/image2.jpg"),
)

thread1.start()
thread2.start()

# 두 스레드의 작업이 끝날 때까지 기다림
thread1.join()
thread2.join()