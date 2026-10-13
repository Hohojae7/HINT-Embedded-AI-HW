clear

% Vehicle Parameters
m = 1500; % kg
lf = 1.4; % Horizontal distance from CG to front axle [m]
lr = 1.8; % Horizontal distance from CG to rear axle [m]
l = lf+lr;
h = 0.65; % CG height above axles [m]

% Wheel-tire Parameters
I_wheel = 0.8; % wheel inertia [kg*m^2]
r_load = 0.3; % loaded radius [m]
rw = 0.4; % unloaded radius [m]
phi = acos(r_load/rw);
r_eff = rw * sin(phi)/phi;

% Magic Formula Dry Asphalt
Dx = 1;
Cx = 1.65;
Bx = 10;
Ex = 0.01;

% Aero Resistance
Af = 4; % Frontal Area [m^2]
Cd = 0.3; % Longitudinal drag coefficient

% Rolling Resistance
R_roll = 0.015; % Rolling resistance coefficient

% Initial value
x_0 = 0; % Initial position [m]
xdot_0 = 0; % Initial velocity [m/s]

% Environment Parameters
g = 9.81; % gravitational acceleration [m/s^2]
rho = 1.225; % air density [kg/m^3]

% 시나리오 1:
% 0~4초 동안 30% Throttle로 가속
% 4~8초 동안 주행 입력 없음
% 8초~  일정한 브레이크 패달 입력으로 감속 
% 차속이 0이 됨과 함께 시뮬레이션 종료
% Throttle Input Setting
Throttle_Input_period = 4; % [sec]
Throttle_Input_Amplitude = 30; % [%]
% Brake Input Setting
Brake_Input_start_time = 8; % [sec]
Brake_Input_Amplitude = 10; % [%]

% 시나리오 2: 시나리오 1에서 Brake Input Amplitude를 10에서 100까지 증가시켜보기 


% Run Simulation
out = sim('Long_Simul_2.slx')

% Plotting Result
figure(2)
subplot(3,1,1)
plot(out.vx_kph(:,1),out.vx_kph(:,2))
ylabel('Velocity(km/h)'); grid on;
title('Velocity, Slip ratio, traction force for each wheel')
subplot(3,1,2)
plot(out.Fx(:,1),out.Fx(:,2)/4)
ylabel('Traction force(N)'); grid on;
subplot(3,1,3)
plot(out.slip_ratio(:,1),out.slip_ratio(:,2))
ylabel('Slip ratio'); grid on; ylim([-1 0.2])
xlabel('time(sec)')

% Magic formula shape
slip = 0:0.001:1;
Mu = Dx*sin(Cx * atan(Bx*slip-Ex*(Bx*slip-atan(Bx*slip))));
Fz = m*g/4;
figure(3)
plot(slip, Mu*Fz)
xlabel('Slip Ratio'); ylabel('Fx');
title('Magic formula')
grid on
% 
% figure(6)
% plot(out.slip_ratio(:,1),out.Fx(:,2)./out.slip_ratio(:,2))
