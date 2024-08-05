from PIL import Image, ImageTk
import sys
import tkinter as tk

def main():
    if len(sys.argv) != 2:
        print("Usage: python pil.py <image_path>")
        sys.exit(1)

    image_path = sys.argv[1]

    try:
        img = Image.open(image_path)

        root = tk.Tk()
        root.title(image_path)

        img_tk = ImageTk.PhotoImage(img)

        label = tk.Label(root, image=img_tk)
        label.pack()

        root.mainloop()

    except IOError:
        print("Error: Unable to open image file.")
        sys.exit(1)

if __name__ == "__main__":
    main()
