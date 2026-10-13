# 03. 실시간 추론과 속도 개선

웹캠 실시간 추론 루프를 만들고, 모델 형식을 바꿔 속도를 비교한 뒤, 검출 결과로 LED를 제어한 실습입니다.

| 파일 | 내용 |
|---|---|
| [`p59_thread_safe_inference.py`](p59_thread_safe_inference.py) | 스레드 두 개가 각자 모델 객체를 만들어 이미지 두 장을 동시에 추론. 모델 하나를 여러 스레드가 같이 쓰지 않는 것이 핵심 |
| [`p60_opencv_video_loop.py`](p60_opencv_video_loop.py) | `cv2.VideoCapture`로 웹캠 프레임을 읽고 추론, 그리기, 표시를 반복하는 기본 루프. `source`만 바꾸면 동영상 파일도 가능 |
| [`p75_webcam_fps.py`](p75_webcam_fps.py) | PyTorch 모델(`yolo26n.pt`)로 웹캠을 추론하면서 추론 FPS를 화면 오른쪽 위에 표시 |
| [`p76_export_ncnn.py`](p76_export_ncnn.py) | `yolo26n.pt`를 NCNN 형식(`yolo26n_ncnn_model/`)으로 변환 |
| [`p77_webcam_fps_ncnn.py`](p77_webcam_fps_ncnn.py) | p75와 같은 조건으로 NCNN 모델의 추론 FPS 측정 (p76을 먼저 실행) |
| [`p84_webcam_led.py`](p84_webcam_led.py) | 사람(클래스 0)이나 책(73)이 보이면 LED를 켜고, 없으면 끔 |
| [`p85_export_onnx.py`](p85_export_onnx.py) | 같은 모델을 ONNX 형식으로 변환 (그래프 단순화, opset 17) |

## PyTorch와 NCNN 비교

NCNN은 ARM CPU에 맞춰 최적화된 추론 엔진입니다. p75와 p77은 모델만 다르고 웹캠 해상도(640×480), 입력 크기(320×320, `rect=False`), FPS 계산 방식이 같아서 화면의 숫자를 그대로 비교할 수 있습니다. FPS는 `results[0].speed["inference"]`(추론 시간, ms)로 계산하므로 캡처와 그리기 시간은 빠져 있습니다.

## LED 제어 (p84)

외부 LED 대신 Pi 보드의 내장 ACT LED를 썼습니다. `/sys/class/leds/ACT`의 `trigger`를 `none`으로 바꿔 저장장치 활동 표시를 잠시 끄고, `brightness`로 켜고 끕니다. 종료할 때는 원래 설정으로 되돌립니다. 이 예제의 입력 크기는 160으로 더 작게 했습니다.
