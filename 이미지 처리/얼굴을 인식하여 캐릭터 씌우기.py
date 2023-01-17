#!/usr/bin/env python
# coding: utf-8

# # 프로젝트 : 얼굴을 인식하여 캐릭터 씌우기

# ## 패키지 설치
# > pip install mediapipe
# 

# In[1]:


import mediapipe as mp


# In[2]:


import cv2
import mediapipe as mp

# 얼굴을 찾고, 찾은 얼굴에 표시를 해주기 위한 변수 정의
mp_face_detection = mp.solutions.face_detection # 얼굴 검출을 위한 face_detection 모듈을 사용
mp_drawing = mp.solutions.drawing_utils # 얼굴의 특징을 그리기 위한 drawing_units 모듈을 사용

# 동영상 파일 열기
cap = cv2.VideoCapture('face_video.mp4')

# 이미지 불러오기
image_right_eye = cv2.imread('right_eye.png', cv2.IMREAD_UNCHANGED)
image_left_eye = cv2.imread('left_eye.png',cv2.IMREAD_UNCHANGED)
image_nose = cv2.imread('nose.png',cv2.IMREAD_UNCHANGED)

with mp_face_detection.FaceDetection(model_selection=0, min_detection_confidence= 0.7) as face_detection: # 70% 확률로 얼굴 인식
    while cap.isOpened():
        success, image = cap.read()
        if not success:
            break

        image.flags.writeable = False
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        results = face_detection.process(image)
        
        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
        # 6가지 특징: 오른쪽 눈, 왼쪽 눈, 코 끝부분, 입 중심, 오른쪽 귀, 왼쪽 귀
        if results.detections:
            for detection in results.detections: 
                
                # 특정 위치 가져오기
                keypoints = detection.location_data.relative_keypoints
                right_eye = keypoints[0]
                left_eye = keypoints[1]
                nose_tip = keypoints[2]
                
                h, w, _ = image.shape # height, width, channel: 이미지로부터 세로, 가로 크기 가져옴
                right_eye = (int(right_eye.x*w)-20, int(right_eye.y*h) - 100) # 이미지 내에서 실제 좌표 (x,y)
                left_eye = (int(left_eye.x*w)+20, int(left_eye.y*h)-100)
                nose_tip = (int(nose_tip.x*w), int(nose_tip.y*h))
                
                #각 특징에다가 이미지 그리기                
                image[right_eye[1]-50:right_eye[1]+50, right_eye[0]-50:right_eye[0]+50] = image_right_eye
                image[left_eye[1]-50:left_eye[1]+50, left_eye[0]-50:left_eye[0]+50] = image_left_eye
                image[nose_tip[1]-50:nose_tip[1]+50, nose_tip[0]-150:nose_tip[0]+150] = image_nose

        cv2.imshow('MediaPipe Face Detection', cv2.resize(image, None, fx=0.5, fy=0.5))
        
        if cv2.waitKey(1) == ord('q'):
            break

cap.release()
cv2.destroyAllWindows()


# In[ ]:




