# AUTOSAR IoHwAb — ADC·DIO·PWM 연동

가변저항을 난방 강도 다이얼로 사용하고, LED로 동작 상태와 출력 강도를 표시하는 실습입니다. SWC에서 RTE 서비스를 호출해 ADC·DIO·PWM에 접근하도록 IoHwAb와 MCAL을 연결했습니다.

## 핀 구성

| 부품 | 핀 | 용도 |
|---|---|---|
| 가변저항 | PB4 | 난방 단계 입력, ADC |
| LED1 | PE4 | 난방 동작 상태, DIO |
| LED2 | PE5 | Alive 표시, DIO |
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

난방 설정은 Client/Server 호출로 전달하고, 상태 LED 값은 `Rte_Send/Rte_Receive` 큐 통신으로 전달합니다. 이 단계의 `passenger` 초기값은 `TRUE`이며, CAN을 통한 승객 상태 갱신은 다음 실습에서 추가합니다.

## 입력값과 출력 단계

| ADC 값 | 난방 단계 | PWM API 전달값 |
|---|---:|---|
| 0 ~ 254 | 4 | `0x8000` |
| 255 ~ 510 | 3 | `0x4000` |
| 511 ~ 766 | 2 | `0x1500` |
| 767 이상 | 1 | `0x0800` |
| 난방 정지 명령 | 0 | `SetOutputToIdle()` |

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
