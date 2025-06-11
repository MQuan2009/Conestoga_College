import cv2
import os

# Settings
output_folder = 'notebook0'
images_per_batch = 20
total_images = 100
batch_count = total_images // images_per_batch

# Create output folder if it doesn't exist
os.makedirs(output_folder, exist_ok=True)

video_url = "http://192.168.1.14:4747/video"

# Start video capture
cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Error: Could not open webcam.")
    exit()

# Set resolution (try 1280x720 or 1920x1080)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

image_count = 0

print("Instructions:")
print("- Hold the bottle in one angle.")
print(f"- Press 'b' to start capturing {images_per_batch} images.")
print(f"- Repeat until {total_images} images are captured.\n")

for batch in range(batch_count):
    print(f"Ready for batch {batch + 1}/{batch_count}. Press 'b' to begin...")

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Error: Could not read frame.")
            break

        cv2.imshow("Live Preview - Press 'b' to capture batch", frame)

        key = cv2.waitKey(1) & 0xFF
        if key == ord('b'):
            print(f"Capturing batch {batch + 1}...")
            for i in range(images_per_batch):
                ret, frame = cap.read()
                if not ret:
                    print("Error: Could not read frame.")
                    break

                img_name = f"image_{image_count:03d}.jpg"
                img_path = os.path.join(output_folder, img_name)
                cv2.imwrite(img_path, frame)
                print(f"Saved {img_path}")
                image_count += 1

                cv2.imshow("Captured", frame)
                cv2.waitKey(100)

            print(f"Batch {batch + 1} complete.\n")
            break

        elif key == ord('q'):
            print("Quitting.")
            cap.release()
            cv2.destroyAllWindows()
            exit()

cap.release()
cv2.destroyAllWindows()
print(f"Done. {image_count} images saved in '{output_folder}'.")
