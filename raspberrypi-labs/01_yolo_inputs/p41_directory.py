from ultralytics import YOLO

model = YOLO("yolo26n.pt")
source = "assets"  # 이미지와 영상이 들어 있는 폴더

results = model(
    source, stream=True, show=True, device="cpu", imgsz=320
)

# 스트리밍 결과 처리
for r in results:
    pass