import cv2
import mediapipe as mp
import time
import socket
from collections import deque

# Even wachten voor opstart
time.sleep(2)

# UDP-gegevens
UDP_IP = "192.168.178.196"  # Vervang door het IP-adres van jouw ESP32
UDP_PORT = 4211


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(b"TEST", (UDP_IP, UDP_PORT))
print("Testbericht verstuurd")

# MediaPipe initialiseren
mp_pose = mp.solutions.pose
pose = mp_pose.Pose()

mp_hands = mp.solutions.hands
hands = mp_hands.Hands(max_num_hands=2, min_detection_confidence=0.7)

mp_drawing = mp.solutions.drawing_utils

# Webcam openen
cap = cv2.VideoCapture(0)

# Buffer voor x-posities van de rechterpols
wrist_x_history = deque(maxlen=10)

# Zwaai-status bijhouden
wave_counter = 0
waving = False

wave_display_counter = 0
WAVE_DISPLAY_TIME = 15

last_thumb_status = None 

# Functie om zwaaibeweging te detecteren
def detect_wave(right_wrist, right_shoulder, wrist_x_history, waving):
    wave_detected = False

    if right_wrist.y < right_shoulder.y:
        wrist_x_history.append(right_wrist.x)

        if len(wrist_x_history) == wrist_x_history.maxlen:
            delta = max(wrist_x_history) - min(wrist_x_history)
            if delta > 0.15 and not waving:
                wave_detected = True
                waving = True
            elif delta < 0.05:
                waving = False
    return waving, wave_detected

# Functie om te detecteren of duim omhoog of omlaag is (gebaseerd op positie duim tov pols)
def thumb_position(hand_landmarks):
    thumb_tip = hand_landmarks.landmark[4]
    wrist = hand_landmarks.landmark[0]

    margin = 0.20

    if thumb_tip.y + margin < wrist.y:
        return "up"
    elif thumb_tip.y > wrist.y + margin:
        return "down"
    else:
        return "neutral"

while cap.isOpened():
    success, frame = cap.read()
    if not success:
        print("Geen beeld van camera.")
        break

    image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    pose_results = pose.process(image)
    hands_results = hands.process(image)

    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

    wave_detected_this_frame = False

    # Zwaaien detectie
    if pose_results.pose_landmarks:
        try:
            right_wrist = pose_results.pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_WRIST]
            right_shoulder = pose_results.pose_landmarks.landmark[mp_pose.PoseLandmark.RIGHT_SHOULDER]

            if right_wrist.visibility > 0.5:
                waving, wave_detected = detect_wave(right_wrist, right_shoulder, wrist_x_history, waving)
                if wave_detected:
                    print("Zwaaibeweging gedetecteerd! Verstuur WAVE")
                    sock.sendto(b"WAVE", (UDP_IP, UDP_PORT))
                    wave_detected_this_frame = True
                    wave_display_counter = WAVE_DISPLAY_TIME  # Start teller om "Zwaaien!" te tonen
            else:
                waving = False
                wrist_x_history.clear()
        except Exception as e:
            waving = False
            wrist_x_history.clear()
            print("Exception bij zwaai detectie:", e)

    # Duim positie alleen checken als er geen zwaaibeweging is deze frame
    if hands_results.multi_hand_landmarks and not wave_detected_this_frame:
        hand_landmarks = hands_results.multi_hand_landmarks[0]
        current_thumb_status = thumb_position(hand_landmarks)

        if current_thumb_status != last_thumb_status:
            last_thumb_status = current_thumb_status
            if current_thumb_status == "up":
                print("Duim omhoog gedetecteerd. Verstuur OPEN")
                sock.sendto(b"OPEN", (UDP_IP, UDP_PORT))
            elif current_thumb_status == "down":
                print("Duim omlaag gedetecteerd. Verstuur DICHT")
                sock.sendto(b"DICHT", (UDP_IP, UDP_PORT))
            else:
                print("Duim neutraal, geen actie")

    # Tekst "Zwaaien!" tonen als de teller > 0 is
    if wave_display_counter > 0:
        cv2.putText(image, 'Zwaaien!', (30, 50), cv2.FONT_HERSHEY_SIMPLEX,
                    1.5, (0, 255, 0), 3)
        wave_display_counter -= 1

    # Teken landmarks
    if pose_results.pose_landmarks:
        mp_drawing.draw_landmarks(image, pose_results.pose_landmarks, mp_pose.POSE_CONNECTIONS)

    if hands_results.multi_hand_landmarks:
        for hand_landmarks in hands_results.multi_hand_landmarks:
            mp_drawing.draw_landmarks(image, hand_landmarks, mp_hands.HAND_CONNECTIONS)

    cv2.imshow('MediaPipe Pose + Hands', image)

    if cv2.waitKey(5) & 0xFF == 27:
        break

cap.release()
cv2.destroyAllWindows()