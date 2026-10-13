from pathlib import Path
from ultralytics import YOLO

BASE_DIR = Path(__file__).resolve().parent

model = YOLO(str(BASE_DIR / "yolo26n.pt"))

export_path = model.export(
    format="ncnn",
    imgsz=320,
    device="cpu",
)

print("NCNN 모델 저장 위치:", export_path)