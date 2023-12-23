import cv2
import os

def main():
    path=r'C:\Users\Andrea\Documents\uni\Magistrale\Multimedia data processing\Esercizi\Y4M\Stitch\gallery_0.jpg'
    print(path)
    img=cv2.imread(path,cv2.IMREAD_COLOR)
    cv2.imshow("fuck",img)

if __name__ == '__main__':
    main()