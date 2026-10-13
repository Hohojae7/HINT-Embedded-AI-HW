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
Cx = 1.65*1.05; % Assume more slippery surface
Bx = 10;
Ex = 0.01;

% ABS slip threshold
Slip_thres = 0.13;

% Aero Resistance
Af = 4; % Frontal Area [m^2]
Cd = 0.3; % Longitudinal drag coefficient

% Rolling Resistance (블록 내부 파라미터 사용)

% Initial value
x_0 = 0; % Initial position [m]
xdot_0 = 0; % Initial velocity [m/s]

% Environment Parameters
g = 9.81; % gravitational acceleration [m/s^2]
rho = 1.225; % air density [kg/m^3]

% 시나리오:
% 0~4초 동안 30% Throttle로 가속
% 4~8초 동안 주행 입력 없음
% 8초~ 일정한 브레이크 패달 입력으로 감속 
% Brake_Input_Amplitude를 5e6Pa 와 1e7Pa로 바꾸어 가면서 결과를 확인해보기 

% Throttle Input Setting
Throttle_Input_period = 4; % [sec]
Throttle_Input_Amplitude = 30; % [%]
% Brake Input Setting
Brake_Input_start_time = 8; % [sec]
Brake_Input_Amplitude = 5e6; % Assume Full Brake Condition in 1e7[Pa]


% Run ABS_OFF Simulation
ABS_ONOFF = 0;
out_ABSOFF = sim('ABS_Simul.slx')

% Run ABS_ON Simulation
ABS_ONOFF = 1;
out_ABSON = sim('ABS_Simul.slx')

% Plotting Result
figure(6)
subplot(3,1,1)
plot(out_ABSOFF.vx_kph(:,1),out_ABSOFF.vx_kph(:,2),...
    out_ABSON.vx_kph(:,1),out_ABSON.vx_kph(:,2))
ylabel('Velocity(km/h)'); grid on;
title('Velocity, Slip ratio, traction force for each wheel')
subplot(3,1,2)
plot(out_ABSOFF.Fx(:,1),out_ABSOFF.Fx(:,2)/4,...
    out_ABSON.Fx(:,1),out_ABSON.Fx(:,2)/4)
ylabel('Traction force(N)'); grid on;
subplot(3,1,3)
plot(out_ABSOFF.slip_ratio(:,1),out_ABSOFF.slip_ratio(:,2),...
    out_ABSON.slip_ratio(:,1),out_ABSON.slip_ratio(:,2))
ylabel('Slip ratio'); grid on; ylim([-1 0.2])
xlabel('time(sec)')
legend('without ABS','with ABS','location','southwest')

% Magic formula shape
slip = 0:0.001:1;
Mu = Dx*sin(Cx * atan(Bx*slip-Ex*(Bx*slip-atan(Bx*slip))));
Fz = 3678; % N
figure(7)
plot(slip, Mu*Fz)
xlabel('Slip Ratio'); ylabel('Fx');
title('Magic formula')
grid on

