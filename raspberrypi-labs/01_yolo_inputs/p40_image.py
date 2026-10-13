from ultralytics import YOLO

model = YOLO("yolo26n.pt")
source = "assets/image.jpg"  

results = model(source, device="cpu", imgsz=320)

# 결과 이미지 표시
for r in results:
    r.show()