# TORCS CAN 프레임·신호 구성

[Project.arxml](../../Configuration/System/DBImport/Project.arxml)의 CC·LKAS CAN 프레임입니다. 송수신 방향은 ECU 기준입니다.

| 프레임 트리거 | CAN ID | 방향 | 주요 신호 |
|---|---|---|---|
| `FT_CC_Recv1` | `0x7EF` | 수신 | `ACCEL_VALUE`, `TARGET_SPEED` |
| `FT_CC_Recv2` | `0x7FD` | 수신 | `SPEED`, `CC_TRIGGER` |
| `FT_CC_Send` | `0x7FF` | 송신 | `BRAKE`, `ACCEL` |
| `FT_LKAS_Recv` | `0x7FC` | 수신 | `STEER_VALUE`, `LKAS_TRIGGER` |
| `FT_LKAS_Send` | `0x7FE` | 송신 | `LEFT_STEER`, `RIGHT_STEER` |

TORCS에서 받은 입력을 COM·RTE로 전달하고, 100 ms 주기의 CC·LKAS Runnable에서 처리한 출력을 CAN 신호로 송신합니다.

모델에는 기본 플랫폼 프레임과 실습 프레임이 함께 포함되어 있습니다. 원본 DBC 파일은 포함하지 않았습니다.
