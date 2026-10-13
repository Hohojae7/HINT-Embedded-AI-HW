from pathlib import Path
from ultralytics import YOLO

BASE_DIR = Path(__file__).resolve().parent

model = YOLO(str(BASE_DIR / "yolo26n.pt"))

exported_path = model.export(
    format="onnx",
    imgsz=640,
    simplify=True,
    opset=17,
    device="cpu",
)

print(f"ONNX export 완료: {exported_path}")