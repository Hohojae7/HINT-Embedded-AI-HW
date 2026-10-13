%% p25: Pi 연결 및 GPIO 핀 배치 확인
% IP가 바뀌었다면 Pi 터미널의 hostname -I 결과로 수정
clear mypi
piPassword = getenv('RPI_PASSWORD');
assert(~isempty(piPassword), 'Set RPI_PASSWORD before running this script.');
mypi = raspi('192.168.137.68', 'user', piPassword);
disp(mypi);
showPins(mypi);

%% p26: Pi 종료 방법
% 실제로 Pi를 종료하려고 할 때만 아래 명령을 실행
% system(mypi, 'sudo shutdown');
% clear mypi
%
% Pi 터미널에서 즉시 종료하는 강의자료의 다른 방법:
% sudo shutdown -h now

%% p27-28: 외부 LED 켜기
ledPin = 24;
configurePin(mypi, ledPin, 'DigitalOutput');

writeDigitalPin(mypi, ledPin, 1);
pause(1); % 켜진 상태를 관찰하기 위한 대기 시간

%% p28: 외부 LED 10번 깜빡이기
for i = 1:10
    writeDigitalPin(mypi, ledPin, 1);
    pause(0.5);
    writeDigitalPin(mypi, ledPin, 0);
    pause(0.5);
end

%% p29: 버튼 입력 핀 설정 및 상태 확인
buttonPin = 23;
configurePin(mypi, buttonPin, 'DigitalInput');

buttonState = readDigitalPin(mypi, buttonPin);
disp(buttonState);

%% p30: 버튼이 눌려 있으면 LED를 빠르게 10번 깜빡이기
for i = 1:100
    buttonPressed = readDigitalPin(mypi, buttonPin);
    if buttonPressed
        for j = 1:10
            writeDigitalPin(mypi, ledPin, 1);
            pause(0.05);
            writeDigitalPin(mypi, ledPin, 0);
            pause(0.05);
        end
    end
    pause(0.1);
end

writeDigitalPin(mypi, ledPin, 0);
