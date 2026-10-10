# AUTOSAR RTE — Timing Event

SWC에 Runnable과 Timing Event를 만들고 OS Task에 매핑하는 실습입니다. `SeatSwitch()`를 100 ms마다 실행하고 직접 IO API를 호출해 LED1(PE4)을 토글합니다. 이 단계에서는 스위치 입력 대신 0/1 값을 생성합니다.

## SWC·Event 설정

| 항목 | 설정 |
|---|---|
| SWC | `SWC_SeatSwitch` |
| Runnable 함수 | `SeatSwitch()` |
| Event | `TE_RE_SeatSwitch` |
| 주기 | `0.1 s` = 100 ms |
| 매핑 Task | `OsTask_ASW_FG1_100ms` |
| 사용 Alarm | `OsAlarm_ASW_100ms` |
| 출력 | LED1, PE4 |

Runnable은 SWC의 실행 단위이며, RTE는 설정된 Event와 OS Task 매핑에 따라 해당 함수를 실행합니다.

## 동작 흐름

```text
100 ms Alarm → OS Task → RTE → SeatSwitch() → 직접 IO 호출 → LED1(PE4)
```

`SeatSwitch()`는 `IoHwAb_DigDirWriteDirect(0, Passenger)`로 현재 값을 출력한 뒤, 다음 실행에 사용할 `Passenger` 값을 0/1로 바꿉니다. 채널 `0`은 LED1(PE4)에 연결되어 있습니다. LED 상태는 100 ms마다 바뀌며, 켜짐과 꺼짐을 합친 반복 주기는 약 200 ms입니다.
`Build/generate.py`의 GenerateRte 입력에는 새 모델인 `App_Rte`를 등록했습니다.

## 주요 파일

| 파일 | 역할 |
|---|---|
| [App_Rte.arxml](Configuration/System/Swcd_App/App_Rte.arxml) | SWC·Runnable·Timing Event 정의 |
| [Ecud_Rte.arxml](Configuration/ECU/Ecud_Rte.arxml) | Event–Task·Alarm 매핑 |
| [App_SWC_SeatSwitch.c](Static_Code/Reference_Code/App_SWC_SeatSwitch.c) | 100 ms마다 출력값을 바꾸는 Runnable |
| [generate.py](Build/generate.py) | `App_Rte` 생성 입력 등록 |
