from ultralytics import YOLO

model = YOLO("yolo26n.pt")

results = model(
    source=0,
    stream=True,
    show=True,  # 바운딩 박스가 표시되는 창
    save=False,
    imgsz=320,
    device="cpu",
)

# 스트리밍 결과 처리
for r in results:
    pass