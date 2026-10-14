# 임베디드·차량 SW 실습

MCU 레지스터 제어에서 시작해 AUTOSAR, CAN 통신, 모델 기반 제어까지 차량용 임베디드 SW를 직접 구현하며 정리한 저장소입니다. 실습별로 폴더를 나눴고, 폴더마다 README에 파일 구성과 동작을 적어 두었습니다.

## 한눈에 보기

| 영역 | 폴더 | 한 일 | 키워드 |
|---|---|---|---|
| MCU | [AURIX_TC275](AURIX_TC275) | Infineon TC275 레지스터를 직접 설정하는 bare-metal 실습 16개 | GPIO, STM, ERU, VADC, DSADC |
| AUTOSAR | [AUTOSAR](AUTOSAR) | OS부터 RTE, BSW, CAN, NvM까지 9개 실습과 TORCS 연동 CC·LKAS | SWC, RTE, COM, NvM |
| 차량 통신 | [python-can](python-can) | Kvaser 가상 CAN에서 DBC 기반 가상 ECU 송수신 | CAN, DBC, 주기 송신, 필터 |
| 제어·MBD | [Simulink](Simulink) | 종·횡방향 차량 모델 위에 ABS, 크루즈, ACC, 칼만 필터 설계와 코드 생성 | 차량 동역학, 상태 추정 |
| 엣지 AI | [raspberrypi-labs](raspberrypi-labs) | Raspberry Pi 5에서 YOLO, NPU, 로컬 LLM과 MATLAB/Simulink 배포 | YOLO, Hailo, Ollama |

## 핵심 경험

### 1. 레지스터 수준의 MCU 펌웨어 — [AURIX_TC275](AURIX_TC275)

- HAL 없이 iLLD 레지스터 헤더로 포트, STM, ERU, VADC, DSADC를 직접 설정했습니다.
- LED 점멸에서 출발해 1 ms tick 기반 다중 주기 태스크, ADC 결과 인터럽트와 ring buffer, IIR 필터와 히스테리시스까지 펌웨어 구조를 단계적으로 쌓았습니다.
- 실습마다 복사해 쓰던 GPIO·tick·ADC 코드를 공통 모듈로 분리했습니다. 그 과정에서 출력 핀을 입력 모드(IOCR `0x10`)로 설정해 LED가 켜지지 않던 버그를 찾아 push-pull 출력(`0x80`)으로 고쳤습니다.

### 2. AUTOSAR 계층과 CAN 통신 — [AUTOSAR](AUTOSAR), [python-can](python-can)

- OS Task → SWC·RTE(Sender/Receiver, Client/Server) → IoHwAb·COM·NvM 순서로 한 계층씩 쌓은 뒤, TORCS 시뮬레이터의 CAN 신호를 CC·LKAS SWC에 연결해 통합했습니다.
- 제공된 CC 로직의 조건식을 정리해 보니 실제로는 성립하지 않는 가속 보정 분기가 있어, 원인을 실습 README에 분석해 두었습니다.
- python-can으로 가상 ECU를 만들어 DBC 메시지 17개를 주기 송신했습니다. 한 루프에서 차례로 보내면 주기가 밀리는 문제를 확인하고, 스레드와 `send_periodic()`으로 바꿔 메시지마다 자기 주기를 지키게 했습니다.

### 3. 차량 모델 기반 제어 설계 — [Simulink](Simulink)

- 슬립률과 Magic Formula 타이어로 종방향 모델을, 자전거 모델로 횡방향 모델을 직접 블록으로 만들고 Vehicle Dynamics Blockset 블록과 결과를 비교했습니다.
- ABS는 슬립 임계값 0.13이 노면 마찰 곡선의 최대점과 맞는지 확인했고, ACC는 등간격 제어에서 뒤차로 갈수록 간격 오차가 커지는 현상(string instability)을 등시간간격(CTG) 제어와 비교했습니다.
- 요레이트 측정값만으로 칼만 필터가 횡슬립각을 추정하게 했고, 모델에서 C 코드를 생성해 Arduino Uno에 10 ms 주기로 올렸습니다.

## 사용 기술

- **언어**: C, Python, MATLAB
- **하드웨어**: Infineon AURIX TC275, NXP MPC5606B, Raspberry Pi 5, Arduino Uno
- **도구**: AURIX Development Studio, Mobilgene Studio, CodeWarrior, MATLAB·Simulink·Simscape, Kvaser CanKing
- **통신**: CAN (DBC), UART
