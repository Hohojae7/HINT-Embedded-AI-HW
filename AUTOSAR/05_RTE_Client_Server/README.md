# AUTOSAR RTE — Client/Server

디지털 입력과 출력에 Client/Server 인터페이스를 연결하는 실습입니다. SeatSwitch는 IO 서비스를 호출해 입력을 읽고, SeatHeatingControl은 IO 서비스를 호출해 출력을 변경합니다.

## 인터페이스 구성

| 연결 | 방식 | 역할 |
|---|---|---|
| SeatSwitch → IO 서비스 | Client/Server | 디지털 입력 읽기 |
| SeatSwitch → SeatHeatingControl | Sender/Receiver | `PassengerDetected` 값 전달 |
| SeatHeatingControl → IO 서비스 | Client/Server | 디지털 출력 쓰기 |

Sender/Receiver는 데이터를 전달하고, Client/Server는 Operation을 호출하는 방식입니다.

## 동작 흐름

```text
디지털 입력 → SeatSwitch → S/R 데이터 전달 → SeatHeatingControl → 디지털 출력
               Rte_Call                          Rte_Call
```

- 입력: `Rte_Call_R_IO_ReadDirect(&Passenger)`
- 데이터 전달: `Rte_Write/Read_*_PassengerDetected()`
- 출력: `Rte_Call_R_HeatingElement_WriteDirect()`

SeatSwitch는 100 ms Timing Event로 실행되고, SeatHeatingControl은 데이터 수신 Event로 실행됩니다.

## 이전 실습과 달라진 점

04의 0/1 생성 코드를 디지털 입력 읽기로 바꿨습니다. 출력 쪽도 `IoHwAb_DigDirWriteDirect()` 직접 호출에서 `Rte_Call_*` 호출로 변경했습니다. 두 SWC 사이의 Sender/Receiver 연결은 유지합니다.

## 주요 파일

| 파일 | 역할 |
|---|---|
| [App_Rte.arxml](Configuration/System/Swcd_App/App_Rte.arxml) | C/S 포트·Operation과 S/R 연결 정의 |
| [EcuExtract.arxml](Configuration/System/Composition/EcuExtract.arxml) | Application과 IO 서비스 포트 연결 |
| [Swcd_IoHwAb.arxml](Generated/Bsw_Output/swcd/Swcd_IoHwAb.arxml) | 제공 IO 서비스의 인터페이스 |
| [App_SWC_SeatSwitch.c](Static_Code/Reference_Code/App_SWC_SeatSwitch.c) | 디지털 입력 호출과 감지 값 송신 |
| [App_SWC_SeatHeatingControl.c](Static_Code/Reference_Code/App_SWC_SeatHeatingControl.c) | 수신 값에 따른 디지털 출력 호출 |
