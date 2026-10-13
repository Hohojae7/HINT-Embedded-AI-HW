# 04. 종방향 차량 동역학

가속·제동 입력에 따라 차속, 휠 속도, 슬립률이 어떻게 변하는지 계산하는 종방향 차량 모델입니다. 같은 시나리오를 세 가지 방식으로 구성했습니다. 처음에는 모든 수식을 서브시스템으로 직접 만들고, 뒤로 갈수록 Powertrain·Vehicle Dynamics Blockset의 블록으로 바꿨습니다.

| 모델 | 구성 | 타이어 |
|---|---|---|
| [`Long_Simul_1.slx`](Long_Simul_1.slx) | 드라이브라인, 휠, 타이어 슬립, 차체, 공기·구름저항을 모두 서브시스템으로 직접 구성 | 선형 $F_x = C\sigma$ |
| [`Long_Simul_2.slx`](Long_Simul_2.slx) | 휠과 타이어를 `Longitudinal Wheel`(디스크 브레이크)로 교체. 제동 입력을 브레이크 압력으로 | Magic Formula |
| [`Long_Simul_3.slx`](Long_Simul_3.slx) | 엔진을 `Simple Engine`, 차체를 `Vehicle Body 1DOF Longitudinal`로 교체 | Magic Formula |

모델마다 `*_Initialize.m`이 있어서 파라미터 설정, 시뮬레이션, 속도·구동력·슬립률 그래프를 한 번에 실행합니다. 시나리오는 0~4초 스로틀(30%, 3번은 50%), 4~8초 무입력, 8초부터 제동입니다. 1·2번은 차가 멈추면 Stop Simulation 블록으로 끝납니다.

## 수식

**차체** (노면 경사 0)

```math
m\ddot{x} = F_x - F_{aero} - R_x
```

```math
F_{aero} = \tfrac{1}{2}\rho C_d A_F V_x^2, \qquad R_x = f\,F_z \quad (f = 0.015)
```

구름저항은 휠이 거의 멈췄을 때(휠 속도 0.1 이하) 0으로 둬서 정지 직후 값이 튀지 않게 했습니다.

**휠**

```math
I_w\dot{\omega} = T_{wheel} - r_{eff}F_x
```

**슬립률**: 가속할 때는 휠이 더 빨리 돌고, 제동할 때는 차가 더 빨리 움직이므로 분모를 다르게 씁니다. 모델은 토크 부호로 둘을 고릅니다.

```math
\sigma_x =
\begin{cases}
\dfrac{r_{eff}\,\omega - V_x}{r_{eff}\,\omega} & \text{(accel)} \\[10pt]
\dfrac{r_{eff}\,\omega - V_x}{V_x} & \text{(brake)}
\end{cases}
```

**타이어 힘**: 슬립이 작을 때는 선형으로 근사할 수 있지만, 슬립이 커지면 실제 마찰력은 오히려 줄어듭니다. 그래서 2·3번은 Magic Formula를 씁니다.

```math
F_x = C\,\sigma_x \quad (C = 6 \times 10^4\ \text{N})
\qquad\text{vs.}\qquad
F_x = F_z\,D\sin\!\Big(C\arctan\big(B\sigma_x - E(B\sigma_x - \arctan B\sigma_x)\big)\Big)
```

마른 아스팔트 기준 $B = 10,\ C = 1.65,\ D = 1,\ E = 0.01$입니다.

**유효 반경**: 하중으로 눌린 타이어의 실제 굴림 반경입니다.

```math
r_{eff} = \frac{r_w\sin\phi}{\phi}, \qquad \phi = \cos^{-1}\frac{r_{stat}}{r_w}
\qquad (r_w = 0.4,\ r_{stat} = 0.3 \Rightarrow r_{eff} \approx 0.366\ \text{m})
```

## 차량 파라미터

04~07에서 공통으로 쓰는 값입니다.

| $m$ | $l_f$ / $l_r$ | $I_w$ | $A_F$ | $C_d$ | $\rho$ |
|---|---|---|---|---|---|
| 1500 kg | 1.4 / 1.8 m | 0.8 kg·m² | 4 m² | 0.3 | 1.225 kg/m³ |

## 사용한 블록

| 블록 | 용도 |
|---|---|
| Step | 스로틀·브레이크 입력 시나리오 |
| Gain, Sum, Product, Integrator | 수식으로 직접 만든 차체·휠·저항 모델 |
| Switch, Abs, Sign, Math Function | 슬립률 분모 선택, 저속 구름저항 처리, 속도 제곱 |
| Transfer Fcn | 엔진 토크(0.5 s), 브레이크(0.1 s)의 1차 지연 |
| Stop Simulation | 차가 멈추면 종료 |
| Longitudinal Wheel | Magic Formula 타이어 + 디스크 브레이크 휠 (2, 3) |
| Simple Engine, Vehicle Body 1DOF Longitudinal | 엔진과 차체 블록 (3) |
