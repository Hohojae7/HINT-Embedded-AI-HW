# 08. 횡방향 차량 모델링과 상태 추정

차량이 선회할 때의 움직임을 자전거 모델(앞·뒤 바퀴를 하나씩으로 합친 모델)로 만들고, 직접 재기 어려운 횡슬립각 $\beta$를 센서 값으로 추정한 실습입니다. 추정에는 50 km/h 슬라럼 시험 데이터([`EXPDATA.mat`](EXPDATA.mat))를 사용했습니다.

| 파일 | 내용 |
|---|---|
| [`Lat_kinematics_Simul.slx`](Lat_kinematics_Simul.slx) / [`_initialize.m`](Lat_kinematics_Simul_initialize.m) | 기구학 모델. 10 km/h에서 앞바퀴를 2~5초 동안 15°까지 꺾었을 때의 궤적 |
| [`Lat_Dynamics_Simul.slx`](Lat_Dynamics_Simul.slx) / [`_initialize.m`](Lat_Dynamics_Simul_initialize.m) | 동역학 모델. 직접 만든 모델과 `Vehicle Body 3DOF Single Track` 블록 비교 |
| [`Cornering_Stiffness_Tuning.m`](Cornering_Stiffness_Tuning.m) | 시험 데이터를 동역학 모델에 넣어 요레이트와 $\beta$를 측정값과 비교 |
| [`Tire_Lateral_Force_Estimation.slx`](Tire_Lateral_Force_Estimation.slx) | 횡가속도와 요레이트로 앞·뒤 타이어 횡력 추정 |
| [`Beta_Estimation_OpenLoop.slx`](Beta_Estimation_OpenLoop.slx) | 횡가속도를 적분해 $\beta$ 추정 (개루프) |
| [`Beta_Estimation_KalmanFilter.slx`](Beta_Estimation_KalmanFilter.slx) | 동역학 모델과 칼만 필터로 $\beta$ 추정 |
| [`State_Estimator.m`](State_Estimator.m) | 위 세 추정 모델을 차례로 실행 |

## 기구학 모델

저속에서는 타이어가 미끄러지지 않는다고 보고 기하 관계만으로 움직임을 구합니다. 후륜 조향각은 0으로 두었습니다.

```math
\beta = \tan^{-1}\!\left(\frac{l_r\tan\delta_f}{l_f + l_r}\right),
\qquad
\dot{\psi} = \frac{V\cos\beta\,\tan\delta_f}{l_f + l_r}
```

```math
\dot{X} = V\cos(\psi + \beta), \qquad \dot{Y} = V\sin(\psi + \beta)
```

직접 만든 서브시스템과 Robotics System Toolbox의 `Bicycle Kinematic Model` 블록(축거 3 m)에 같은 입력을 넣어 궤적을 비교했습니다.

## 동역학 모델

속도가 높아지면 타이어 횡력을 고려해야 합니다. 슬립각이 작을 때 횡력은 슬립각에 코너링 강성 $C$를 곱한 값으로 둡니다.

```math
mV_x(\dot{\beta} + \gamma) = F_{yf} + F_{yr}, \qquad I_z\dot{\gamma} = l_f F_{yf} - l_r F_{yr}
```

```math
F_{yf} = 2C_f\Big(\delta - \beta - \frac{l_f\gamma}{V_x}\Big), \qquad F_{yr} = 2C_r\Big(-\beta + \frac{l_r\gamma}{V_x}\Big)
```

$\beta$와 요레이트 $\gamma$를 상태로 두면 상태방정식이 됩니다.

```math
\dot{x} = Ax + Bu, \quad x = \begin{bmatrix}\beta \\ \gamma\end{bmatrix}, \quad u = \delta
```

```math
A = \begin{bmatrix}
-\dfrac{2C_f + 2C_r}{mV_x} & -1 + \dfrac{2C_r l_r - 2C_f l_f}{mV_x^2} \\[10pt]
\dfrac{2C_r l_r - 2C_f l_f}{I_z} & -\dfrac{2C_f l_f^2 + 2C_r l_r^2}{I_z V_x}
\end{bmatrix},
\qquad
B = \begin{bmatrix} \dfrac{2C_f}{mV_x} \\[10pt] \dfrac{2C_f l_f}{I_z} \end{bmatrix}
```

`Cornering_Stiffness_Tuning.m`은 $C_f$, $C_r$를 바꿔 가며 모델의 요레이트와 $\beta$가 시험 데이터에 맞는지 확인하는 스크립트입니다. 코너링 강성은 타이어, 하중, 노면에 따라 달라서 측정값에 맞춰 조정해야 합니다.

## 상태 추정

**타이어 횡력 (센서 기반)**: 횡방향 힘 평형과 요 모멘트 평형을 풀면 측정값만으로 횡력을 구할 수 있습니다. 요레이트를 미분하면 잡음이 커지므로 1차 저역통과 필터(시정수 0.06 s)를 함께 썼습니다.

```math
F_{yf} = \frac{m l_r a_y + I_z\dot{\gamma}}{l_f + l_r}, \qquad F_{yr} = \frac{m l_f a_y - I_z\dot{\gamma}}{l_f + l_r}
```

**$\beta$ 개루프 추정**: 횡가속도 식을 그대로 적분합니다. 센서 오차까지 함께 적분되어 시간이 갈수록 실제 값에서 벗어납니다.

```math
\beta = \int_0^t \left(\frac{a_y}{V_x} - \gamma\right) d\tau
```

**칼만 필터**: 요레이트 $\gamma$만 측정하고($C = [0\ 1]$), 모델 예측과 측정의 차이로 $\beta$까지 보정합니다.

```math
\dot{\hat{x}} = A\hat{x} + Bu + K(y - C\hat{x})
```

이득 $K$는 Kalman Filter 블록이 공정 잡음 $Q = 1$, 측정 잡음 $R = 100$으로 계산합니다. $R$이 클수록 측정을 덜 믿고 모델 예측을 더 따릅니다.

## 사용한 블록

| 블록 | 용도 |
|---|---|
| MATLAB Function | 기구학·동역학 모델의 $\beta$, 요레이트 계산식 |
| Integrator, Trigonometry, Product | 상태 적분, 위치 $(X, Y)$ 계산 |
| From Workspace | 조향각, 차속, 시험 데이터 입력 |
| Derivative, Transfer Fcn | 요레이트 미분과 저역통과 필터 |
| Bicycle Kinematic Model | Robotics System Toolbox 기구학 모델 |
| Vehicle Body 3DOF Single Track | Vehicle Dynamics Blockset 동역학 모델 |
| Kalman Filter | 연속 시간 칼만 필터 ($A, B, C$는 스크립트에서 계산) |
