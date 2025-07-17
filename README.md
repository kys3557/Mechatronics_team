# Mechatronics_Competition
  
## 🧭 프로젝트 개요  

총 13팀 / 팀당 3명  
대회장 트랙을 가장 빠르고 정확하게 주행하는 팀이 우승  
[곡선 구간 / 오르막 구간 / 장애물 정지 구간]
<br><br>
![KakaoTalk_20250717_210738455](https://github.com/user-attachments/assets/fcc5218a-85ed-41b3-8bdd-d05fff47f16e)
<img src="https://github.com/user-attachments/assets/4672e900-be1e-4128-9473-26b9c0f71573" width="450" height="520"/>



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
흰색 바닥을 수광부가 바라볼 경우,  
→ IR LED의 빛이 강하게 반사되어 수광부에 도달  
→ 수광부가 빛을 감지 → 전기 신호 "1"로 인식됨  

검정 라인을 바라볼 경우,  
→ 검정색은 빛을 흡수하기 때문에 반사되지 않음  
→ 수광부에 빛이 도달하지 않음 → 전기 신호 "0"으로 인식됨  

각각의 IR 센서에서 읽어온 "1" 또는 "0" 값은 mbed의 BusIn으로 병렬 입력되어 **7비트 이진값(int)** 으로 결합  


<img width="447" height="564" alt="KakaoTalk_20250717_233217028" src="https://github.com/user-attachments/assets/b9c65899-74f6-4a0e-878d-d7e7b10cf7a9" />  
<br><br>

[코드참고]
| 센서 위치  | 이름    | 비트값         |
| ------ | ----- | ----------- |
| 가장 왼쪽  | `lll` | `0b1000000` |
| 왼쪽 중간  | `ll`  | `0b0100000` |
| 왼쪽     | `l`   | `0b0010000` |
| 중앙     | `m`   | `0b0001000` |
| 오른쪽    | `r`   | `0b0000100` |
| 오른쪽 중간 | `rr`  | `0b0000010` |
| 가장 오른쪽 | `rrr` | `0b0000001` |

예) 
```
if ((sensorIn | 0b1111110) == 0b1111110)
```
검정 차선 기준 차량이 왼쪽에 있으므로  
error = +3.0으로 계산됨 → 오른쪽으로 조향해야 함  
※ (-)일 경우 왼쪽으로 조향 / (0)일 경우 직진
<br><br>  

https://github.com/user-attachments/assets/0b8ee586-df8d-4c87-afdd-6b04327e7aaa


⛔ 라인 트래킹 중 문제점 발생  
1. 실내 조명, 자연광 등의 빛 반사로 인해:  
   → 검정 테이프가 흰색 바닥처럼 인식  
   → 센서가 라인을 감지하지 못해 오작동 발생  
2. 커브 주행 시 센서가 라인을 벗어나 모든 센서가 켜지는 현상 발생  
3. 주행 시 양 옆 발광부의 불이 2개가 꺼지는 경우  

✅ 개선 방안  
1. 빛 반사 차단 암막 커튼을 소형으로 제작 → 상부 조명으로부터의 직접 반사를 차단  
   측면 빛 차단 수광부 양옆을 절연 테이프로 감쌈 → 센서가 바로 아래 영역만 인식 가능하도록 제한  
2. 라인이 전혀 감지되지 않을 경우 이전 조향값을 그대로 유지  
3. 감지된 각각의 위치에 대해 가중치를 모두 더한 후 평균을 계산  
예) 왼쪽(left = +1.0), 중앙(mid = 0.0)이 동시에 검정 선을 감지한 경우:  
* errorSum = 20.0 * (1.0 + 0.0) = 20.0  
* errorCount = 2  
* error = 10.0 → 오른쪽 살짝 조향  

```
if (errorCount == 0) {
    error_ = prevError_;  //이전 조향 유지
} else {
    error_ = errorSum / ((float)errorCount);  //2개가 꺼질경우
}
prevError_ = error_;
return error_;
```


### 2. ⛰️ 오르막 / 내리막 경사 대응
- `HallSensor`로 바퀴 속도를 측정하여 경사 감지
- **실제 속도(curr_speed)가 목표 속도(setSpeed)보다 낮으면 오르막**, 보정된 PID 출력을 증가
- 내리막에서는 속도를 제한하거나 제동

```
if(curr_speed > setSpeed) // 현재 속도가 목표 속도 보다 빠를경우 
{
    setSpeed = 0.0; // 그냥 속도를 0으로
}
```  
<img width="469" height="326" alt="image" src="https://github.com/user-attachments/assets/98077c4b-cd37-4ef5-981a-de25e87d91ed" />


### 3. ⚠️ 장애물 감지 및 정지
- `DistanceSensor`(PSD 센서)로 전방 거리 측정
- 설정 거리(예: 30cm) 이내 진입 시 후진 후 정지
 


https://github.com/user-attachments/assets/a88faef8-e433-46b3-a752-1f677d68cb98



### 4. 🎛️ PID 속도 제어
- `PIDController` 클래스 사용 (P, I, D 항 조합)
- 목표 속도(setSpeed)와 실제 속도(curr_speed)의 오차를 보정

### 5. 📈 USB Plotting (디버깅용)
- `Plotting` 클래스 활용
- Hall 센서 속도, PSD 거리 등의 센서 값을 USB로 송출
- 외부 툴로 실시간 시각화
- 실제 센서 동작의 신뢰성 확인

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

