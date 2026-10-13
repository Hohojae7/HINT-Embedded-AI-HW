from pathlib import Path
import time
import cv2
from ultralytics import YOLO

BASE_DIR = Path(__file__).resolve().parent

model = YOLO(str(BASE_DIR / "yolo11n-pose.pt"))
cap = cv2.VideoCapture(0)

try:
    if not cap.isOpened():
        raise RuntimeError(
            "USB 웹캠을 열 수 없습니다. 연결 상태와 다른 카메라 프로그램을 확인하세요."
        )

    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 320)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 320)

    interval = 1.0
    last_time = time.monotonic()

    while True:
        ret, frame = cap.read()
        if not ret:
            print("웹캠에서 프레임을 읽지 못했습니다.")
            break

        current_time = time.monotonic()
        if current_time - last_time >= interval:
            last_time = current_time

            results = model(frame, device="cpu", imgsz=640)
            annotated_frame = results[0].plot()
            cv2.imshow("YOLO Pose Estimation", annotated_frame)

        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

except KeyboardInterrupt:
    pass

finally:
    cap.release()
    cv2.destroyAllWindows()