clear

% Desired velocity (목표속도)
V_desire = 100; % [kph]

% Initial value
x_0 = 0; % Initial position [m]
xdot_0 = 60; % Initial velocity [kph]

% Cruise controller parameter
% K_P, K_I 값을 바꾸어 가면서 최적의 값 찾아보기 
K_P = 0.1;
K_I = 0.01;

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
R_roll = 0.015; % Rolling resistance coefficient
C_long = 6*10^4;


% Aero Resistance
Af = 4; % Frontal Area [m^2]
Cd = 0.3; % Longitudinal drag coefficient
rho = 1.225; % air density [kg/m^3]

% Environment Parameters
g = 9.81; % gravitational acceleration [m/s^2]

% Run Cruise Control Simulation 
% with pre-defined vehicle dynamics block
out = sim('CruiseControl_Simul.slx');

% Plotting Result
figure(1)
plot(out.vx(:,1),out.vx_kph(:,2),out.vx(:,1),out.vx(:,1)*0+V_desire)
grid on; ylabel('Velocity(km/h)');xlabel('time(sec)')
legend('Actual Velocity','Desired Velocity')
title('Cruise Control Simulation with pre-defined vehicle dynamics block')

% Run Cruise Control Simulation 
% with custom designed vehicle dynamics subsystem
out2 = sim('CruiseControl_Simul2.slx');

% Plotting Result
figure(2)
plot(out2.vx(:,1),out2.vx_kph(:,2),out2.vx(:,1),out2.vx(:,1)*0+V_desire)
grid on; ylabel('Velocity(km/h)');xlabel('time(sec)')
legend('Actual Velocity','Desired Velocity')
title('Cruise Control Simulation with custom designed vehicle dynamics subsystem')


