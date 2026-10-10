# AUTOSAR 응용 — TORCS CC·LKAS 연동

TORCS의 CAN 신호를 CC·LKAS SWC에 연결하는 실습입니다. 제공된 제어 예제를 바탕으로 SWC·포트·Event를 구성하고, ECU의 RTE·OS Task·COM 설정과 통합했습니다.

```text
TORCS (PC, 제공 runtime) ↔ PCAN-USB ↔ TRK-MPC5606B   CAN 500 kbit/s
```

TORCS 폴더에 제공 runtime을 설치하고, PCAN을 연결한 뒤 `execute.bat`를 관리자 권한으로 실행합니다.

## SWC 구성

| SWC | Runnable 함수 | 주기 | 입력 | 출력 |
|---|---|---|---|---|
| `App_CC` | `Runnable_CC_100ms()` | 100 ms | 가속 입력, 목표 속도, 현재 속도, CC 트리거 | 가속·제동 신호 |
| `App_LKAS` | `Runnable_LKAS_100ms()` | 100 ms | 조향 입력, LKAS 트리거 | 좌·우 조향 신호 |

두 Timing Event는 `OsTask_ASW_FG1_100ms`에 매핑했습니다. `Build/generate.py`의 생성 입력에도 `App_CC`, `App_LKAS`를 등록했습니다.

## CAN 신호 구성

송수신 방향은 ECU 기준입니다.

| 프레임 | CAN ID | 방향 | 주요 신호 |
|---|---|---|---|
| CC Recv1 | `0x7EF` | 수신 | `ACCEL_VALUE`, `TARGET_SPEED` |
| CC Recv2 | `0x7FD` | 수신 | `SPEED`, `CC_TRIGGER` |
| CC Send | `0x7FF` | 송신 | `BRAKE`, `ACCEL` |
| LKAS Recv | `0x7FC` | 수신 | `STEER_VALUE`, `LKAS_TRIGGER` |
| LKAS Send | `0x7FE` | 송신 | `LEFT_STEER`, `RIGHT_STEER` |

## 동작 흐름

```text
TORCS CAN 입력 → COM·RTE → CC / LKAS Runnable → RTE·COM → TORCS CAN 출력
```

- **CC**: `CC_TRIGGER > 5000`이면 입력 가속값(`ACCEL_VALUE`)에 속도 조건에 따른 보정을 더해 `ACCEL`로 출력하고, 비활성이면 `ACCEL = 0`입니다. 제공 코드의 `BRAKE`는 항상 0입니다.

  ```c
  ACCEL = ACCEL_VALUE;
  if (SPEED * 100 - TARGET_SPEED < -TARGET_SPEED * 100)  ACCEL += 500;
  else if (SPEED * 100 > TARGET_SPEED)                   ACCEL -= 1000;
  ```

- **LKAS**: 활성 트리거와 조향 입력의 부호에 따라 좌·우 출력값을 정하고, 출력 크기를 최대 10000으로 제한합니다.

이 실습은 제공 제어 코드와 AUTOSAR 구성의 연결을 다룹니다.

## 주요 파일

| 파일 | 역할 |
|---|---|
| [App_CC.c](App_CC.c) | 제공 CC Runnable 구현 |
| [App_LKAS.c](App_LKAS.c) | 제공 LKAS Runnable 구현 |
| [App_CC.arxml](Configuration/System/Swcd_App/App_CC.arxml) | CC SWC·포트·Timing Event |
| [App_LKAS.arxml](Configuration/System/Swcd_App/App_LKAS.arxml) | LKAS SWC·포트·Timing Event |
| [Ecud_Rte.arxml](Configuration/ECU/Ecud_Rte.arxml) | Event–Task 매핑 |
| [Project.arxml](Configuration/System/DBImport/Project.arxml) | TORCS CAN 프레임·신호 모델 |

CAN 프레임과 신호 구성은 `Configuration/System/DBImport/Project.arxml`에 정의되어 있습니다. [CAN 신호 설명](References/DB/README.md)에 송수신 구성을 정리했습니다. 제공 C 파일의 기존 작성자·권리 표기는 유지했습니다.
