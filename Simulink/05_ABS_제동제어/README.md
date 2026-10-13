# 05. ABS 제동 제어

급제동 때 바퀴가 잠기지 않도록, 슬립률이 임계값을 넘으면 제동압을 풀었다가 다시 거는 ABS를 만든 실습입니다. 04의 `Long_Simul_3` 차량 모델에 ABS 서브시스템을 더하고, 같은 시나리오를 ABS를 끈 경우와 켠 경우로 두 번 돌려 비교했습니다.

| 파일 | 내용 |
|---|---|
| [`ABS_Simul.slx`](ABS_Simul.slx) | 차량 모델 + `ABS system` 서브시스템 |
| [`ABS_Simul_Initialize.m`](ABS_Simul_Initialize.m) | `ABS_ONOFF`를 0과 1로 두 번 시뮬레이션하고 속도, 구동력, 슬립률을 겹쳐 그림 |

## 원리

제동력은 슬립률이 커질수록 늘다가 최대 마찰점을 지나면 오히려 줄어듭니다. 바퀴가 완전히 잠기면($\sigma = -1$) 제동력이 떨어지고 조향도 되지 않습니다.

```math
\mu(\sigma) = D\sin\!\Big(C\arctan\big(B\sigma - E(B\sigma - \arctan B\sigma)\big)\Big)
```

이 실습은 조금 더 미끄러운 노면을 가정해 $C = 1.65 \times 1.05$로 두었습니다($B = 10,\ D = 1,\ E = 0.01$). 이 곡선에서 $\mu$는 $|\sigma| \approx 0.13$에서 최대(1.0)이고, 바퀴가 잠기면 약 0.56으로 떨어집니다. 그래서 임계 슬립을 `Slip_thres = 0.13`으로 잡았습니다.

## ABS 로직

```math
P_{brake} =
\begin{cases}
0 & \text{ABS on and } |\sigma| > 0.13 \\
P_{cmd} & \text{otherwise}
\end{cases}
```

슬립이 임계값을 넘으면 제동압을 0으로 풀고, 바퀴가 다시 돌아 슬립이 줄면 제동압을 겁니다. 이 과정이 반복되면서 슬립이 최대 마찰점 근처에 머뭅니다. 밸브 응답은 1차 지연 $1/(0.05s + 1)$로 넣었습니다.

시나리오는 0~4초 스로틀 30%, 8초부터 제동압 5×10⁶ Pa입니다. 스크립트에서 완전 제동(1×10⁷ Pa)으로 바꿔 비교할 수 있습니다.

## 사용한 블록

| 블록 | 용도 |
|---|---|
| Abs, Relational Operator, Logical Operator | $\lvert\sigma\rvert$ > 임계값이고 ABS가 켜졌는지 판단 |
| Switch, Constant | 조건에 따라 제동압을 그대로 넘기거나 0으로 |
| Transfer Fcn | 브레이크 밸브 지연 |
| Simple Engine, Longitudinal Wheel, Vehicle Body 1DOF Longitudinal | 04의 차량 모델 |
| Memory | 휠 속도·슬립 되먹임 경로의 대수 루프를 끊음 |
