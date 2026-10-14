<div align="center">

# HINT 임베디드 AI HW 1기

**2026.07.27 – 2026.10.22**

<img alt="HINT Embedded AI HW" src="https://github.com/user-attachments/assets/ba9d3cf9-1cff-4fae-8a37-d2f1c8bd22ed" width="100%"/>

<br>

<p>
  <img alt="C" src="https://img.shields.io/badge/C-A8B9CC?style=flat-square&logo=c&logoColor=white"/>
  <img alt="Python" src="https://img.shields.io/badge/Python-3776AB?style=flat-square&logo=python&logoColor=white"/>
  <img alt="MATLAB" src="https://img.shields.io/badge/MATLAB-E16737?style=flat-square&logo=mathworks&logoColor=white"/>
  <img alt="Simulink" src="https://img.shields.io/badge/Simulink-0076A8?style=flat-square&logo=mathworks&logoColor=white"/>
  <img alt="AURIX TC275" src="https://img.shields.io/badge/AURIX_TC275-0A8276?style=flat-square&logo=infineon&logoColor=white"/>
  <img alt="AUTOSAR" src="https://img.shields.io/badge/AUTOSAR-1C3F75?style=flat-square"/>
  <img alt="CAN" src="https://img.shields.io/badge/CAN-6E3FA3?style=flat-square"/>
  <img alt="Raspberry Pi" src="https://img.shields.io/badge/Raspberry_Pi-A22846?style=flat-square&logo=raspberrypi&logoColor=white"/>
  <img alt="Arduino" src="https://img.shields.io/badge/Arduino-00878F?style=flat-square&logo=arduino&logoColor=white"/>
</p>

</div>

현대자동차그룹 **HINT 임베디드 AI HW 1기** 과정에서 진행한 실습 결과물을 모아 둔 저장소입니다.
<br/>
분야별(MCU, AUTOSAR, CAN, 모델 기반 제어, 엣지 AI)로 폴더를 나누고, 폴더마다 README에 파일 구성과 동작을 정리했습니다.

## 디렉토리 구조

```
HINT-Embedded-AI-HW/
├── AURIX_TC275/
│   ├── Lab01 ~ Lab05      # GPIO: LED, 버튼, RGB
│   ├── Lab06 ~ Lab10      # 인터럽트·타이머: STM, ERU, 1 ms tick, DHT11
│   ├── Lab11 ~ Lab16      # 아날로그: VADC, DSADC, 필터
│   └── my_*.c / my_*.h    # 공통 모듈 (gpio, tick, dht11, adc, scan, dsadc)
├── python-can/
│   ├── 00 ~ 02            # 기본 송수신
│   ├── 03 ~ 07            # DBC 기반 주기 송신
│   ├── 08 ~ 10            # 필터, 송수신 통합, 리모트 프레임
│   └── project.dbc
├── AUTOSAR/
│   ├── 01 ~ 02            # OS: Task, Alarm
│   ├── 03 ~ 05            # RTE: Timing Event, S/R, C/S
│   └── 06 ~ 09            # BSW: IoHwAb, CAN, NvM → TORCS 통합
├── Simulink/
│   ├── 01 ~ 03            # 동적 시스템, Simscape, C 코드 생성
│   ├── 04 ~ 07            # 종방향: 차량 모델, ABS, 크루즈, ACC
│   ├── 08                 # 횡방향 모델, 칼만 필터
│   └── 09                 # Arduino 배포
└── raspberrypi-labs/
    ├── 01 ~ 06            # Python: YOLO, Pose, NPU, LLM
    └── 07 ~ 13            # MATLAB/Simulink: GPIO, UART, ResNet-50, 정지 표지판
```

## Course Navigation

| No | 과정 | 실습 결과물 | 키워드 |
|:-:|---|---|---|
| 01 | [MCU 프로그래밍](#course-01) | [AURIX_TC275](AURIX_TC275) · Lab 16개 | GPIO, 인터럽트, 타이머, ADC |
| 02 | [차량 통신 (CAN)](#course-02) | [python-can](python-can) · 실습 11개 | CAN, DBC, 주기 송신 |
| 03 | [AUTOSAR](#course-03) | [AUTOSAR](AUTOSAR) · 실습 9개 | SWC, RTE, BSW |
| 04 | [모델 기반 제어](#course-04) | [Simulink](Simulink) · 실습 9개 | 차량 동역학, 제어기, 상태 추정 |
| 05 | [Raspberry Pi 엣지 AI](#course-05) | [raspberrypi-labs](raspberrypi-labs) · 실습 13개 | YOLO, NPU, 로컬 LLM |
| 06 | [(과정명)](#course-06) | - | - |

## Curriculum

<a id="course-01"></a>
### 01. MCU 프로그래밍

> Infineon AURIX TC275에서 HAL 없이 레지스터를 직접 설정해 GPIO, 인터럽트, 타이머, ADC를 다루는 bare-metal 펌웨어를 작성했습니다.

**실습** [AURIX_TC275](AURIX_TC275) · ShieldBuddy TC275 · AURIX Development Studio

<details>
<summary><b>상세 학습 내용</b></summary>
<br>

- **디지털 입출력**: IOCR로 핀 모드를 정하고 OMR, IN 레지스터로 LED·버튼·RGB 제어, 소프트웨어 디바운스와 에지 검출
- **인터럽트와 타이머**: STM compare 인터럽트, ERU 외부 인터럽트, SRC 설정과 ISR, ISR과 main 사이 임계 구역
- **주기 실행**: 1 ms tick 기반 비선점 다중 주기 태스크
- **센서 통신**: DHT11 단선 통신, 펄스 폭 측정, checksum, 측정 상태 머신
- **아날로그**: VADC 단발·tick trigger·scan 변환, ADC 결과 인터럽트와 ring buffer, DSADC
- **신호 처리**: 이동평균, 3점 중앙값, 고정소수점 IIR 저역통과, 히스테리시스

**실습하며 한 일**
- 실습마다 복사해 쓰던 GPIO·tick·ADC 코드를 공통 모듈로 분리
- 출력 핀을 입력 모드(IOCR `0x10`)로 설정하던 버그를 찾아 push-pull 출력(`0x80`)으로 수정

</details>

<a id="course-02"></a>
### 02. 차량 통신 (CAN)

> CAN 프레임 구조와 DBC를 익히고, Kvaser 가상 CAN 채널 위에서 python-can 스크립트로 가상 ECU를 만들어 메시지를 주고받았습니다.

**실습** [python-can](python-can) · Kvaser 가상 채널 · CanKing

<details>
<summary><b>상세 학습 내용</b></summary>
<br>

- **CAN 기초**: 표준·확장 ID, DLC, 데이터·리모트 프레임, 비트레이트와 비트 타이밍(샘플 포인트)
- **DBC**: cantools로 메시지·신호 정의 읽기, 신호 인코딩, Intel·Motorola 바이트 순서
- **주기 송신**: DBC의 송신 주기 반영, 단일 루프 → 스레드 → `send_periodic()`
- **수신**: ID·마스크 필터, 수신 스레드와 송신 스레드 동시 실행

**실습하며 한 일**
- 단일 루프 송신에서 주기가 누적되는 문제를 확인하고, 메시지마다 독립 주기로 보내도록 개선
- 모든 스크립트에 종료 처리와 수신 timeout을 넣고, 만들기만 하던 `BitTiming`을 실제 버스 설정에 적용

</details>

<a id="course-03"></a>
### 03. AUTOSAR

> AUTOSAR Classic의 Application, RTE, BSW 계층을 Mobilgene Studio로 구성하고, TRK-MPC5606B 보드에서 SWC와 BSW를 연결했습니다.

**실습** [AUTOSAR](AUTOSAR) · Mobilgene Studio · CodeWarrior · TRK-MPC5606B

<details>
<summary><b>상세 학습 내용</b></summary>
<br>

- **OS**: Task 활성화·종료, Counter와 Alarm 기반 주기 Task
- **RTE**: SWC, Runnable, Timing Event, Sender/Receiver와 Client/Server 인터페이스
- **BSW**: IoHwAb로 ADC·DIO·PWM 추상화, DBC import와 COM 신호, NvM 비동기 읽기·쓰기
- **통합**: TORCS 시뮬레이터와 CAN으로 연동한 CC·LKAS SWC

**실습하며 한 일**
- 제공된 CC 로직의 조건식을 정리해, 실제로는 성립하지 않는 가속 보정 분기를 찾아 기록

</details>

<a id="course-04"></a>
### 04. 모델 기반 제어

> MATLAB/Simulink로 동적 시스템과 차량 동역학을 모델링하고, 그 위에서 ABS·크루즈·ACC 제어기와 칼만 필터를 설계한 뒤 C 코드 생성과 Arduino 배포까지 진행했습니다.

**실습** [Simulink](Simulink) · MATLAB·Simulink·Simscape · Arduino Uno

<details>
<summary><b>상세 학습 내용</b></summary>
<br>

- **Simulink 기초**: 미분방정식을 블록선도로 구성, 워크스페이스 연동, `sim()` 반복 실행
- **물리 모델링**: Simscape 기어 회전계와 DC 모터, 2차 시스템 응답과 감쇠비
- **종방향 차량**: 슬립률, Magic Formula 타이어, 공기·구름저항, Vehicle Dynamics Blockset
- **종방향 제어**: ABS 슬립 임계 제어, 크루즈 PI + 피드포워드, ACC 등간격·등시간간격 비교와 string stability
- **횡방향 차량과 추정**: 자전거 모델과 상태방정식, 개루프 추정, 칼만 필터 횡슬립각 추정
- **코드 생성**: Simulink Coder로 PI 제어기 C 코드 생성, Arduino Uno에 10 ms 주기로 배포

</details>

<a id="course-05"></a>
### 05. Raspberry Pi 엣지 AI

> Raspberry Pi 5와 USB 웹캠으로 YOLO 객체 검출·자세 추정, NPU 가속, 로컬 LLM을 다루고, MATLAB/Simulink에서 만든 알고리즘을 Pi에 배포했습니다.

**실습** [raspberrypi-labs](raspberrypi-labs) · Raspberry Pi 5 · AI HAT+ · MATLAB/Simulink

<details>
<summary><b>상세 학습 내용</b></summary>
<br>

- **YOLO**: 입력 소스, 작업별 결과(탐지·분할·자세·분류·회전 박스), OpenCV 실시간 추론 루프
- **속도 개선**: 입력 크기 축소, nano 모델, NCNN 변환, 추론 간격 조절
- **AI 가속**: AI HAT+(Hailo-8L NPU)와 HEF 모델 추론
- **로컬 LLM**: Ollama REST API, JSON 출력 검증 후 GPIO 제어
- **MATLAB/Simulink 연동**: `raspi` 원격 제어, GPIO·UART, 코드 생성 배포, ResNet-50 분류, cascade 검출기

</details>

<a id="course-06"></a>
### 06. (과정명)

> (학습 내용 한두 줄)

<details>
<summary><b>상세 학습 내용</b></summary>
<br>

- (주제): (내용)

</details>
