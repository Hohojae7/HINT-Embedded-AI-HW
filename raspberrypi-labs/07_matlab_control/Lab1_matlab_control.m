%% p17-18: Raspberry Pi 연결

labDir = 'C:\Users\User\Documents\Embedded_AI_MBD\day4_matlab\lab1';
cd(labDir);

piPassword = getenv('RPI_PASSWORD');
assert(~isempty(piPassword), 'Set RPI_PASSWORD before running this script.');
mypi = raspi('192.168.137.68', 'user', piPassword);

disp(mypi);

%% p19: 내장 LED 끄고 켜기

disp(mypi.AvailableLEDs);

led = mypi.AvailableLEDs{1};

disp(led);

writeLED(mypi, led, 0);
pause(1);

writeLED(mypi, led, 1);
pause(1);

%% p20: 내장 LED 10번 깜빡이기

for i = 1:10
    writeLED(mypi, led, 0);
    pause(0.5);

    writeLED(mypi, led, 1);
    pause(0.5);
end

%% p21: Pi에서 Linux 명령 실행하기

system(mypi, 'ls -al /home/')

output = system(mypi, 'ls -al /home/user');
disp(output);

owner = system(mypi, 'stat --format="%U" /home/user/.profile');
disp(owner);

%% p22: Pi의 이미지 파일을 노트북으로 가져오기

getFile(mypi, '/usr/share/pixmaps/debian-logo.png', labDir);

img = imread(fullfile(labDir, 'debian-logo.png'));

figure;
image(img);
axis image;
axis off;

%% p23: 노트북의 이미지 파일을 Pi로 보내기

piLabDir = '/home/user/projects/raspberrypi-labs/day4_matlab/lab1';

system(mypi, ['mkdir -p ' piLabDir]);

piCopy = [piLabDir '/debian-logo.png.copy'];

putFile(mypi, fullfile(labDir, 'debian-logo.png'), piCopy);

system(mypi, ['ls -l ' piCopy])

%% p23: Pi에 보낸 복사본 삭제하기

deleteFile(mypi, piCopy);

system(mypi, ['ls -al ' piLabDir])