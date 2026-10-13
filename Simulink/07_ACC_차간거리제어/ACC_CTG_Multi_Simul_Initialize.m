clear all

% 시간
t_end = 300;
t = (0:0.001:t_end)';

% 앞 차의 속도 
V_Head(:,1) = t;
V_Head(:,2) = 100 + 10*sin(0.1*t); % [kph]

% 초기 속도
xdot1_0 = 80; % [kph]
xdot2_0 = 80; % [kph]

% 앞 차와의 초기 간격
delta_h1_0 = 600; % [m]
delta_12_0 = 600; % [m]

% Cruise Controller
V_desire = 120; % [kph]
Kp_Cruise = 1;
Ki_Cruise = 0.1;

% Adaptive Cruise Controller
L_ACC = 500; % 크루즈 제어에서 ACC 제어로 바뀌는 시점
h = 10;
ramda = 1;

% Powertrain control delay 
tau = 0.5;

% H(s)
figure(1)
H = tf([1 ramda],[h*tau h (1+ramda*h) ramda]);
bode(H); grid on


% Vehicle Parameters
m = 1500; % kg
lf = 1.4; % Horizontal distance from CG to front axle [m]
lr = 1.8; % Horizontal distance from CG to rear axle [m]
l = lf+lr;

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
out = sim('ACC_CTG_Multi_Simul.slx');

% Plotting Result
figure(2)
plot(out.L_head_Car1(:,1),out.L_head_Car1(:,2), ...
    out.L_Car1_Car2(:,1),out.L_Car1_Car2(:,2))
hold on
plot(out.vx_Car1(:,1),out.vx_Car1(:,2)*h)
plot(out.ControlMode1(:,1),out.ControlMode1(:,2)*50)
plot(out.ControlMode2(:,1),out.ControlMode2(:,2)*50+100)
hold off
grid on;
ylim([0 max([delta_h1_0 delta_12_0])+100])
xlabel('time(sec)'); ylabel('Distance(m)')
legend('L_{Head,Car1}','L_{Car1,Car2}','L_{desired}', ...
    'Car1 ControlMode','Car2 ControlMode','location','best')
title('Distance Between Cars')

figure(3)
plot(out.vx_Car1(:,1),out.vx_Car1(:,2)*3.6,...
    out.vx_Car2(:,1),out.vx_Car2(:,2)*3.6, ...
    V_Head(:,1),V_Head(:,2))
hold on
plot(out.ControlMode1(:,1),out.ControlMode1(:,2)*20)
plot(out.ControlMode2(:,1),out.ControlMode2(:,2)*20+40)
hold off
grid on;
xlabel('time(sec)'); ylabel('Velocity(km/h)')
legend('V_{x,Car1}','V_{x,Car2}','V_{x,Heading Car}',...
    'Car1 ControlMode','Car2 ControlMode','location','best')
title('Velocity of Cars')

