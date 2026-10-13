function raspi_webcam_resnet()
%#codegen
% Copyright 2020 The MathWorks, Inc.
% 강의자료 56~59쪽 기반 USB 웹캠 이미지 분류 예제

% Pi와 첫 번째 USB 웹캠 연결
raspiObj = raspi();
cam = webcam(raspiObj, 1);

% 사전 학습된 ResNet-50 불러오기
net = coder.loadDeepLearningNetwork('resnet50');

% 모델 입력 크기: 높이, 너비, RGB 채널
inputSize = [224, 224, 3];

% 첫 분류 전까지 표시할 문구
textToDisplay = '......';

% 분류 간격 측정 시작
start = tic;

fprintf('Entering into while loop.\n');

while true
    % USB 웹캠에서 이미지 한 장 가져오기
    img = snapshot(cam);

    elapsedTime = toc(start);

    % 일정 간격으로 이미지 분류
    if elapsedTime > 1
        % 모델 입력 크기에 맞게 이미지 조정
        imgSizeAdjusted = imresize(img, inputSize(1:2));

        % 이미지 분류
        [label, score] = net.classify(imgSizeAdjusted);

        % 가장 높은 분류 점수
        maxScore = max(score);

        % 표시할 문자열 만들기
        labelStr = cellstr(label);

        textToDisplay = sprintf( ...
            'Label : %s \nScore : %f', ...
            labelStr{:}, maxScore);

        % 다음 분류 간격 측정 시작
        start = tic;
    end

    % 영상에 분류 결과 표시
    imgLabel = insertText(img, [0, 0], textToDisplay);

    % Pi 화면에 영상 출력
    displayImage(raspiObj, imgLabel);
end

end