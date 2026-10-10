# AUTOSAR IoHwAb — ADC·DIO·PWM 연동

가변저항을 열선시트 강도 다이얼로 사용하고, LED로 동작 상태와 출력 강도를 표시하는 실습입니다. SWC에서 RTE 서비스를 호출해 ADC·DIO·PWM에 접근하도록 IoHwAb와 MCAL을 연결했습니다.

## 핀 구성

| 부품 | 핀 | 용도 |
|---|---|---|
| 가변저항 | PB4 | 열선시트 단계 입력, ADC |
| LED1 | PE4 | 열선시트 동작 상태, 승객 감지 시 ON, DIO |
| LED2 | PE5 | Alive 표시, 100 ms마다 토글, DIO |
| LED4 | PE7 | 열선시트 강도 표시, PWM |

## SWC 구성

| SWC | 실행 조건 | 역할 |
|---|---|---|
| `Swc_HeatingDial` | 10 ms Timing Event | ADC 읽기, 단계 변환, 상태·Alive LED 갱신 |
| `Swc_SHControl` | `Setting.Write` 호출 | 승객 상태와 다이얼 값으로 열선시트 단계 결정 |
| `SWC_SeatHeating` | `Heating.Write` 호출 | 열선시트 단계를 PWM 출력으로 변환 |

```text
ADC → HeatingDial → SHControl → SeatHeating → PWM
        ↑              │
        └─ 상태 LED 값 ─┘  (Queued Sender/Receiver)
```

열선시트 단계는 Client/Server 호출로 전달하고, 상태 LED 값은 Queued S/R(큐 길이 4)로 `Rte_Send`/`Rte_Receive`해 전달합니다.
이 단계에서는 `passenger`가 항상 `TRUE`라 LED1이 켜진 상태로 유지되며, CAN으로 승객 상태를 바꾸는 부분은 07에서 추가합니다.

## 입력값과 출력 단계

ADC는 10비트로 0~1023을 읽습니다. PWM Duty는 `전달값 / 0x8000 × 100%`로 환산하며, LED4의 PWM 활성 극성은 LOW입니다.

| ADC 값 | 열선시트 단계 | PWM API 전달값 |
|---|---:|---|
| 0 ~ 254 | 4 | `0x8000` — 100% |
| 255 ~ 510 | 3 | `0x4000` — 50% |
| 511 ~ 766 | 2 | `0x1500` — 약 16.41% |
| 767 ~ 1023 | 1 | `0x0800` — 6.25% |
| 승객 없음 (07) | 0 | `SetOutputToIdle()` |

## 이전 실습과 달라진 점

05의 스위치 입력·LED 출력 두 SWC 구조를 다이얼 입력(HeatingDial), 열선시트 제어(SHControl), PWM 출력(SeatHeating)의 세 SWC로 바꾸고 ADC·PWM 서비스를 추가했습니다.

## 주요 파일

| 파일 | 역할 |
|---|---|
| [HeatingDial.c](Static_Code/App_Code/HeatingDial.c) | ADC 입력·단계 변환·LED 제어 |
| [SHControl.c](Static_Code/App_Code/SHControl.c) | 열선시트 단계와 상태 LED 값 결정 |
| [SeatHeating.c](Static_Code/App_Code/SeatHeating.c) | PWM Duty·Idle 설정 |
| [App_SeatHeating.arxml](Configuration/System/Swcd_App/App_SeatHeating.arxml) | SWC·포트·인터페이스·Event |
| [Ecud_IoHwAb.arxml](Configuration/ECU/Ecud_IoHwAb.arxml) | 논리 IO 채널과 MCAL 연결 |
| [Swcd_IoHwAb.arxml](Generated/Bsw_Output/swcd/Swcd_IoHwAb.arxml) | RTE에서 호출하는 IO 서비스 모델 |

`Ecud_Rte.arxml`에서 10 ms Event를 Task에 매핑하고, `Build/generate.py`에 `App_SeatHeating`을 등록했습니다.
