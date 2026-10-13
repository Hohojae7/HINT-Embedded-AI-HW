# 01. YOLO 추론 입력 소스

같은 YOLO26n 모델에 여러 종류의 입력을 넣어 보며 Ultralytics의 `source` 인자를 익힌 실습입니다. YOLO26n은 COCO 80개 클래스로 사전 학습된 가장 작은(nano) 탐지 모델입니다. 모든 파일은 Pi CPU에서 돌리기 위해 `device="cpu"`, `imgsz=320`으로 맞췄습니다.

| 파일 | 입력 | 용도 |
|---|---|---|
| [`p40_image.py`](p40_image.py) | `assets/image.jpg` | 이미지 파일 한 장을 추론하고 결과 창으로 확인 |
| [`p40_screen.py`](p40_screen.py) | `"screen"` | 지금 모니터 화면을 캡처해 한 번만 추론하고 종료 |
| [`p40_url.py`](p40_url.py) | 이미지 URL | 인터넷의 예제 이미지(`bus.jpg`)를 내려받아 추론 |
| [`p41_video.py`](p41_video.py) | `assets/video.mp4` | 동영상을 프레임마다 추론하며 재생 |
| [`p41_directory.py`](p41_directory.py) | `assets/` 폴더 | 폴더 안의 이미지와 동영상을 한 번에 추론 |
| [`p41_webcam.py`](p41_webcam.py) | `source=0` | USB 웹캠 영상을 실시간으로 추론 |
| [`p42_predict_args.py`](p42_predict_args.py) | 이미지 URL | `predict()`에 `save`, `imgsz`, `conf` 인수를 주고, 결과 이미지를 `runs/detect/` 아래에 저장 |

- 동영상, 폴더, 웹캠처럼 입력이 계속 들어오는 경우는 `stream=True`로 결과를 한 프레임씩 받아 메모리를 아낍니다.
- `conf=0.25`는 신뢰도가 25% 미만인 박스를 버리는 기준입니다. 낮출수록 더 많이 잡지만 잘못 잡는 경우도 늘어납니다.
- `assets/` 폴더의 이미지와 영상은 직접 준비한 파일이라 레포에는 넣지 않았습니다.
