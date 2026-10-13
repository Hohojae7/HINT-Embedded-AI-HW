# 03. PI 제어기 C 코드 생성

PI 제어기 모델을 Simulink Coder(`grt.tlc`)로 C 코드로 바꾼 결과물입니다. 모델 파일은 없고 생성된 코드만 보관했습니다.

## 제어기 구조

```math
u(t) = K_P\,e(t) + K_I\int_0^t e(\tau)\,d\tau
```

오차를 비례 Gain과 적분 Gain → Integrator 두 경로로 나눈 뒤 더하는 PI 서브시스템을 두 단으로 이었습니다. 각 단의 출력은 Delay(1스텝)를 거쳐 다시 오차 계산에 들어갑니다. 게인은 모두 1, 초기값은 0입니다.

| 항목 | 설정 |
|---|---|
| 타깃 | `grt.tlc`, x86-64 (Windows) |
| 솔버 | 고정 스텝 ODE3, 1 ms |
| 종료 시간 | 10 s |

## 생성된 파일

| 파일 | 역할 |
|---|---|
| [`untitled (C코드 생성실습_PID제어기.c`](<untitled (C코드 생성실습_PID제어기.c>) | 모델 본체. `untitled_initialize()`, `untitled_step()`, `untitled_derivatives()` |
| [`untitled.h`](untitled.h) | 신호(B), 연속 상태(X), 블록 상태(DW), 파라미터(P) 구조체와 함수 선언 |
| [`untitled_data.c`](untitled_data.c) | 게인과 초기값 같은 파라미터 값 |
| [`untitled_private.h`](untitled_private.h), [`untitled_types.h`](untitled_types.h) | 내부 매크로와 모델 타입 정의 |
| [`rtwtypes.h`](rtwtypes.h), [`builtin_typeid_types.h`](builtin_typeid_types.h), [`multiword_types.h`](multiword_types.h) | `real_T` 같은 Simulink 기본 타입 |
| [`rt_nonfinite.c`](rt_nonfinite.c), [`rt_nonfinite.h`](rt_nonfinite.h) | NaN, Inf 처리 |
| [`rtmodel.h`](rtmodel.h) | 실시간 모델 구조체 접근 매크로 |

## 코드에서 볼 점

- `untitled_step()`이 1 ms마다 한 번 불리는 함수입니다. 오차 계산, 비례 경로, Delay 갱신이 모두 이 안에 있습니다. 함수 자체는 주기를 모르고, 부르는 쪽이 주기를 맞춥니다.
- 적분기를 연속 블록(Integrator)으로 만들어서, `untitled_derivatives()`가 미분값을 내면 `rt_ertODEUpdateContinuousStates()` 안의 ODE3 솔버가 상태를 갱신합니다. 이산 적분기(Discrete-Time Integrator)를 쓰면 이 솔버 코드 없이 누적 합 한 줄로 바뀝니다.
- 파라미터가 `untitled_P` 구조체 하나에 모여 있어서, 게인을 바꿀 때 `untitled_data.c`만 고치면 됩니다.
