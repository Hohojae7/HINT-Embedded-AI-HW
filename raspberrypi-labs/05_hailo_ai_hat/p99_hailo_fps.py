from picamera2 import Picamera2
from picamera2.devices import Hailo
import time


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

        try:
            print("100프레임의 카메라 캡처 + 추론 시간을 측정합니다.")

            start = time.perf_counter()
            frame_count = 0

            while frame_count < 100:
                lores = cam.capture_array("lores")
                results = hailo.run(lores)
                frame_count += 1

            elapsed = time.perf_counter() - start
            fps = frame_count / elapsed

            print(f"처리한 프레임: {frame_count}")
            print(f"걸린 시간: {elapsed:.3f}초")
            print(f"평균 FPS: {fps:.1f}")

        except KeyboardInterrupt:
            print("측정을 중단했습니다.")

        finally:
            cam.stop()
            cam.close()


if __name__ == "__main__":
    main()