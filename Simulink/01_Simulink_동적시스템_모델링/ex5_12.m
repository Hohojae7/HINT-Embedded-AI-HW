clear;
clc;
close all;

% 파라미터
C = [1 2 3];
K = [2 3 4];

n = 1;

% 시뮬레이션
for i = 1:length(C)
    for j = 1:length(K)

        c = C(i);
        k = K(j);

        outdata(n) = sim("ex5_12.slx");

        n = n + 1; % n++;
    end
end


%% 결과 그래프
figure;
hold on;
grid on;

n = 1;

for i = 1:length(C)
    for j = 1:length(K)

        plot(outdata(n).tout, outdata(n).yout{1}.Values.Data  , ...
            'LineWidth', 1.5, ...
            'DisplayName', ...
            sprintf('C=%d, K=%d',C(i),K(j)));

        n = n + 1;
    end
end

xlabel('Time (s)');
ylabel('y(t)');
title('C, K Parameter Simulation');
legend('show');
