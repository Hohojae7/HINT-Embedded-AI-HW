from ultralytics import YOLO

model = YOLO("yolo26n.pt")

single_results = model(
    "https://ultralytics.com/images/bus.jpg",
    device="cpu",
    imgsz=320,
)

batch_results = model(
    [
        "https://ultralytics.com/images/bus.jpg",
        "https://ultralytics.com/images/zidane.jpg",
    ],
    device="cpu",
    imgsz=320,
)

print("단일 추론 결과 개수:", len(single_results))
print("배치 추론 결과 개수:", len(batch_results))