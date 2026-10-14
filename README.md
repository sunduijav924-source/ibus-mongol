# Mongol IME (IBus Engine for Latin-to-Cyrillic)

Linux (Debian) системд зориулсан латинаар бичихэд шууд криллээр буудаг, хөнгөн бөгөөд найдвартай IBus гарны хөдөлгүүр.

Windows-ийн Buuz гар шиг латин үсгээр бичихэд үг шууд крилл рүү хөрвөх бөгөөд бичих явцад үгээ харж байгаад бичнэ.

---

## Онцлог боломжууд

* **Бодит цагийн хөрвүүлэлт:** Бичиж байх явцад үг доогуураа зураастай харагдаж, `Space` эсвэл `Enter` дарахад жинхэнэ текст болж баталгаажна. //энэ шууд ажиллаж байгаа 
* **Засвар хийхэд хялбар:** `цасхан` гэх мэт үгийн дундуур `sh` үсэг орсон тохиолдолд `цас` гэж бичээд зай аваад, `хан` гэж бичсэний дараа дундах зайгаа арилгаж нийлүүлэх боломжтой. // эсвэл шууд латин `х` үсэг дарж хийх боломжтой  
* **Эгшгийн дүрэм:** Ямар ч эгшиг үсгийн араас `i` дарахад автоматаар `й` болно (`ai` -> `ай`, `oi` -> `ой`, `ui` -> `үй`, `ii` -> `ий`).
* **Том үсгийн дэмжлэг:** `Shift` болон `Caps Lock` ашиглан том үсгийг хэвийн бичнэ.
* **Системийн үйлдлүүд саадгүй:** `Ctrl+C`, `Ctrl+V`, `Ctrl+A` гэх мэт системийн товчлолуудтай зөрчилдөхгүй.

---
## Хувилбарын түүх
### v1.0.0
Анхны хувилбар (Фонетик латин-крилл хөрвүүлэгч, эгшгийн дүрэм, товчлолууд).


### v1.0.1 (Алдааг засварлав)
- **Ubuntu(GNOME Wayland) алдаа засвар:** `Space` болон `Enter` дарахад бичсэн үг алга болдог асуудлыг шийдвэрлэсэн.

## Үсгийн дүйцэл (Key Mapping)

| Латин | Крилл | Жишээ |
| :--- | :--- | :--- |
| `q` | ө | `qdor` -> өдөр |
| `w` | ү | `wneg` -> үнэг |
| `c` | ц | `cag` -> цаг |
| `x` эсвэл `h` | х | `xund` -> хүнд |
| `y` | ы | `ulsy` -> улсы |
| `'` (дангаар) | ь | `mor'` -> морь |
| `Shift + '` (`"`) | ъ | `og"` -> огъ |
| `sh` | ш | `shatar` -> шатар |
| `ch` | ч | `chuluu` -> чулуу |
| `ya`, `ye`, `yo`, `yu` | я, е, ё, ю | `yaria`, `ye` -> яриа, е |
| `эгшиг + i` | й | `sain` -> сайн |

---

## Суулгах заавар (.deb багц)

Энэ нь хамгийн хялбар бөгөөд хурдан арга юм:

1. [Releases](../../releases) хэсгээс хамгийн сүүлийн `ibus-mongol_1.0-1_amd64.deb` багцыг татаж авна.
2. Терминал нээгээд дараах коммандыг ажиллуулна: (ctrl+alt+t)
   ```bash
   sudo dpkg -i ibus-mongol_1.0-1_amd64.deb
   ibus-daemon -drx
3. Гараа системдээ нэмэх:

    Ubuntu (GNOME): Settings -> Keyboard -> Input Sources -> + дээр дарж Mongolian (Mongol)-г нэмнэ.

    Kubuntu (KDE): Терминал дээр ibus-setup комманд ажиллуулж, Input Method -> Add -> Mongolian (Mongol)-г сонгож нэмнэ.

4. Super + Space (Windows товч + Space) товчоор гараа сольж ашиглана .// although this failed i'm using it manually to change

# For developers 
if you guys want customize or idk do something
1. Шаардлагатай санг суулгах
sudo apt update
sudo apt install -y build-essential libibus-1.0-dev

2.  Төслийг татах
git clone [https://github.com/](https://github.com/)<таны_username>/ibus-mongol.git
cd ibus-mongol

3.  Хөрвүүлэх ба суулгах
make
sudo rm -f /usr/lib/ibus/ibus-engine-mongol
sudo cp ibus-engine-mongol /usr/lib/ibus/
sudo cp mongol.xml /usr/share/ibus/component/

4.  IBus-ийг шинэчлэх
ibus-daemon -drx
## Устгах 
sudo apt remove ibus-mongol
