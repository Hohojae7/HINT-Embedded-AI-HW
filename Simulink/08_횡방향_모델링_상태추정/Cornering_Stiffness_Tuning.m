
clear 

% exp2: 50kph slalom test

load EXPDATA.mat;
data = exp;

% 데이터 추출
Time=data.X.Data';
t_l=Time(length(Time));

Ax=data.Y(26).Data';
Ay=data.Y(28).Data';
Yawrate=data.Y(33).Data';
SteerAngle=data.Y(23).Data'/16.4;
Vx_kph=data.Y(42).Data';
Fyf_Sensor=data.Y(15).Data';
Fyr_Sensor=data.Y(16).Data';
Slipangle=data.Y(41).Data';

figure(1)
plot(Time,SteerAngle,Time,Vx_kph)
xlabel('time(sec)'); ylabel('SteerAngle(Deg), Vx(km/h)'); grid on

% 파라미터 값 
M = 1523; % kg
J = 3495; % kg m^2
g = 9.81; % m/s^2
Ts = 0.01; % sec

Lf = 1.15; % m
Lr = 1.49; % m
L = Lf+Lr; % m

Cf_nom = 140000; % N/rad
Cr_nom = 160000; % N/rad

% Simulation execution
out = sim('Lat_Dynamics_Simul.slx');

figure(2)
plot(out.tout,out.YawRate_Model(:,2)*180/pi,Time,Yawrate)
xlabel('time(sec)'); ylabel('YawRate(deg/s)'); grid on
legend('From Model','From Measurement')
ylim([-30 30])

figure(3)
plot(out.tout,out.Beta_Model(:,2)/pi*180,Time,Slipangle)
xlabel('time(sec)'); ylabel('beta(deg)'); grid on
legend('From Model','From Measurement')
ylim([-2.5 2.5])