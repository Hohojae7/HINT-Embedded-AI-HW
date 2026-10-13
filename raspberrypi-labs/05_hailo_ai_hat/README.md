# 05. AI HAT+ (Hailo-8L NPU)

Pi 5의 PCIe 슬롯에 연결하는 AI HAT+(Hailo-8L NPU, 13 TOPS)로 객체를 검출한 실습입니다. 추론은 NPU가 맡고 CPU는 영상 캡처와 화면 표시만 하므로, CPU만 쓸 때보다 큰 모델(YOLOv8s)을 실시간으로 돌릴 수 있습니다.

| 파일 | 내용 |
|---|---|
| [`p97_hailo_detection.py`](p97_hailo_detection.py) | NPU로 객체를 검출하고, 신뢰도 50%를 넘는 결과를 박스와 라벨로 표시 |
| [`p99_hailo_fps.py`](p99_hailo_fps.py) | 화면 출력 없이 100프레임의 캡처와 추론에 걸린 시간을 재서 평균 FPS 출력 |
| [`p100_hailo_save_results.py`](p100_hailo_save_results.py) | p97에 저장 기능을 더해, 프레임마다 검출 결과를 `detections.json`에 한 줄씩 누적 |

- 모델은 `hailo-all` 패키지에 들어 있는 `/usr/share/hailo-models/yolov8s_h8l.hef`를 사용합니다. HEF는 Hailo 칩에서 실행하도록 컴파일된 전용 모델 형식입니다.
- 영상은 Picamera2로 USB 웹캠에서 받습니다. 화면용(1280×720)과 추론용(모델 입력 크기) 스트림을 따로 만들고, 검출 좌표는 화면 해상도에 맞게 다시 계산해서 그립니다.
- `detections.json`은 한 줄에 JSON 하나씩 쓰는 형식(JSON Lines)이며, 시각, 라벨, 신뢰도, 박스 좌표를 남깁니다.
