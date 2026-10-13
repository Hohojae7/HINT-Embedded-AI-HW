%% p39-43: Simulink 내장 LED 모델 실행 제어

%% 1. 실습 폴더와 Pi 연결
labDir = fileparts(mfilename('fullpath'));
cd(labDir);
modelName = 'raspberrypi_gettingstarted';
logName = [modelName, '.log'];

% Lab1의 raspi 객체와 구분: 배포된 모델 제어에는 raspberrypi를 사용
piPassword = getenv('RPI_PASSWORD');
assert(~isempty(piPassword), 'Set RPI_PASSWORD before running this script.');
r = raspberrypi('192.168.137.68', 'user', piPassword);

%% 2. 모델 실행 여부 확인: 실행 중이면 true(1)
isModelRunning(r, modelName)

%% 3. 모델 정지: LED의 주기적인 깜빡임이 멈추는지 확인
stopModel(r, modelName);
isModelRunning(r, modelName)

%% 4. 이미 배포된 모델 다시 실행
runModel(r, modelName);
isModelRunning(r, modelName)

%% 5. Pi에 있는 실행 로그 확인
disp(system(r, ['cat ', logName]));

%% 6. 실행 로그를 노트북의 실습 폴더로 가져오기
getFile(r, logName, labDir);

% 실습 종료 후 모델을 멈추려면 3번 섹션을 다시 실행합니다.
