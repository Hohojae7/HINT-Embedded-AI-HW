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
C_long = 6*10^4;

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

% % 시나리오 1:
% % 0~4초 동안 30% Throttle로 가속
% % 4~8초 동안 주행 입력 없음
% % 8초~ 10% 브레이크 패달 입력으로 감속 
% % 차속이 0이 됨과 함께 시뮬레이션 종료 
% % Throttle Input Setting
Throttle_Input_period = 4; % [sec]
Throttle_Input_Amplitude = 30; % [%]
% Brake Input Setting
Brake_Input_start_time = 8; % [sec]
Brake_Input_Amplitude = 10; % [%]

% 시나리오 2:
% 0~4초 동안 30% Throttle로 가속
% 4~8초 동안 주행 입력 없음
% 8초~ 100% 브레이크 패달 입력으로 감속 
% 차속이 0이 됨과 함께 시뮬레이션 종료 
% Throttle Input Setting
Throttle_Input_period = 4; % [sec]
Throttle_Input_Amplitude = 30; % [%]
% Brake Input Setting
Brake_Input_start_time = 8; % [sec]
Brake_Input_Amplitude = 100; % [%]

% Run Simulation 
out = sim('Long_Simul_1.slx')

% Plotting Result
figure(1)
subplot(3,1,1)
plot(out.vx_kph(:,1),out.vx_kph(:,2))
ylabel('Velocity(km/h)'); grid on;
title('Velocity, Slip ratio, traction force for each wheel')
subplot(3,1,2)
plot(out.Fx(:,1),out.Fx(:,2)/4)
ylabel('Traction force(N)'); grid on;
subplot(3,1,3)
plot(out.slip_ratio(:,1),out.slip_ratio(:,2))
ylabel('Slip ratio'); grid on;
xlabel('time(sec)')




