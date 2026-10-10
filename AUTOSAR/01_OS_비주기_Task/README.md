# AUTOSAR OS — 비주기 Task

시스템 시작 경로에서 OS Task를 한 번 활성화해 LED2를 초기화하는 실습입니다. LED2를 끄고 500 ms 뒤 켠 다음, 초기화 상태를 기록하고 Task를 종료합니다.

## Task 설정

| 항목 | 설정 |
|---|---|
| Task | `OsTask_LED_Init` |
| Activation / Priority | `1` / `1` |
| Schedule | `FULL` — 선점 가능 |
| 활성화 | 시작 코드에서 `ActivateTask(OsTask_LED_Init)` 호출 |
| 종료 | `TerminateTask()` |
| LED | LED2, PE5, Low = ON |

## 동작 흐름

```text
OsTask_BSW_Init → EcuM_StartupTwo()
               → ActivateTask(OsTask_LED_Init)
               → LED2 OFF → 500 ms 지연 → LED2 ON → Task 종료
```

`DelayMS()`는 OS Counter 값을 반복해서 확인하는 busy-wait 방식입니다. 초기화가 끝나면 `GblLedInit = TRUE`를 기록합니다.

## 주요 파일

| 파일 | 역할 |
|---|---|
| [Ecud_Os.arxml](Configuration/ECU/Ecud_Os.arxml) | Task 속성과 OS-Application 연결 |
| [App_Os.c](Static_Code/Reference_Code/App_Os.c) | LED 초기화 Task, Counter 기반 지연 함수 |
| [시작 코드 연결](Static_Code/Integration_Code/README.md) | EcuM 시작 경로의 Task 활성화 위치 |

`App_Os.c`에는 다음 실습의 주기 Task 함수도 들어 있지만, 이 단계의 OS 설정에는 해당 Task와 Alarm이 등록되어 있지 않습니다.
