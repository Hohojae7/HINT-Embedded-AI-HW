# 07. ACC 차간 거리 제어

앞차와 거리를 유지하는 ACC(Adaptive Cruise Control)를 만든 실습입니다. 앞차와 멀면 06의 크루즈 제어로 120 km/h를 따라가고, 거리가 500 m 안으로 들어오면 간격 제어로 바뀝니다. 간격 제어는 등간격과 등시간간격(CTG) 두 방식을 비교했습니다.

| 모델 | 스크립트 | 조건 |
|---|---|---|
| [`ACC_Space_Single_Simul.slx`](ACC_Space_Single_Simul.slx) | [`ACC_Space_Single_Simul_Initialize.m`](ACC_Space_Single_Simul_Initialize.m) | 차 1대, 등간격. 앞차 100 km/h 일정, 목표 간격 300 m |
| [`ACC_Space_Multi_Simul.slx`](ACC_Space_Multi_Simul.slx) | [`ACC_Space_multi_Simul_Initialize.m`](ACC_Space_multi_Simul_Initialize.m) | 차 2대가 줄지어 따라감, 등간격. 앞차 속도 $100 + 10\sin(0.1t)$ km/h |
| [`ACC_CTG_Multi_Simul.slx`](ACC_CTG_Multi_Simul.slx) | [`ACC_CTG_Multi_Simul_Initialize.m`](ACC_CTG_Multi_Simul_Initialize.m) | 같은 조건에서 등시간간격 |

세 모델 모두 Switch로 거리 500 m를 기준으로 크루즈와 간격 제어를 바꾸고, 목표 가속도에 질량을 곱한 구동력을 파워트레인 지연 $1/(\tau s + 1)$, $\tau = 0.5$ s를 거쳐 차체에 넣습니다.

## 등간격 제어

앞차와의 거리를 $L_{des}$로 일정하게 유지하는 PD 제어입니다.

```math
\delta_i = x_i - x_{i-1} + L_{des}, \qquad \ddot{x}_{i,des} = -k_p\,\delta_i - k_v\,\dot{\delta}_i
```

앞차 속도가 일정하면 $k_p, k_v > 0$에서 간격 오차 $\delta_i$가 0으로 갑니다. 하지만 앞차가 감속·가속을 반복할 때 뒤차로 전달되는 간격 오차의 비율은 아래와 같고, 저주파($\omega^2 < 2k_p$)에서 항상 $|H| > 1$입니다. 그래서 뒤차로 갈수록 간격 오차가 커집니다(string instability).

```math
H(s) = \frac{\delta_i}{\delta_{i-1}} = \frac{k_v s + k_p}{s^2 + k_v s + k_p}
```

## 등시간간격 (CTG) 제어

목표 간격을 속도에 비례하게(시간 간격 $h$) 잡습니다.

```math
\delta_i = \varepsilon_i + h\,\dot{x}_i, \quad \varepsilon_i = x_i - x_{i-1} + l_{i-1},
\qquad
\ddot{x}_{i,des} = -\frac{1}{h}\big(\dot{\varepsilon}_i + \lambda\,\delta_i\big)
```

```math
H(s) = \frac{s + \lambda}{h\tau s^3 + h s^2 + (1 + \lambda h)s + \lambda}
```

$h \ge 2\tau$이면 모든 주파수에서 $|H| \le 1$이 되어 간격 오차가 뒤로 갈수록 줄어듭니다. 이 실습은 $h = 10$ s, $\lambda = 1$, $\tau = 0.5$ s로 조건을 만족하고, 스크립트가 `bode(H)`로 크기를 보여 줍니다.

## 사용한 블록

| 블록 | 용도 |
|---|---|
| Compare To Constant, Switch | 거리 500 m 기준 크루즈 ↔ 간격 제어 전환 |
| Derivative, Gain, Sum | PD 제어기, CTG 제어기 |
| Integrator | 앞차 속도를 적분해 위치 계산 (`Heading Car`) |
| From Workspace | 시간에 따라 변하는 앞차 속도 입력 |
| Transfer Fcn | 파워트레인 지연 |
| Vehicle Body 1DOF Longitudinal | 뒤따르는 차량 |
