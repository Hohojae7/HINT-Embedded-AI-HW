# CAN 프레임·신호 구성

[Project.arxml](../../Configuration/System/DBImport/Project.arxml)의 실습 CAN 프레임입니다. 송수신 방향은 ECU 기준입니다.

| 프레임 | CAN ID | 방향 | 신호·역할 |
|---|---|---|---|
| `ECU2_Msg_PD` | `0x005` | 수신 | `Sig1`: 0이면 승객 없음, 0 이외이면 승객 감지 |
| `ECU1_Msg_SH` | `0x004` | 송신 | `Sig1`: 수신한 승객 감지 값을 상태 신호로 송신 |

`SeatSwitch`는 수신 신호를 읽어 `PassengerDetected`로 전달하고, 같은 값을 송신 신호에 기록합니다. `SHControl`은 승객 상태에 따라 난방을 허용하거나 정지합니다.

모델에는 기본 플랫폼 프레임과 실습 프레임이 함께 포함되어 있습니다. 원본 DBC 파일은 포함하지 않았습니다.
