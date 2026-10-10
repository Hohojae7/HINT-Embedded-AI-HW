# AUTOSAR OS — 주기 Task

Counter와 Alarm으로 Task를 주기적으로 활성화하는 실습입니다. LED 초기화가 끝난 뒤 `OsTask_Test_1s`가 1초 간격으로 실행되며 LED2(PE5) 상태를 전환합니다.

## Task·Alarm 설정

| 항목 | 설정 |
|---|---|
| Task | `OsTask_Test_1s` |
| Activation / Priority | `1` / `2` |
| Schedule | `FULL` — 선점 가능 |
| Alarm | `OsAlarm_Test_1s` |
| Counter | `OsCounter_0` — STM 하드웨어 타이머 채널 0, 1 tick = 1 µs |
| Autostart | `RELATIVE`, `OsAppMode0` |
| 시작값 / 반복값 | `500000` ticks = 0.5 s / `1000000` ticks = 1 s |
| Alarm Action | `OsTask_Test_1s` 활성화 |

## 동작 흐름

```text
OS Counter → Alarm 만료 → OsTask_Test_1s
                       → 초기화 상태 확인 → LED2 상태 전환 → Task 종료
```

Task는 `GblLedInit`가 참일 때 LED2 출력을 바꾸고 `TerminateTask()`로 종료합니다. Task 실행 간격은 1초이며, 켜짐과 꺼짐을 합친 반복 주기는 약 2초입니다.

## 이전 실습과 달라진 점

`App_Os.c`는 01과 같습니다. `Ecud_Os.arxml`에 주기 Task와 Alarm을 추가하고, 두 객체를 `OsApplication0`에 등록했습니다. OS-Application은 OS 객체의 소유와 접근 권한을 관리하는 단위입니다.

## 주요 파일

| 파일 | 역할 |
|---|---|
| [Ecud_Os.arxml](Configuration/ECU/Ecud_Os.arxml) | Counter·Alarm·Task 설정 |
| [App_Os.c](Static_Code/Reference_Code/App_Os.c) | 초기화 상태 확인과 LED 토글 |
| [시작 코드 연결](Static_Code/Integration_Code/README.md) | LED 초기화 Task 활성화 |
