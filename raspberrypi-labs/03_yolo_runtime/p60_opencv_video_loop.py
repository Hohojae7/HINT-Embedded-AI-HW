import cv2
from ultralytics import YOLO

model = YOLO("yolo26n.pt")

source = 0

# 동영상 파일로 실습하려면 위 줄을 다음과 같이 변경:
# source = "assets/video.mp4"

cap = cv2.VideoCapture(source)

# 입력을 열지 못했을 때 원인 표시
if not cap.isOpened():
    raise RuntimeError("카메라 또는 영상 파일을 열 수 없습니다.")

# 종료할 때 입력과 창을 정리하도록 try/finally 사용
try:
    while cap.isOpened():
        success, frame = cap.read()

        if not success:
            break

        results = model(frame, device="cpu", imgsz=320)

        annotated_frame = results[0].plot()
        cv2.imshow("YOLO Inference", annotated_frame)

        # 결과 창을 선택하고 q를 누르면 종료
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

finally:
    cap.release()
    cv2.destroyAllWindows()