# Mechatronics_Competition

## 🧭 프로젝트 개요  
총 13팀 / 한 팀당 3명  
대회장 트랙을 가장 빠르고 정확하게 주행하는 팀이 우승  
[곡선 구간 / 오르막 구간 / 장애물 정지 구간]  
![KakaoTalk_20250717_210738455](https://github.com/user-attachments/assets/fcc5218a-85ed-41b3-8bdd-d05fff47f16e)



---

## 🧩 H/W Components

| 구성 요소 | 설명 |
|-----------|------|
| **Line IR Sensor** (7채널) | 흰색 바닥 + 검정 라인 인식 (좌측 3개, 중앙 1개, 우측 3개) |
| **Hall Sensor** | 바퀴에 부착된 자석의 회전 속도 감지 (속도 계산) |
| **PSD 거리 센서** | 전방 장애물 거리 측정 |
| **DC Motor + Motor Driver** | PWM 제어를 통해 속도 및 방향 제어 |
| **Servo Motor** | 라인 오차에 따라 조향 각도 제어 |
| **Mbed Platform** | 실시간 센서 제어 및 PID 제어 구현 |

---

## 🧠 주요 기능

### 1. 🚗 라인트레이싱 주행
- `LaneSensor` 클래스 사용 (7채널 IR 센서)
- 센서 패턴을 통해 차량의 중앙 오차(`error`) 계산
- `Servo` 클래스를 이용해 steering angle 조절



### 2. ⛰️ 오르막 / 내리막 경사 대응
- `HallSensor`로 바퀴 속도를 측정하여 경사 감지
- **실제 속도(curr_speed)가 목표 속도(setSpeed)보다 낮으면 오르막**, 보정된 PID 출력을 증가
- 내리막에서는 속도를 제한하거나 제동

### 3. ⚠️ 장애물 감지 및 정지
- `DistanceSensor`(PSD 센서)로 전방 거리 측정
- 설정 거리(예: 30cm) 이내 진입 시 자동 감속 및 정지
- 거리별로 속도를 점진적으로 줄이도록 구현

### 4. 🎛️ PID 속도 제어
- `PIDController` 클래스 사용 (P, I, D 항 조합)
- 목표 속도(setSpeed)와 실제 속도(curr_speed)의 오차를 보정
- 저속 영역에서도 부드러운 제어 가능

### 5. 📈 USB Plotting (디버깅용)
- `Plotting` 클래스 활용
- Hall 센서 속도, PSD 거리 등의 센서 값을 USB로 송출
- 외부 툴로 실시간 시각화 가능

---

## 📂 주요 파일 구성

| 파일명 | 설명 |
|--------|------|
| `main.cpp` | 전체 시스템 로직 실행 |
| `Motor.h / Motor.cpp` | 모터 PWM 속도 및 방향 제어 |
| `Servo.h / Servo.cpp` | 조향 각도 제어 |
| `SpeedController.*` | PID 제어기 |
| `LaneSensor.*` | IR 라인 센서 처리 |
| `DistanceSensor.*` | PSD 거리 센서 측정 및 필터링 |
| `HallSensor.*` | 바퀴 속도 계산용 홀센서 처리 |
| `Plotting.*` | 센서 데이터 USB 송신 |

---

## ⚠️ 주의사항

- 코드 상의 `#define FORWARD 'f'` / `#define BACKWARD 'b'` 정의는 **모터 드라이버 기준 전기적 방향**입니다.
  - 실제 차량 기준으로는:
    - `FORWARD` → **후진**
    - `BACKWARD` → **전진**
- 회로 구성 또는 기구 구조에 따라 실제 회전 방향이 달라질 수 있으므로 주행 시 실제 방향을 기준으로 판단하세요.

---

## 🚀 실행 방법

1. mbed 플랫폼에 모든 소스 업로드
2. USB 직렬 모니터(baud rate: 9600 또는 설정값)에 연결하여 디버깅 정보 확인
3. 차량을 트랙 위에 올려 실행

---

## 🛠️ 향후 확장 가능성

- ROS 포팅 및 RViz 시각화
- 속도 지도 기반 자동 주행 최적화
- 라인트레이싱 외에도 QR 인식, 미션 기반 FSM 도입 가능

---

## 📅 제작 정보

- **과목**: 메카트로닉스
- **목적**: 트랙 기반 자율주행 미션 구현
- **기능**: 라인트레이싱, PID 속도 제어, 경사 감지, 장애물 정지
