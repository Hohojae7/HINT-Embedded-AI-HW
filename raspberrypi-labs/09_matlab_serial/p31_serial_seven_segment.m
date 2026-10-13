%% p32: Pi 연결
% IP가 바뀌었다면 Pi 터미널의 hostname -I 결과로 수정
clear segmentDisp sd mypi
piPassword = getenv('RPI_PASSWORD');
assert(~isempty(piPassword), 'Set RPI_PASSWORD before running this script.');
mypi = raspi('192.168.137.68', 'user', piPassword);
disp(mypi);

%% p36: 시리얼 포트 열기
% 강의자료 예시 경로
uartPort = '/dev/ttyAMA0';
baudRate = 9600;

segmentDisp = serialdev(mypi, uartPort, baudRate);

%% p36: 숫자와 문자를 순서대로 표시하기
write(segmentDisp, 0);
write(segmentDisp, 7);
write(segmentDisp, hex2dec('a'));
write(segmentDisp, hex2dec('b'));

%% p37: 화면 지우기
write(segmentDisp, hex2dec('76'));

%% p37: 커서 위치를 지정하고 9 표시하기
write(segmentDisp, [hex2dec('79'), 2]);
write(segmentDisp, 9);
% 위치 2는 왼쪽에서 세 번째 자리입니다.

%% p38: 준비된 MATLAB 클래스로 디스플레이 제어하기
% 같은 포트를 중복으로 열지 않도록 기존 serialdev 객체를 해제
clear segmentDisp

sd = raspi.internal.serialsegmentdisplay(mypi);

clr(sd);
print(sd, '112');
print(sd, 'dead');
print(sd, 'beef');

%% p38: 1부터 1000까지 증가하는 카운터
N = 1000;
delay = 0.01;

clr(sd);
for i = 1:N
    print(sd, num2str(i));
    pause(delay);
end
