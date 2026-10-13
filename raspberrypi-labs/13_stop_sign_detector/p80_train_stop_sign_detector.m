%% 작업 폴더
workDir = fileparts(mfilename('fullpath'));
cd(workDir);

%% MATLAB에 포함된 학습 데이터
visionDataDir = fullfile(toolboxdir('vision'), 'visiondata');

trainingData = load( ...
    fullfile(visionDataDir, 'stopSignsAndCars.mat'));

% 첫 번째 열: 이미지 파일
% 두 번째 열: 정지 표지판의 바운딩 박스
positiveInstances = trainingData.stopSignsAndCars(:, 1:2);

% 이미지 경로를 절대 경로로 변경
positiveInstances{:, 1} = fullfile( ...
    visionDataDir, positiveInstances{:, 1});

% 정지 표지판이 없는 배경 이미지 폴더
negativeFolder = fullfile(visionDataDir, 'nonStopSigns');

%% 검출기 학습
xmlFile = fullfile(workDir, 'stopSignDetector.xml');

% 이미 학습한 XML이 있으면 재사용
if ~isfile(xmlFile)
    trainCascadeObjectDetector( ...
        'stopSignDetector.xml', positiveInstances, negativeFolder, ...
        'FeatureType', 'HOG', ...
        'FalseAlarmRate', 0.01, ...
        'NumCascadeStages', 3);
end

%% 동작 확인용 이미지 준비
% 기본 예제 이미지에서 표지판 주변을 잘라 사용
% 320x240 영상에서도 표지판이 충분히 크게 보이도록 한다.
sourceImage = imread(fullfile( ...
    visionDataDir, 'stopSignImages', 'image029.jpg'));

testImage = imcrop(sourceImage, [390 210 720 540]);
testImage = imresize(testImage, [240 320]);

imwrite(testImage, fullfile(workDir, 'stop_sign_test.png'));

%% 학습한 검출기로 표지판 찾기
detector = vision.CascadeObjectDetector(xmlFile);

bbox = step(detector, testImage);

detectedImage = insertObjectAnnotation( ...
    testImage, 'rectangle', bbox, 'stop sign');

figure;
imshow(detectedImage);
title('Stop sign detection');

disp('검출한 바운딩 박스 [x y width height]:');
disp(bbox);

fprintf('XML 저장 위치: %s\n', xmlFile);