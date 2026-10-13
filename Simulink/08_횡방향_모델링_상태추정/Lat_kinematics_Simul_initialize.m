clear

% Simulation Time
tend = 10;
t = 0 : 0.001 : tend;

% Vehicle parameter
lf = 1.4;
lr = 1.6;

% Velocity Input 
Vel_kph = 10; % [kph]
Vel(:,1) = t;
Vel(:,2) = zeros(1,length(t)) + Vel_kph / 3.6;

% Front Steering Input
delta_f(:,1) = t; 
for i=1:length(t)
    if t(i)<2
        delta_f(i,2) = 0;
    elseif t(i) < 5  
        delta_f(i,2) = (t(i)-2)*(5/180*pi); % 3초동안 5deg/s로 조향 
    else
        delta_f(i,2) = 15/180*pi; % 15도로 조향각 고정 
    end

end

% Rear Steering Input
delta_r(:,1) = t;
delta_r(:,2) = zeros(1,length(t));

% Simulation execution
out = sim('Lat_kinematics_Simul')

% 차량의 운동 표시 
figure(1)
plot(out.X1(:,2),out.Y1(:,2),out.X3(:,2),out.Y3(:,2))
xlabel('m');ylabel('m');
legend('From manual block','From pre-defined block')
grid on

