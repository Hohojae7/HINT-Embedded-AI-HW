from ultralytics import YOLO

model = YOLO("yolo26n.pt")
source = "https://ultralytics.com/images/bus.jpg"

results = model(source, device="cpu", imgsz=320)

for r in results:
    r.show()