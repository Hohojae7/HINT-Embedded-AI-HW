clear
clc

% Simulation Time
t_l = 10;
Time = (0 : 0.001 : t_l)';

% Vehicle parameter
m = 2000;
Iz = 4000;
lf = 1.4;
lr = 1.6;
Cr = 12e3;
Cf = 11e3;

% 파라미터 값 
M = 1523; % kg
J = 3495; % kg m^2
g = 9.81; % m/s^2
Ts = 0.01; % sec

Lf = 1.15; % m
Lr = 1.49; % m
L = Lf+Lr; % m

Cf_nom = 146625; % N/rad
Cr_nom = 146400; % N/rad

% Velocity Input 
Vel_kph = 50; % [kph]
Vx_kph = zeros(length(Time),1) + Vel_kph/3.6;

% Steering Angle Input
SteerFreq = 0.5; %Hz
SteerAmp = 10; % deg
SteerAngle = SteerAmp*sin(2*pi*SteerFreq*Time);

% Simulation execution
out = sim('Lat_Dynamics_Simul.slx');

% 차량의 운동 표시 
figure(1)
plot(out.X1(:,2),out.Y1(:,2),out.X2(:,2),out.Y2(:,2))
xlabel('X (m)');ylabel('Y (m)');
legend('From manual block','From pre-defined block')
grid on

