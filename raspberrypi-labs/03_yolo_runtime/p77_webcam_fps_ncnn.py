from pathlib import Path
import cv2
from ultralytics import YOLO

BASE_DIR = Path(__file__).resolve().parent
IMGSZ = 320

model_dir = BASE_DIR / "yolo26n_ncnn_model"

if not model_dir.is_dir():
    raise FileNotFoundError("먼저 p76_export_ncnn.py를 실행하세요.")

# 변환된 NCNN 모델 폴더를 불러옴
model = YOLO(str(model_dir), task="detect")

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

if not cap.isOpened():
    cap.release()
    raise RuntimeError("USB 웹캠을 열 수 없습니다.")

try:
    while True:
        success, frame = cap.read()
        if not success:
            print("웹캠에서 프레임을 읽지 못했습니다.")
            break

        # 두 모델을 같은 320x320 입력 조건으로 비교
        results = model(
            frame, imgsz=IMGSZ, rect=False, device="cpu"
        )
        annotated_frame = results[0].plot()

        # 추론 시간(ms)을 FPS로 환산
        inference_time = results[0].speed["inference"]
        fps = 1000 / inference_time if inference_time > 0 else 0
        text = f"Infer FPS: {fps:.1f}"

        font = cv2.FONT_HERSHEY_SIMPLEX
        text_size = cv2.getTextSize(text, font, 1, 2)[0]
        text_x = annotated_frame.shape[1] - text_size[0] - 10
        text_y = text_size[1] + 10

        cv2.putText(
            annotated_frame,
            text,
            (text_x, text_y),
            font,
            1,
            (255, 255, 255),
            2,
            cv2.LINE_AA,
        )
        cv2.imshow("YOLO NCNN", annotated_frame)

        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

finally:
    cap.release()
    cv2.destroyAllWindows()