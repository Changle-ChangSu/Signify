import cv2  # pip install opencv
import socket
from ultralytics import YOLO
import time
import os
import win32gui  # pip install pywin32
import win32con


def connect_to_server():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # Set the retry mechanism until the connection is successful
    while True:
        try:
            client_socket.connect(('localhost', 12345))  # Trying to connect to a C++ server
            print("Successfully connected to the C++ server.")
            break  # Exit the loop if the connection is successful
        except ConnectionRefusedError:
            print("Connection refused, retrying in 2 seconds...")
            time.sleep(2)  # Wait for 2 seconds before retrying
        except Exception as e:
            print(f"An error occurred: {e}")
            return None  # Handle other unknown errors
    
    return client_socket  # Return the connected socket object


def set_window_topmost(window_name):
    hwnd = win32gui.FindWindow(None, window_name)
    if hwnd: 
        win32gui.SetWindowPos(hwnd, win32con.HWND_TOPMOST, 0, 0, 0, 0,
                              win32con.SWP_NOMOVE | win32con.SWP_NOSIZE)
        

def hand_sign_detection():
    # Start the TCP client
    client_socket = connect_to_server()
    if client_socket is None:
        print("Failed to connect to the server.")
        return

    # Get the directory of the current script file (model.py)
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # Construct the relative path to the model file
    model_path = os.path.join(script_dir, './train10/weights/best.pt')  # Adjust to go up and locate the model file

    # Load the model using the relative path
    model = YOLO(model_path)

    # Open the camera
    cap = cv2.VideoCapture(0)  # Parameter 0 indicates using the computer's default camera

    if not cap.isOpened():
        print("Unable to open camera")
        return

    # Set the window name
    window_name = 'Press "q" to close the camera.'

    # Create a named window to set its size and other properties
    cv2.namedWindow(window_name, cv2.WINDOW_NORMAL)  # Make window resizable
    cv2.resizeWindow(window_name, 320, 240)  # Set the desired window size

    set_window_topmost(window_name)

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Unable to read camera frame")
            break
        
        # Use the trained model for inference
        results = model(frame)
        
        # Get inference results
        result = results[0]
        
        # Filter out bounding boxes with confidence less than 0.4
        filtered_boxes = result.boxes[result.boxes.conf > 0.5]

        # If there are bounding boxes that meet the criteria, draw them
        if len(filtered_boxes) > 0:
            result.boxes = filtered_boxes
            annotated_frame = result.plot()  # Draw bounding boxes, categories, and confidence on the frame

            # Extract hand gesture letter category
            for box in filtered_boxes:
                conf = box.conf.item()  # Get confidence
                cls = box.cls.item()  # Get category index
                letter = result.names[int(cls)]  # Use the category index to get the hand gesture letter

                if conf > 0.7:  # Send to C++ server if confidence is greater than 0.85
                    message = letter  # Format message
                    print(f"Detected gesture: {letter}, confidence: {conf:.2f}")
                    
                    # Send message to the C++ server
                    client_socket.sendall(message.encode('utf-8'))

                    # Try sending data and catch connection loss
                    try:
                        client_socket.sendall(message.encode('utf-8'))
                    except (BrokenPipeError, ConnectionResetError):
                        print("Lost connection to the server. Exiting...")
                        break  # Disconnect and exit detection

        else:
            annotated_frame = frame  # If no boxes meet the criteria, show the original frame

        # Display the image with detection results
        cv2.imshow(window_name, annotated_frame)

        # Retain the 'q' key exit mechanism as a secondary method
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release camera resources and close all windows
    cap.release()
    cv2.destroyAllWindows()
    client_socket.close()  # Close the TCP connection




if __name__ == '__main__':
    hand_sign_detection()
