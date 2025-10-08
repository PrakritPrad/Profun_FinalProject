# ✈️ Flight Booking System
*********สวัสดีครับอาจาร์ยย C:*********
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
path ตอนแรกที่อยู่
```bash
\Profun_FinalProject>
```
### 🧱 1. คอมไพล์โปรแกรมหลัก
```bash
gcc .\Main.c .\E2E_Test.c .\Unit_Test.c -o .\output\app.exe
```
จากนั้นรันได้ด้วย
```bash
.\output\app.exe
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
--- Manage FL001 (Bangkok -> Paris) ---
1) View Seats
2) Add Passenger
3) Cancel Passenger
4) Update Passenger
5) Back
Choose: 1

Flight: FL001 | From: Bangkok | To: Paris | Date: 2025-10-29
Seats:
A1 [Available]
A2 [Booked by Somchai]
...
```

---

© Prakrit  🛫
