# CAN DB 입력과 실제 Import 모델

이 실습의 CAN 설정은 [Configuration/System/DBImport/Project.arxml](../../Configuration/System/DBImport/Project.arxml)에 보존했습니다. 아래 표는 이 저장된 모델에서 확인한 실습 프레임입니다. 기본 플랫폼 프레임도 같은 모델 안에 함께 존재합니다.

| 프레임 트리거 | CAN ID (16진수) | CAN ID (10진수) |
|---|---|---|
| `FT_Project_ECU1_Msg_SH` | `0x004` | 4 |
| `FT_Project_ECU2_Msg_PD` | `0x005` | 5 |

원본 워크스페이스의 `References/DB/Project.dbc`는 이름이 같지만 기본 플랫폼 예제용이며 위 실습 프레임·신호가 없습니다. 실제 Import 결과와 다른 DBC를 원본 입력인 것처럼 게시하지 않도록 복사에서 제외했습니다.

백업에서 위 실습에 대응하는 별도의 원본 DBC는 확인되지 않았습니다. 강의의 DBC Import 과정은 모델로 확인할 수 있지만, 원본 DBC 자체까지 보존된 스냅샷은 아닙니다. 추측해서 새 DBC를 만들지 않았습니다.
