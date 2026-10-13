# 11. ResNet-50 이미지 분류 (MATLAB 코드 생성)

USB 웹캠 영상을 ResNet-50으로 분류하는 MATLAB 함수를 C++ 코드로 생성해 Pi에 배포한 실습입니다. ResNet-50은 50층짜리 CNN으로, 층을 건너뛰는 연결(잔차 연결)을 넣어 깊은 네트워크도 잘 학습되도록 만든 모델입니다. ImageNet의 1000개 클래스로 사전 학습되어 있습니다.

| 파일 | 용도 |
|---|---|
| [`raspi_webcam_resnet.m`](raspi_webcam_resnet.m) | Pi에서 실행될 함수. 웹캠 이미지를 1초마다 224×224로 줄여 분류하고, 라벨과 점수를 영상에 써서 Pi 화면에 표시 |
| [`p60_deploy_resnet50.m`](p60_deploy_resnet50.m) | 배포 스크립트. 64비트 Pi용 `targetHardware` 설정, C++ 코드 생성, Pi에서 빌드 후 실행 |
| [`logs/deploy_resnet50_log.txt`](logs/deploy_resnet50_log.txt) | 노트북에서 코드를 생성한 결과 (무한 루프 경고와 함께 성공) |
| [`logs/raspi_webcam_resnet.log`](logs/raspi_webcam_resnet.log) | Pi에서 실행된 프로그램의 로그 (웹캠 캡처 반복) |

- `coder.DeepLearningConfig('none')`은 외부 딥러닝 라이브러리 없이 네트워크를 일반 C++ 코드로 생성한다는 뜻입니다.
- 함수가 `while true`로 계속 도는 카메라 앱이라 코드 생성 때 무한 루프 경고가 나오지만, 의도한 동작입니다.
- 빌드 결과는 `BuildDir`로 지정한 Pi 폴더에 생성되고, 분류 화면은 Pi 데스크톱에 나타납니다.
