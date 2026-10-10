# AUTOSAR IoHwAb — ADC·DIO·PWM 연동

가변저항을 난방 강도 다이얼로 사용하고, LED로 동작 상태와 출력 강도를 표시하는 실습입니다. SWC에서 RTE 서비스를 호출해 ADC·DIO·PWM에 접근하도록 IoHwAb와 MCAL을 연결했습니다.

## 핀 구성

| 부품 | 핀 | 용도 |
|---|---|---|
| 가변저항 | PB4 | 난방 단계 입력, ADC |
| LED1 | PE4 | 승객 상태가 TRUE이면 ON, FALSE이면 OFF, DIO |
| LED2 | PE5 | 10 ms Runnable의 10회 실행마다 토글(100 ms 간격), DIO |
| LED4 | PE7 | 난방 강도 표시, PWM |

## SWC 구성

| SWC | 실행 조건 | 역할 |
|---|---|---|
| `Swc_HeatingDial` | 10 ms Timing Event | ADC 읽기, 단계 변환, 상태·Alive LED 갱신 |
| `Swc_SHControl` | `Setting.Write` 호출 | 승객 상태와 다이얼 값을 이용해 난방 명령 결정 |
| `SWC_SeatHeating` | `Heating.Write` 호출 | 난방 명령을 PWM 출력으로 변환 |

```text
ADC → HeatingDial → SHControl → SeatHeating → PWM
        ↑              │
        └─ 상태 LED 값 ─┘  (Queued Sender/Receiver)
```

난방 설정은 Client/Server 호출로 전달합니다. 상태 LED 값은 Queued S/R로 최대 4개까지 버퍼링하고, `Rte_Send/Rte_Receive`로 보내고 꺼내 처리하도록 구성했습니다.
이 단계의 `passenger`는 `TRUE`로 유지되어 첫 상태 수신 후 LED1이 켜집니다. CAN을 통한 승객 상태 갱신은 다음 실습에서 추가합니다.

## 입력값과 출력 단계

ADC는 10비트로 0~1023을 읽습니다. PWM Duty는 `전달값 / 0x8000 × 100%`로 환산하며, LED4의 PWM 활성 극성은 LOW입니다.

| ADC 값 | 난방 단계 | PWM API 전달값 |
|---|---:|---|
| 0 ~ 254 | 4 | `0x8000` — 100% |
| 255 ~ 510 | 3 | `0x4000` — 50% |
| 511 ~ 766 | 2 | `0x1500` — 약 16.41% |
| 767 ~ 1023 | 1 | `0x0800` — 6.25% |
| 난방 정지 명령 | 0 | `SetOutputToIdle()` |

## 이전 실습과 달라진 점

05의 스위치 감지·출력 두 SWC에서, 06은 다이얼 입력(HeatingDial)·난방 판단(SHControl)·PWM 출력(SeatHeating)의 세 SWC로 구성을 바꾸고 ADC·PWM 서비스를 추가했습니다.

## 주요 파일

| 파일 | 역할 |
|---|---|
| [HeatingDial.c](Static_Code/App_Code/HeatingDial.c) | ADC 입력·단계 변환·LED 제어 |
| [SHControl.c](Static_Code/App_Code/SHControl.c) | 난방 명령과 상태 LED 값 결정 |
| [SeatHeating.c](Static_Code/App_Code/SeatHeating.c) | PWM Duty·Idle 설정 |
| [App_SeatHeating.arxml](Configuration/System/Swcd_App/App_SeatHeating.arxml) | SWC·포트·인터페이스·Event |
| [Ecud_IoHwAb.arxml](Configuration/ECU/Ecud_IoHwAb.arxml) | 논리 IO 채널과 MCAL 연결 |
| [Swcd_IoHwAb.arxml](Generated/Bsw_Output/swcd/Swcd_IoHwAb.arxml) | RTE에서 호출하는 IO 서비스 모델 |

`Ecud_Rte.arxml`에서 10 ms Event를 Task에 매핑하고, `Build/generate.py`에 `App_SeatHeating`을 등록했습니다.
