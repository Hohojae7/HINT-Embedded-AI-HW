from ultralytics import YOLO

model = YOLO("yolo26n-obb.pt")

results = model(
    "https://ultralytics.com/images/boats.jpg",
    device="cpu",
    imgsz=320,
)

for r in results:
    print(r.obb)