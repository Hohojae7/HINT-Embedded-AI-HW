from ultralytics import YOLO

model = YOLO("yolo26n.pt")
source = "screen"

# 화면 입력을 스트리밍으로 받고 한 번만 처리
results = model(source, stream=True, device="cpu", imgsz=320)

try:
    for r in results:
        r.show()
        break
finally:
    results.close()