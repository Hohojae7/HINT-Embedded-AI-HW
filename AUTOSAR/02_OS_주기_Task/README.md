# AUTOSAR OS — 주기 Task

Counter와 Alarm으로 Task를 주기적으로 활성화하는 실습입니다. LED 초기화가 끝난 뒤 `OsTask_Test_1s`가 1초 간격으로 실행되며 LED2(PE5) 상태를 전환합니다.

## Task·Alarm 설정

| 항목 | 설정 |
|---|---|
| Task | `OsTask_Test_1s` |
| Activation / Priority | `1` / `2` |
| Schedule | `FULL` — 선점 가능 |
| Alarm | `OsAlarm_Test_1s` |
| Counter | `OsCounter_0` |
| Autostart | `RELATIVE`, `OsAppMode0` |
| 시작값 / 반복값 | `500000` / `1000000` ticks |
| Alarm Action | `OsTask_Test_1s` 활성화 |

## 동작 흐름

```text
OS Counter → Alarm 만료 → OsTask_Test_1s
                       → 초기화 상태 확인 → LED2 상태 전환 → Task 종료
```

Task는 `GblLedInit`가 참일 때 LED2 출력을 바꾸고 `TerminateTask()`로 종료합니다. Task 실행 간격은 1초이며, 켜짐과 꺼짐을 합친 반복 주기는 약 2초입니다.

## 이전 실습과 달라진 점

01과 `App_Os.c`는 동일합니다. 이 단계에서 달라지는 핵심은 `Ecud_Os.arxml`의 주기 Task·Alarm 등록과 OS-Application 연결입니다.

## 주요 파일

| 파일 | 역할 |
|---|---|
| [Ecud_Os.arxml](Configuration/ECU/Ecud_Os.arxml) | Counter·Alarm·Task 설정 |
| [App_Os.c](Static_Code/Reference_Code/App_Os.c) | 초기화 상태 확인과 LED 토글 |
| [시작 코드 연결](Static_Code/Integration_Code/README.md) | LED 초기화 Task 활성화 |
