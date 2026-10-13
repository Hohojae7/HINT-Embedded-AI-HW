from pathlib import Path
import cv2
from ultralytics import YOLO

BASE_DIR = Path(__file__).resolve().parent

# 일반적인 Linux LED 인터페이스: 1=켜짐, 0=꺼짐
# 실제 LED가 반대로 동작하면 이 값을 0으로 변경
LED_ON_VALUE = 1


# 내장 ACT LED를 강의의 output.on()/off() 방식으로 제어
class ActivityLED:
    def __init__(self, led_path="/sys/class/leds/ACT"):
        self.led_path = Path(led_path)
        self.trigger = self.led_path / "trigger"
        self.brightness = self.led_path / "brightness"

        if not self.trigger.exists():
            raise FileNotFoundError("내장 ACT LED 경로가 없습니다.")

        triggers = self.trigger.read_text().split()
        self.original_trigger = next(
            item[1:-1] for item in triggers if item.startswith("[")
        )
        self.original_brightness = self.brightness.read_text().strip()

        # 저장장치 활동에 따른 자동 점멸을 잠시 해제
        self.trigger.write_text("none\n")

    def on(self):
        self.brightness.write_text(f"{LED_ON_VALUE}\n")

    def off(self):
        self.brightness.write_text(f"{1 - LED_ON_VALUE}\n")

    def close(self):
        # 기존 밝기와 자동 점멸 기능 복원
        try:
            self.brightness.write_text(self.original_brightness + "\n")
        finally:
            self.trigger.write_text(self.original_trigger + "\n")


model = YOLO(str(BASE_DIR / "yolo26n.pt"))

# 강의의 지정 클래스 유지: 사람(0), 책(73)
objects_to_detect = [0, 73]

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

if not cap.isOpened():
    cap.release()
    raise RuntimeError("USB 웹캠을 열 수 없습니다.")

output = None

try:
    # 강의의 output = LED(14)를 내장 LED로 대체
    output = ActivityLED()
    output.off()

    while True:
        success, frame = cap.read()
        if not success:
            print("웹캠에서 프레임을 읽지 못했습니다.")
            break

        # LED 예제는 강의의 입력 크기 160을 유지
        results = model(frame, imgsz=160, device="cpu")
        detected_objects = results[0].boxes.cls.tolist()

        object_found = False
        for obj_id in objects_to_detect:
            if obj_id in detected_objects:
                object_found = True
                print(f"Detected object with ID {obj_id}!")

        if object_found:
            output.on()
            print("ACT LED on")
        else:
            output.off()
            print("ACT LED off")

        annotated_frame = results[0].plot()
        cv2.imshow("Object Detection", annotated_frame)

        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

finally:
    try:
        if output is not None:
            output.close()
    finally:
        cap.release()
        cv2.destroyAllWindows()