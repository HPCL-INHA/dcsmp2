# DCSMP2(Digital Companion Sensor Message Processor 2)

1. 구현 기능
```
1. 가속도 센서만을 이용한 보행속도 및 보폭 측정
```

2. 사용법
```
make release # 릴리즈 빌드
make debug # 디버그 메세지 출력 포함 빌드
make test # server-less 테스트 용 빌드
make clean # 지우기. 지금으로썬 매번 새로 make 수행할 때 마다 돌려야 함.
```

3. 소스 구조
```
1. dcsmp2.cpp: main() 진입점 정의
2. user.cpp/user.hpp: user 정의
3. xyz.cpp/xyz.hpp: 벡터 계산 함수
4. dc_mqtt.cpp/dc_mqtt.hpp: 디지털 컴패니언 MQTT 관련 함수
5. dc_mqtt_msg.hpp: 디지털 컴패니언 MQTT 메세지 관련 헤더
```
