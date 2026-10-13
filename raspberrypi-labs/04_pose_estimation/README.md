# 04. 자세 추정 (YOLO Pose)

웹캠 영상에서 사람의 관절 17개(코, 눈, 어깨, 팔꿈치, 손목, 무릎 등)를 찾아 뼈대처럼 그리는 실습입니다. GPU가 없는 Pi에서는 첫 버전이 너무 느려서, 한 번에 한 가지씩 조건을 바꿔 가며 속도를 개선했습니다.

| 파일 | 모델 | 웹캠 해상도 | 바꾼 점 |
|---|---|---|---|
| [`p87_pose_baseline.py`](p87_pose_baseline.py) | yolo11m-pose | 640×640 | 기준 코드 |
| [`p89_pose_small_frame.py`](p89_pose_small_frame.py) | yolo11m-pose | 320×320 | 캡처 해상도를 낮춤 |
| [`p90_pose_nano.py`](p90_pose_nano.py) | yolo11n-pose | 320×320 | 모델을 medium에서 nano로 교체 |
| [`p91_pose_interval.py`](p91_pose_interval.py) | yolo11n-pose | 320×320 | 추론을 1초에 한 번만 실행 |

- nano 모델은 medium보다 훨씬 가벼워서, 정확도를 조금 내주는 대신 추론 시간이 크게 줄어듭니다.
- 네 파일 모두 모델 입력 크기는 `imgsz=640`으로 같습니다.
- p91은 프레임은 계속 읽고 추론만 1초 간격으로 합니다. 화면은 1초마다 바뀌지만 `q` 키는 바로 반응합니다.
