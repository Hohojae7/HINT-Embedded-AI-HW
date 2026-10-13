clear 

% 앞 차의 속도 
V_Head = 100; % [kph]

% 내차의 초기 속도
xdot_0 = 80; % [kph]

% 앞 차와의 초기 간격
delta_0 = 800; % [m]

% Cruise Controller
V_desire = 120; % [kph]
Kp_Cruise = 1;
Ki_Cruise = 0.1;

% Adaptive Cruise Controller
L_ACC = 500; % 크루즈 제어에서 ACC 제어로 바뀌는 시점 
L_desire = 300;
Kp_ACC = 0.01; % 0.01, 0.1, 1 
Kd_ACC = 0; % 0, 0.1, 1, 3

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

% Environment Parameters
g = 9.81; % gravitational acceleration [m/s^2]
rho = 1.225; % air density [kg/m^3]

% Run Simulation
out = sim('ACC_Space_Single_Simul.slx')

% Plotting Result
figure(1)
plot(out.L(:,1),out.L(:,2),out.L(:,1),out.L(:,2)*0+L_desire)
hold on
plot(out.ControlMode(:,1),out.ControlMode(:,2)*100)
hold off
grid on;
ylim([0 delta_0+100])
xlabel('time(sec)'); ylabel('Distance(m)')
legend('L','L_{desired}','Control Mode')
title('L and control mode')

figure(2)
plot(out.vx(:,1),out.vx(:,2)*3.6,...
    out.vx(:,1),out.vx(:,2)*0+V_Head)
hold on
plot(out.ControlMode(:,1),out.ControlMode(:,2)*10)
hold off
grid on;
xlabel('time(sec)'); ylabel('Velocity(km/h)')
legend('V_{x,My Car}','V_{x,Heading Car}','ControlMode','location','best')
title('Velocity and control mode')




