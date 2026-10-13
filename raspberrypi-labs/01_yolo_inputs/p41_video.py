from ultralytics import YOLO

model = YOLO("yolo26n.pt")
source = "assets/video.mp4"  # 직접 준비한 영상 경로

# CPU 설정과 결과 창 표시
results = model(
    source, stream=True, show=True, device="cpu", imgsz=320
)

for r in results:
    pass  # 개별 결과를 처리할 코드를 여기에 추가할 수 있음