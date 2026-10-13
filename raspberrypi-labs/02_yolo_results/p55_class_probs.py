from ultralytics import YOLO

model = YOLO("yolo26n-cls.pt")

results = model(
    "https://ultralytics.com/images/bus.jpg",
    device="cpu",
    imgsz=320,
)

for r in results:
    print(r.probs)