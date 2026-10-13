%% 작업 폴더 설정
% 이 스크립트가 저장된 폴더에서 작업한다.
workDir = fileparts(mfilename('fullpath'));
cd(workDir);

%% Pi 연결
piIP = '192.168.137.68';

% 이전 웹캠 객체와 Pi 연결 정리
clear camTest mypi

piPassword = getenv('RPI_PASSWORD');
assert(~isempty(piPassword), 'Set RPI_PASSWORD before running this script.');
mypi = raspi(piIP, 'user', piPassword);

%% Pi에 빌드 결과 저장 폴더 생성
piBuildDir = ...
    '/home/user/projects/raspberrypi-labs/day4_matlab/resnet50_webcam';

system(mypi, ['mkdir -p ' piBuildDir]);

%% 64비트 Raspberry Pi용 배포 설정
board = targetHardware('Raspberry Pi (64bit)');

board.DeviceAddress = piIP;
board.Username = 'user';
board.Password = piPassword;

board.BuildDir = piBuildDir;
board.BuildAction = 'Build, load, and run';

%% C++ 코드 생성 설정
board.CoderConfig.TargetLang = 'C++';

%% 딥러닝 코드 생성 설정
dlcfg = coder.DeepLearningConfig('none');

board.CoderConfig.DeepLearningConfig = dlcfg;

%% 코드 생성, Pi에서 빌드, 배포 및 실행
deploy(board, 'raspi_webcam_resnet');