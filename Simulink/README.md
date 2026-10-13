# Simulink — 모델 기반 제어 실습

MATLAB/Simulink로 물리 시스템을 블록선도로 옮기고, 그 모델 위에서 차량 제어기와 상태 추정기를 설계한 실습입니다. 미분방정식과 Simscape 물리 모델에서 시작해 종·횡방향 차량 동역학, ABS·크루즈·ACC 제어, 칼만 필터를 다루고, 마지막에는 모델에서 C 코드를 생성해 Arduino에 올렸습니다.

## 실습 목록

| No | 폴더 | 내용 | 핵심 개념 |
|---|---|---|---|
| 01 | [동적 시스템 모델링](01_Simulink_동적시스템_모델링) | 적분, 1차·2차 미분방정식을 블록으로 구성하고 파라미터를 바꿔 반복 실행 | Integrator 피드백, `sim()` |
| 02 | [Simscape 기어·DC 모터](02_Simscape_기어_회전계_DC모터) | 기어로 연결된 회전계와 DC 모터를 물리 블록으로 모델링 | 등가 관성, 전기·기계 결합 |
| 03 | [PI 제어기 C 코드 생성](03_PI제어기_C코드생성) | PI 제어기 모델에서 생성한 C 코드 | Simulink Coder, `model_step()` |
| 04 | [종방향 차량 동역학](04_종방향_차량동역학) | 가속·제동 시 차속, 휠속, 슬립률을 계산하는 차량 모델 | 슬립률, Magic Formula |
| 05 | [ABS](05_ABS_제동제어) | 슬립률이 임계값을 넘으면 제동압을 풀어 바퀴 잠김 방지 | 최대 마찰 슬립 |
| 06 | [크루즈 컨트롤](06_크루즈_속도제어) | 목표 속도를 따라가는 PI 속도 제어 | 상위·하위 제어기, 피드포워드 |
| 07 | [ACC](07_ACC_차간거리제어) | 앞차와 간격을 유지하는 제어, 등간격과 등시간간격 비교 | String stability |
| 08 | [횡방향 모델링과 상태 추정](08_횡방향_모델링_상태추정) | 자전거 모델로 선회를 모델링하고 횡슬립각 추정 | Bicycle model, 칼만 필터 |
| 09 | [Arduino 배포](09_Arduino_Blockset) | 가변저항으로 서보 2개를 움직이는 모델을 Arduino Uno에 배포 | 코드 생성, 10 ms 주기 |

## 주요 수식

### 2차 동적 시스템 (01, 02)

질량-스프링-댐퍼와 DC 모터는 모두 2차 시스템입니다. 감쇠와 강성(모터는 저항, 관성, 역기전력 상수)이 응답이 진동할지, 천천히 수렴할지를 정합니다.

```math
m\ddot{y} + c\dot{y} + ky = F
\qquad\qquad
\frac{\omega(s)}{V(s)} = \frac{K}{(Ls + R)(Js + B) + K^2}
```

### 종방향 차량 (04~07)

노면 경사가 없을 때 차체는 타이어 구동력에서 공기저항과 구름저항을 뺀 힘으로 움직입니다.

```math
m\ddot{x} = F_x - F_{aero} - R_x,
\qquad
F_{aero} = \tfrac{1}{2}\rho C_d A_F V_x^2,
\qquad
R_x = f\,F_z
```

타이어 구동·제동력은 바퀴 회전 속도와 차속의 차이인 슬립률로 정해지고, 슬립이 커지면 Magic Formula처럼 비선형이 됩니다.

```math
\sigma_x = \frac{r_{eff}\,\omega - V_x}{\max(r_{eff}\,\omega,\ V_x)},
\qquad
F_x = F_z\,D\sin\!\Big(C\arctan\big(B\sigma_x - E(B\sigma_x - \arctan B\sigma_x)\big)\Big)
```

### 횡방향 차량 (08)

앞·뒤 바퀴를 하나씩으로 합친 자전거 모델입니다. 슬립각이 작으면 타이어 횡력은 슬립각에 비례합니다.

```math
mV_x(\dot\beta + \gamma) = F_{yf} + F_{yr},
\qquad
I_z\dot\gamma = l_f F_{yf} - l_r F_{yr}
```

```math
F_{yf} = 2C_f\Big(\delta - \beta - \frac{l_f\gamma}{V_x}\Big),
\qquad
F_{yr} = 2C_r\Big(-\beta + \frac{l_r\gamma}{V_x}\Big)
```

### 제어와 추정 (06~08)

크루즈 제어는 속도 오차의 PI로 목표 가속도를 만들고, 상태 추정기는 측정값과 모델 예측의 차이로 추정값을 보정합니다.

```math
a_{des} = K_P\,e + K_I\!\int_0^t e\,d\tau \quad (e = V_{ref} - V_x),
\qquad
\dot{\hat{x}} = A\hat{x} + Bu + K(y - C\hat{x})
```

## 실행

04~08의 모델은 파라미터를 MATLAB 워크스페이스에서 읽습니다. 각 폴더의 `*_Initialize.m`(08은 `State_Estimator.m`, `Cornering_Stiffness_Tuning.m`)을 실행하면 파라미터 설정, 시뮬레이션, 결과 그래프가 한 번에 진행됩니다.
