# AUTOSAR CAN — 승객 감지·난방 제어

IoHwAb 실습에 CAN 승객 감지 신호를 추가한 실습입니다. 수신한 값을 SeatSwitch에서 승객 상태로 전달하고, SHControl이 난방을 허용하거나 정지하도록 연결했습니다.

## CAN 구성

| 항목 | 설정 |
|---|---|
| 통신 속도 | 500 kbit/s |
| 수신 프레임 | `ECU2_Msg_PD`, ID `0x005` |
| 송신 프레임 | `ECU1_Msg_SH`, ID `0x004` |
| 수신 값 | `Sig1`: 0이면 승객 없음, 0이 아니면 승객 감지 |
| 송신 값 | 수신한 `Sig1` 값을 상태 신호에 기록 |

## 동작 흐름

```text
CAN RX → COM·RTE → SeatSwitch → PassengerDetected → SHControl → PWM
                      └─ 수신 값 → CAN 상태 송신
```

CAN 수신 Data Received Event로 `Re_SeatSwitch()`를 실행합니다. `Re_SHSwitch()`는 승객 상태를 읽고, 기존 다이얼 처리 Runnable은 이 상태에 따라 난방 단계 또는 0을 전달합니다. 두 수신 Event는 `OsTask_ASW_DRE`에 매핑되어 있습니다.

| `0x005` 첫 바이트 | LED1(PE4) | LED4(PE7) | LED2(PE5) |
|---|---|---|---|
| `0` | OFF | 난방 정지, Idle | Alive 표시 유지 |
| 0 이외 | ON | 다이얼 단계에 따라 PWM 변경 | Alive 표시 유지 |

## 이전 실습과 달라진 점

`SeatSwitch.c`와 `SHControl.c`의 `Re_SHSwitch()`를 추가했습니다. HeatingDial과 SeatHeating의 ADC·PWM 구현은 유지하고, CAN 신호를 처리하는 SWC·Event·COM 연결을 확장했습니다.

## 주요 파일

| 파일 | 역할 |
|---|---|
| [SeatSwitch.c](Static_Code/App_Code/SeatSwitch.c) | CAN 수신 값 읽기, 승객 상태·송신 신호 작성 |
| [SHControl.c](Static_Code/App_Code/SHControl.c) | 승객 상태 수신과 난방 허용 판단 |
| [App_SeatHeating.arxml](Configuration/System/Swcd_App/App_SeatHeating.arxml) | 추가 SWC·포트·수신 Event |
| [Project.arxml](Configuration/System/DBImport/Project.arxml) | 실습 CAN 프레임·신호 Import 모델 |
| [Ecud_Com.arxml](Configuration/ECU/Ecud_Com.arxml) | COM 신호 설정 |

CAN 프레임과 신호 구성은 `Configuration/System/DBImport/Project.arxml`에 정의되어 있습니다. [CAN 신호 설명](References/DB/README.md)에 프레임별 송수신 방향과 역할을 정리했습니다.
