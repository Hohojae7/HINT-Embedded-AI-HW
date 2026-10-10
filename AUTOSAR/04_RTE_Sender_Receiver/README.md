# AUTOSAR RTE — Sender/Receiver

SeatSwitch가 만든 승객 감지 값을 SeatHeatingControl로 전달하는 실습입니다. 두 SWC 사이에 Sender/Receiver 포트를 연결하고, 데이터 수신 Event로 출력 제어 Runnable을 실행합니다.

## SWC 구성

| SWC | 실행 조건 | 역할 |
|---|---|---|
| `SWC_SeatSwitch` | 100 ms Timing Event | 0/1 값을 만들어 `PassengerDetected` 송신 |
| `SWC_SeatHeatingControl` | Data Received Event | 승객 감지 값을 읽고 디지털 출력 제어 |

## 데이터 전달

| 처리 | RTE API |
|---|---|
| 송신 | `Rte_Write_P_SeatSwitch_PassengerDetected()` |
| 수신 | `Rte_Read_R_SeatSwitch_PassengerDetected()` |

```text
SeatSwitch → RTE Sender/Receiver → SeatHeatingControl → 디지털 출력
```

송신 Runnable은 100 ms Task에, 수신 Event는 저장된 설정의 `OsTask_BSW_AppModeRequest`에 매핑되어 있습니다. 수신한 값이 참이면 `IOHWAB_HIGH`, 거짓이면 `IOHWAB_LOW`를 직접 IO API에 전달합니다.

## 이전 실습과 달라진 점

03에서는 SeatSwitch가 직접 출력을 바꿨습니다. 이 단계에서는 SeatSwitch는 데이터를 송신하고, SeatHeatingControl이 데이터를 읽어 출력하는 두 SWC로 역할을 나눴습니다.

## 주요 파일

| 파일 | 역할 |
|---|---|
| [App_Rte.arxml](Configuration/System/Swcd_App/App_Rte.arxml) | S/R 인터페이스·포트·수신 Event |
| [EcuExtract.arxml](Configuration/System/Composition/EcuExtract.arxml) | SWC 인스턴스와 연결 |
| [Ecud_Rte.arxml](Configuration/ECU/Ecud_Rte.arxml) | 두 Event의 Task 매핑 |
| [App_SWC_SeatSwitch.c](Static_Code/Reference_Code/App_SWC_SeatSwitch.c) | 데이터 생성과 송신 |
| [App_SWC_SeatHeatingControl.c](Static_Code/Reference_Code/App_SWC_SeatHeatingControl.c) | 데이터 수신과 출력 제어 |
