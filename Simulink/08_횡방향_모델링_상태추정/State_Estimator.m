%% 1. Tire Lateral force estimator (Open loop state estimate)

clear 

load EXPDATA.mat;
data = exp;
Vel = 50/3.6;

% 데이터 추출

Time=data.X.Data';
t_l=Time(length(Time));
t_l = 40;

Ax=data.Y(26).Data';
Ay=data.Y(28).Data';
Yawrate=data.Y(33).Data';
SteerAngle=data.Y(23).Data'/18;
Vx_kph=data.Y(42).Data';

Fyf_Sensor=data.Y(15).Data';
Fyr_Sensor=data.Y(16).Data';

Slipangle=data.Y(41).Data';

figure(1)
plot(Time,SteerAngle,Time,Vx_kph)
xlabel('time(sec)'); ylabel('SteerAngle(Deg), Vx(km/h)'); grid on
legend('Steering','Vx')

% 파라미터 값 
M = 1523; % kg
J = 3495; % kg m^2
g = 9.81; % m/s^2
Ts = 0.01; % sec

Lf = 1.15; % m
L = 2.64; % m
Lr = L-Lf; % m

Cf_nom = 146000; % N/rad
Cr_nom = 146000; % N/rad


sim('Tire_Lateral_Force_Estimation.slx');



%% 2. beta estimator (Open loop state estimate)
 

clear 

load EXPDATA.mat;
data = exp;
Vel = 50/3.6;

% 데이터 추출

Time=data.X.Data';
t_l=Time(length(Time));
t_l = 40;

Ax=data.Y(26).Data';
Ay=data.Y(28).Data';
Yawrate=data.Y(33).Data';
SteerAngle=data.Y(23).Data'/18;
Vx_kph=data.Y(42).Data';

Fyf_Sensor=data.Y(15).Data';
Fyr_Sensor=data.Y(16).Data';

Slipangle=data.Y(41).Data';

figure(1)
plot(Time,SteerAngle,Time,Vx_kph)
xlabel('time(sec)'); ylabel('SteerAngle(Deg), Vx(km/h)'); grid on
legend('Steering','Vx')

% 파라미터 값 
M = 1523; % kg
J = 3495; % kg m^2
g = 9.81; % m/s^2
Ts = 0.01; % sec

Lf = 1.15; % m
L = 2.64; % m
Lr = L-Lf; % m

Cf_nom = 146000; % N/rad
Cr_nom = 146000; % N/rad


sim('Beta_Estimation_OpenLoop.slx');

%% 3. Kalman Filter based beta estimator
 
clear 

load EXPDATA.mat;
data = exp;
Vel = 50/3.6;

% 데이터 추출

Time=data.X.Data';
t_l=Time(length(Time));
t_l = 40;


Ax=data.Y(26).Data';
Ay=data.Y(28).Data';
Yawrate=data.Y(33).Data';
SteerAngle=data.Y(23).Data'/18;
Vx_kph=data.Y(42).Data';

Fyf_Sensor=data.Y(15).Data';
Fyr_Sensor=data.Y(16).Data';

Slipangle=data.Y(41).Data';

figure(1)
plot(Time,SteerAngle,Time,Vx_kph)
xlabel('time(sec)'); ylabel('SteerAngle(Deg), Vx(km/h)'); grid on
legend('steering','velocity')
% 파라미터 값 
M = 1523; % kg
J = 3495; % kg m^2
g = 9.81; % m/s^2
Ts = 0.01; % sec

Lf = 1.15; % m
L = 2.64; % m
Lr = L-Lf; % m

Cf_nom = 130000; % N/rad
Cr_nom = 160000; % N/rad

A = [-(2*Cf_nom+2*Cr_nom)/(M*Vel) (-1+(-2*Lf*Cf_nom+2*Lr*Cr_nom)/(M*Vel^2)) ; ...
    (-2*Lf*Cf_nom + 2*Lr*Cr_nom)/J (-2*Lf^2*Cf_nom-2*Lr^2*Cr_nom)/(J*Vel)];
B = [2*Cf_nom/M/Vel ; 2*Lf*Cf_nom/J];
C = [0 1];

out = sim('Beta_Estimation_KalmanFilter.slx');

figure(2)
plot(out.tout,out.YawRate_Kalman(:,2)*180/pi,Time,Yawrate)
xlabel('time(sec)'); ylabel('YawRate(deg/s)'); grid on
legend('From Model','From Measurement')
ylim([-20 20])

figure(3)
plot(out.tout,out.Beta_Kalman(:,2)/pi*180,Time,Slipangle)
xlabel('time(sec)'); ylabel('beta(deg)'); grid on
legend('From Model','From Measurement')
ylim([-1.5 1.5])


