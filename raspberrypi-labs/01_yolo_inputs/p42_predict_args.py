from ultralytics import YOLO

model = YOLO("yolo26n.pt")

model.predict(
    "https://ultralytics.com/images/bus.jpg",
    save=True,
    imgsz=320,
    conf=0.25,
    device="cpu",  
)