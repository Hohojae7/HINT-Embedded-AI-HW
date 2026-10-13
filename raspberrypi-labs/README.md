# Raspberry Pi — 엣지 AI 실습

Raspberry Pi 5에 USB 웹캠을 연결하고, 클라우드나 GPU 없이 보드 안에서 AI를 돌리는 방법을 두 갈래로 다뤘습니다.

- **Python**: Pi에서 YOLO로 객체 검출과 자세 추정을 하고, NPU 가속과 로컬 LLM까지 연결
- **MATLAB·Simulink**: 노트북에서 알고리즘을 만들고, Pi를 원격으로 제어하거나 코드를 생성해 Pi에 배포

## 개발 환경

| 항목 | 내용 |
|---|---|
| 보드 | Raspberry Pi 5 |
| OS | Raspberry Pi OS Trixie (64-bit) |
| 카메라 | USB 웹캠 |
| AI 가속 | AI HAT+ (Hailo-8L NPU) |
| Python | Ultralytics (YOLO26, YOLO11-pose), OpenCV, gpiozero |
| 로컬 LLM | Ollama + gemma3:1b |
| MATLAB | MATLAB·Simulink R2026a, Raspberry Pi Blockset |
| 연결 | Windows 노트북과 Pi를 같은 네트워크로 연결 |

## Python 실습 (Pi에서 실행)

| No | 폴더 | 내용 | 키워드 |
|---|---|---|---|
| 01 | [YOLO 입력 소스](01_yolo_inputs) | 이미지, 화면, URL, 동영상, 폴더, 웹캠을 YOLO26n에 입력 | `source`, `stream`, `conf` |
| 02 | [YOLO 결과 객체](02_yolo_results) | 탐지, 분할, 자세, 분류, 회전 박스 결과를 `Results`에서 꺼내 보기 | `boxes`, `masks`, `keypoints`, `probs`, `obb` |
| 03 | [실시간 추론과 속도 개선](03_yolo_runtime) | 웹캠 추론 루프, NCNN·ONNX 변환과 FPS 비교, 검출 결과로 LED 제어 | OpenCV 루프, NCNN, 스레드 |
| 04 | [자세 추정](04_pose_estimation) | 해상도, 모델 크기, 추론 간격을 바꿔 가며 Pose 모델 속도 개선 | YOLO Pose, 키포인트 |
| 05 | [AI HAT+](05_hailo_ai_hat) | Hailo NPU로 YOLOv8s 검출, FPS 측정, 결과를 JSON으로 저장 | NPU, HEF |
| 06 | [Ollama와 GPIO](06_ollama_gpio) | 로컬 LLM을 REST API로 호출하고, 자연어 명령으로 LED 3개 제어 | REST API, 스트리밍, JSON 출력 |

## MATLAB·Simulink 실습 (노트북에서 개발 → Pi)

| No | 폴더 | 내용 | 키워드 |
|---|---|---|---|
| 07 | [MATLAB 기본 제어](07_matlab_control) | 내장 LED 점멸, Pi 리눅스 명령 실행, 파일 주고받기 | `raspi`, `system`, `getFile` |
| 08 | [GPIO](08_matlab_gpio) | 외부 LED 출력과 버튼 입력 | `writeDigitalPin`, `readDigitalPin` |
| 09 | [시리얼 7세그먼트](09_matlab_serial) | UART로 4자리 7세그먼트에 숫자와 문자 표시 | `serialdev`, UART |
| 10 | [Simulink LED](10_simulink_led) | LED 점멸 모델을 코드 생성으로 Pi에 배포하고 실행 제어 | 모델 기반 설계, 코드 생성 |
| 11 | [ResNet-50 (MATLAB)](11_resnet50_matlab) | 웹캠 이미지 분류 함수를 C++로 생성해 Pi에서 실행 | `targetHardware`, `deploy` |
| 12 | [ResNet-50 (Simulink)](12_resnet50_simulink) | 같은 분류를 Simulink 템플릿으로 구성, 시뮬레이션 후 배포 | Image Classifier, V4L2 |
| 13 | [정지 표지판 검출](13_stop_sign_detector) | cascade 검출기를 학습하고 Simulink 모델로 웹캠 영상에서 검출 | Viola-Jones, HOG |

## 실습하며 정리한 것

- YOLO는 이미지를 한 번만 보고 물체의 위치와 종류를 함께 예측하는 1-stage 검출기입니다. YOLO26은 겹친 박스를 걸러 내는 NMS 후처리까지 없애서, 모델 출력이 곧 최종 결과입니다.
- GPU가 없는 Pi에서는 속도가 가장 큰 과제였습니다. 입력 크기를 줄이고, 작은(nano) 모델을 쓰고, ARM용 형식(NCNN)으로 바꾸고, 추론 간격을 늘리는 방법을 차례로 적용했습니다.
- 추론을 NPU에 맡기면 CPU는 영상 입출력만 처리하면 되므로, 더 큰 모델도 실시간으로 돌릴 수 있습니다.
- LLM으로 하드웨어를 움직일 때는 답을 정해진 JSON 형식으로 받고, 허용된 장소와 동작인지 검사한 뒤에만 GPIO를 제어했습니다.
- MATLAB에서는 두 방식을 비교했습니다. `raspi` 객체로 명령을 하나씩 보내는 방식은 바로 시험해 보기 좋고, 코드를 생성해 배포한 프로그램은 MATLAB 없이 Pi에서 혼자 돌아갑니다.
- 분류(ResNet-50)는 이미지 전체에 라벨 하나를 붙이고, 검출(YOLO, cascade)은 물체마다 위치와 라벨을 알려 줍니다.

## 실행 전 준비

- **Python**: 가상환경에 `pip install ultralytics`로 설치합니다. YOLO 모델 파일(`.pt`)은 레포에 넣지 않았고, 처음 실행할 때 자동으로 내려받습니다.
- **MATLAB**: Raspberry Pi Blockset을 설치하고 `raspisetup`으로 Pi 설정을 마칩니다. 스크립트는 Pi 비밀번호를 환경변수에서 읽으므로 먼저 `setenv('RPI_PASSWORD', '비밀번호')`를 실행합니다.
