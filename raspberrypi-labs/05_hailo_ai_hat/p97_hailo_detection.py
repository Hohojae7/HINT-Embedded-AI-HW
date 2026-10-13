from picamera2 import Picamera2
from picamera2.devices import Hailo
import cv2


def main():
    model_path = "/usr/share/hailo-models/yolov8s_h8l.hef"

    with Hailo(model_path) as hailo:
        model_h, model_w, _ = hailo.get_input_shape()

        cam = Picamera2()
        config = cam.create_preview_configuration(
            main={"size": (1280, 720), "format": "RGB888"},
            lores={"size": (model_w, model_h), "format": "RGB888"},
        )
        cam.configure(config)
        cam.start()

        print(f"모델 입력 크기: {model_w}x{model_h}")
        print("객체 인식 시작: 영상 창에서 q로 종료")

        try:
            while True:
                frame = cam.capture_array("main")
                lores = cam.capture_array("lores")

                results = hailo.run(lores)

               
                for detection in results.get("detections", []):
                    label = detection["label"]
                    confidence = detection["confidence"]
                    bbox = detection["bbox"]

                    if confidence > 0.5:
                        x1, y1, x2, y2 = bbox

                        # 모델 입력 좌표를 화면 해상도에 맞게 변환
                        x1 = int(x1 * 1280 / model_w)
                        y1 = int(y1 * 720 / model_h)
                        x2 = int(x2 * 1280 / model_w)
                        y2 = int(y2 * 720 / model_h)

                        cv2.rectangle(
                            frame, (x1, y1), (x2, y2), (0, 255, 0), 2
                        )

                        text = f"{label}: {confidence:.1%}"
                        cv2.putText(
                            frame,
                            text,
                            (x1, y1 - 10),
                            cv2.FONT_HERSHEY_SIMPLEX,
                            0.6,
                            (0, 255, 0),
                            2,
                        )

                cv2.imshow("AI HAT+ Object Detection", frame)

                if cv2.waitKey(1) & 0xFF == ord("q"):
                    break

        except KeyboardInterrupt:
            pass

        finally:
            cam.stop()
            cam.close()
            cv2.destroyAllWindows()


if __name__ == "__main__":
    main()