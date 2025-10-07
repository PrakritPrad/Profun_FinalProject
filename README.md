# ✈️ Flight Booking System

ระบบจำลองการจองตั๋วเครื่องบินแบบ command-line เขียนด้วยภาษา **C**  
รองรับฟังก์ชัน:
- แสดงที่นั่งแต่ละเที่ยวบิน  
- จองที่นั่งพร้อมรหัสการจอง (Booking Code) แบบไม่ซ้ำ  
- ยกเลิกที่นั่ง  
- ค้นหาผู้โดยสาร  
- บันทึกและโหลดข้อมูลจากไฟล์ CSV  
- มี **Unit Test** และ **End-to-End (E2E) Test** สำหรับตรวจสอบความถูกต้องของระบบ  

---

## 📁 โครงสร้างไฟล์

```
output/
│
├── app.exe                # โปรแกรมหลัก (ไฟล์รัน)
├── Main.c                 # main() ของระบบ
├── Booking_system.h       # Header รวมโครงสร้างและฟังก์ชันหลัก
├── Unit_Test.c            # Unit test แต่ละฟังก์ชันย่อย
├── E2E_Test.c             # ทดสอบระบบแบบครบวงจร
├── passengers.csv         # ไฟล์เก็บข้อมูลผู้โดยสารจริง
├── test_passengers.csv    # ไฟล์จำลองผู้โดยสารสำหรับทดสอบ
└── README.md              # ไฟล์อธิบายโปรเจกต์ (คุณอยู่ที่นี่)
```

---

## ⚙️ การคอมไพล์ (How to Compile)

### 🧱 1. คอมไพล์โปรแกรมหลัก
```bash
gcc Main.c -o app.exe
```
จากนั้นรันได้ด้วย
```bash
./app.exe
```

---

### 🧪 2. คอมไพล์และรัน Unit Test
```bash
gcc Unit_Test.c -o unit_test.exe
./unit_test.exe
```

---

### 🔄 3. คอมไพล์และรัน End-to-End Test
```bash
gcc E2E_Test.c -o e2e_test.exe
./e2e_test.exe
```

---

## 💾 ระบบไฟล์ CSV

| คอลัมน์ | ความหมาย |
|----------|------------|
| SeatID | ที่นั่ง เช่น A1–C5 |
| Name | ชื่อผู้โดยสาร |
| Booked | สถานะ (0=ว่าง, 1=จองแล้ว) |
| Code | รหัสการจองแบบไม่ซ้ำ |

ระบบจะโหลดและบันทึกไฟล์อัตโนมัติเมื่อมีการจองหรือยกเลิก

---

## 🧩 ฟีเจอร์หลัก

| ฟังก์ชัน | คำอธิบาย |
|-----------|------------|
| `init_flights()` | สร้างเที่ยวบินเริ่มต้น |
| `view_seats()` | แสดงสถานะที่นั่ง |
| `add_passenger()` | จองที่นั่ง พร้อมรหัสสุ่มไม่ซ้ำ |
| `cancel_passenger()` | ยกเลิกการจอง |
| `search_menu()` | ค้นหาผู้โดยสารจากชื่อหรือโค้ด |
| `save_to_csv()` / `load_from_csv()` | จัดการไฟล์ข้อมูล |
| `generate_unique_code()` | สร้างรหัสการจองแบบไม่ซ้ำ |

---

## 🧠 หมายเหตุ

- ใช้ `fgets()` แทน `scanf()` เพื่อป้องกัน buffer overflow  
- ใช้ `assert()` ใน Unit Test เพื่อตรวจสอบค่าที่คาดหวัง  
- การจองที่นั่งไม่สนตัวพิมพ์เล็ก/ใหญ่ (case-insensitive)  
- CSV จะรีเฟรชข้อมูลทุกครั้งที่มีการจองหรือยกเลิก  

---

## 🏁 ตัวอย่างการรัน

```
==== Flight Booking System ====
1) View Flights
2) Manage Seats
3) Search Passenger
4) Save & Exit
Choose: 1

Flight: FL001 | From: Bangkok | To: Paris | Date: 2025-10-29
Seats:
A1 [Available]
A2 [Booked by Somchai]
...
```

---

© 2025 Cyber Engi — All Rights Reserved 🛫
