import cv2
import mediapipe as mp
import serial
import time

arduino = serial.Serial('COM3', 9600)
time.sleep(2)

mp_pose = mp.solutions.pose
pose = mp_pose.Pose()
mp_drawing = mp.solutions.drawing_utils

cap = cv2.VideoCapture(0)

while cap.isOpened():
    success, frame = cap.read()
    if not success:
        print("Geen beeld van camera.")
        break

    # Beeld omzetten naar RGB
    image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    results = pose.process(image)

    # Beeld terug naar BGR voor OpenCV
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

    if results.pose_landmarks:
        mp_drawing.draw_landmarks(
            image, results.pose_landmarks, mp_pose.POSE_CONNECTIONS)

    cv2.imshow('MediaPipe Pose', image)

    if cv2.waitKey(5) & 0xFF == 27:  # Druk op ESC om te stoppen
        break

cap.release()
cv2.destroyAllWindows()
