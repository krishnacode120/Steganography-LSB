LSB Image Steganography Project

### *Using C, BMP Images, and Least Significant Bit (LSB) Encoding*


## 📌 **Project Overview**

Steganography is the technique of hiding secret data inside another file without noticeably changing its appearance.
This project implements **LSB (Least Significant Bit) Image Steganography** in **24-bit BMP images**, where:

* **Encoding:** A secret text file is embedded inside a BMP image.
* **Decoding:** The hidden message is extracted back from the stego BMP image.

This implementation strictly follows the **Emertxe Steganography Project** structure.

---

## 📁 **Folder Structure**

```
Steganography_Project/
│
├── lsb_steg.c
├── encode.c
├── encode.h
├── decode.c
├── decode.h
├── common.c
├── common.h
├── types.h
├── Makefile
│
├── beautiful.bmp      ← Original cover image
├── secret.txt         ← Secret message file
├── stego.bmp          ← Output encoded image (generated)
├── output.txt         ← Decoded message (generated)
│
└── README.md
```

---

## 🔧 **Compilation Instructions**

Open your terminal and run:

```sh
make
```

This produces the final executable:

```
lsb_steg
```

To clean compiled files:

```sh
make clean
```

---

## ▶️ **How to Use the Program**

---

### 🟦 **Encoding Mode (Hide Secret Data)**

Command:

```sh
./lsb_steg -e <input.bmp> <secret.txt> [output_stego.bmp]
```

Example:

```sh
./lsb_steg -e beautiful.bmp secret.txt stego.bmp
```

Output (sample):

```
INFO: Opening required files
INFO: Opened beautiful.bmp
INFO: Opened secret.txt
INFO: Opened stego.bmp
INFO: ## Encoding Procedure Started ##
...
INFO: ## Encoding Done Successfully ##
```

---

### 🟩 **Decoding Mode (Extract Secret Data)**

Command:

```sh
./lsb_steg -d <stego.bmp> [output.txt]
```

Example:

```sh
./lsb_steg -d stego.bmp output.txt
```

---

## 🧠 **How LSB Steganography Works**

A pixel in a 24-bit BMP image has 3 color channels:

* **R (Red)**
* **G (Green)**
* **B (Blue)**

Each channel is 8 bits → One pixel = **24 bits**

The least significant bit of each byte is altered to embed secret data:

```
Original Byte:  01100110
Modified Byte:  0110011*   ← We replace the last bit
```

The visual difference is **imperceptible** to the human eye.

### Data Embedded in Order:

1. **Magic string** (to identify stego images)
2. **Secret file extension** (e.g., "txt")
3. **Secret file size** (32 bits)
4. **Actual secret file data**

---

## 🧪 **Testing the Project**

### Step 1 — Encode:

```
./lsb_steg -e beautiful.bmp secret.txt stego.bmp
```

### Step 2 — Decode:

```
./lsb_steg -d stego.bmp decoded.txt
```
### Step 3 — Compare:

```
diff secret.txt decoded.txt
```

If the output is **empty**, encoding/decoding is successful.

---

## 🏗 **File Descriptions**

### 🔹 `lsb_steg.c`

Main controller file. Handles:

* Argument parsing
* Detecting encode/decode mode
* Calling respective modules

### 🔹 `encode.c / encode.h`

Contains all encoding logic:

* Read BMP header
* Modify pixel LSBs
* Encode magic string, extension, size, and data

### 🔹 `decode.c / decode.h`

Extracts data from stego image:

* Validates magic string
* Reads extension and file size
* Rebuilds secret file

### 🔹 `common.c / common.h`

Utility functions used by both modules.

### 🔹 `types.h`

Defines common data types:

* `uint`
* `Status` enum
* `OperationType` enum

---

## 🎯 **Features Implemented**

✔ LSB encoding in 24-bit BMP images
✔ Lossless secret file recovery
✔ Magic string signature to validate stego images
✔ Handles arbitrary text file sizes
✔ Command-line interface
✔ Emertxe-style logging and output format

---

## 📌 **Limitations**

* Works only with **24-bit uncompressed BMP images**
* Does not support JPG, PNG, GIF (due to compression)
* Assumes small to medium secret files (for classroom use)

---

## 🚀 **Future Improvements (Optional)**

* Add support for audio/video steganography
* Encrypt secret message before embedding
* Support for multi-byte UTF-8 text
* GUI version using GTK/Qt

---

## 👤 **Author**

**Krishna Moorthy**
Information Technology Department
Steganography Project – Academic Submission
---
