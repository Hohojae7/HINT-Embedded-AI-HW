# 06. 크루즈 컨트롤

앞차가 없을 때 목표 속도를 유지하는 크루즈 제어기를 만든 실습입니다. 60 km/h에서 출발해 100 km/h를 따라가게 했고, 같은 제어기를 두 가지 차량 모델에 붙여 결과를 비교했습니다.

| 파일 | 내용 |
|---|---|
| [`CruiseControl_Simul.slx`](CruiseControl_Simul.slx) | 차량을 `Vehicle Body 1DOF Longitudinal` 블록으로 구성 |
| [`CruiseControl_Simul2.slx`](CruiseControl_Simul2.slx) | 차량을 04에서 직접 만든 차체·휠·타이어 서브시스템으로 구성 |
| [`CruiseControl_Initialize.m`](CruiseControl_Initialize.m) | 파라미터를 설정하고 두 모델을 차례로 실행해 실제 속도와 목표 속도를 그림 |

## 제어기 구조

상위 제어기가 속도 오차로 목표 가속도를 만들고, 하위 제어기가 그 가속도를 내는 데 필요한 구동력을 계산합니다.

```math
a_{des} = K_P\,(V_{ref} - V_x) + K_I\int_0^t (V_{ref} - V_x)\,d\tau
```

```math
F_{des} = m\,a_{des} + F_{aero} + R_x
```

공기·구름저항을 미리 더해 주는 피드포워드라서, PI는 남은 오차만 맡습니다. PI의 적분항은 정상상태 오차를 0으로 만듭니다. 실제 파워트레인은 힘을 바로 내지 못하므로 1차 지연을 넣었습니다.

```math
F_x = \frac{1}{\tau s + 1}\,F_{des}, \qquad \tau = 0.5\ \text{s}
```

게인은 $K_P = 0.1,\ K_I = 0.01$에서 시작해 스크립트에서 바꿔 가며 응답을 봤습니다. `CruiseControl_Simul2`는 구동력에 $r_{eff}$를 곱해 휠 토크로 넣습니다.

## 사용한 블록

| 블록 | 용도 |
|---|---|
| Constant, Sum, Gain, Integrator | 목표 속도와 PI 제어기 |
| Aero-Resistance, Frolling Front 서브시스템 | 피드포워드용 공기저항, 구름저항 계산 (Product, Math Function, Sign, Switch) |
| Transfer Fcn | 파워트레인 지연 |
| Vehicle Body 1DOF Longitudinal | 차체 모델 (`CruiseControl_Simul`) |
| 차체·휠·타이어 서브시스템 | 04의 직접 만든 모델 (`CruiseControl_Simul2`) |
