# 07. MATLAB으로 Pi 제어하기

노트북의 MATLAB에서 `raspi` 객체로 Pi에 접속해 원격으로 다뤄 본 실습입니다. 코드가 Pi로 옮겨지지 않고, MATLAB이 명령을 하나씩 보내 Pi에서 실행합니다.

| 파일 | 용도 |
|---|---|
| [`Lab1_matlab_control.m`](Lab1_matlab_control.m) | 섹션(`%%`)별로 나눠 실행하는 실습 스크립트 |
| [`debian-logo.png`](debian-logo.png) | `getFile()`로 Pi에서 노트북으로 가져온 이미지 |

스크립트는 다음 순서로 진행됩니다.

1. `raspi(IP, 사용자, 비밀번호)`로 연결하고 보드 정보 확인
2. 내장 LED(`led0`)를 `writeLED()`로 끄고 켠 뒤, 0.5초 간격으로 10번 깜빡이기
3. `system()`으로 Pi에서 리눅스 명령 실행 (`ls`로 폴더 목록, `stat`로 `.profile` 소유자 확인)
4. `getFile()`로 Pi의 `debian-logo.png`를 가져와 MATLAB 창에 표시
5. `putFile()`로 같은 이미지를 Pi에 복사본으로 보내고, `deleteFile()`로 삭제

`labDir`은 노트북의 작업 폴더 경로라서 환경에 맞게 바꿔서 사용합니다.
