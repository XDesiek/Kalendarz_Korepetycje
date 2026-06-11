# Kalendarz Korepetycji

Aplikacja desktopowa dla studentów udzielających korepetycji. Umożliwia zarządzanie harmonogramem zajęć, bazą uczniów i płatnościami, a także integrację z planem zajęć z systemu USOS.

Projekt realizowany w ramach kursu **Programowanie 2: Programowanie Obiektowe w C++** na Politechnice Warszawskiej.

**Autorzy:** Antoni Kamiński, Wiktor Głuszczak

---

## Funkcjonalności

- Widok kalendarza tygodniowego z nawigacją między tygodniami
- Dodawanie korepetycji (data, godzina, uczeń)
- Zarządzanie bazą uczniów (imię, nazwisko, telefon, komunikator)
- Rejestrowanie płatności za jedną lub więcej lekcji naraz
- Import zajęć z planu USOS w formacie `.ics`
- Kolorowanie kafelków: zielony = opłacone, czerwony = nieopłacone, żółty = USOS
- Wykrywanie konfliktów — aplikacja nie pozwala dodać lekcji w zajętym terminie
- Trwały zapis danych w plikach CSV

---

## Wymagania

- Qt 6.x (moduły: `core`, `widgets`)
- Kompilator C++17 (np. GCC, MSVC, Clang)
- Qt Creator (zalecane) lub dowolne środowisko z qmake

---

## Uruchomienie

1. Sklonuj repozytorium:
```bash
git clone https://github.com/XDesiek/Kalendarz_Korepetycje.git
cd Kalendarz_Korepetycje
```

2. Otwórz plik `WeeklyCalendar.pro` w Qt Creator i kliknij **Run**, lub z wiersza poleceń:
```bash
qmake WeeklyCalendar.pro
make
./WeeklyCalendar
```

3. Przy pierwszym uruchomieniu aplikacja automatycznie utworzy puste pliki CSV w folderze z plikiem wykonywalnym.

---

## Pliki danych

Aplikacja przechowuje dane lokalnie w czterech plikach CSV w tym samym folderze co plik wykonywalny:

| Plik | Zawartość |
|------|-----------|
| `students.csv` | Baza uczniów |
| `student_lessons.csv` | Lekcje korepetycji |
| `usos_lessons.csv` | Zajęcia zaimportowane z USOS |
| `payments.csv` | Historia płatności |

---

## Import planu z USOS

1. Zaloguj się do systemu USOS swojej uczelni
2. Pobierz plan zajęć jako plik `.ics` (format kalendarza)
3. W aplikacji kliknij przycisk **Wczytaj Zajęcia** i wskaż pobrany plik
4. Zajęcia z USOS pojawią się w kalendarzu w kolorze żółtym

---

## Struktura projektu

```
├── main.cpp                    # punkt wejścia aplikacji
│
├── — Model —
│   ├── ICalendarEntry.h        # interfejs bazowy dla wszystkich wpisów kalendarza
│   ├── ILesson.h / .cpp        # abstrakcyjna klasa bazowa dla lekcji
│   ├── LessonStudent.h / .cpp  # lekcja korepetycji z uczniem
│   ├── LessonUSOS.h / .cpp     # zajęcia zaimportowane z USOS
│   ├── Student.h / .cpp        # dane ucznia
│   └── Payment.h / .cpp        # płatność za lekcje
│
├── — Logika —
│   ├── CalendarEngine.h / .cpp # główny koordynator aplikacji
│   ├── DaySchedule.h / .cpp    # plan jednego dnia
│   ├── WeekSchedule.h / .cpp   # plan tygodnia (7 × DaySchedule)
│   └── LessonMerger.h / .cpp   # łączenie i sortowanie lekcji
│
├── — Dane —
│   └── CSVStorage.h / .cpp     # zapis/odczyt CSV oraz parser plików ICS
│
└── — Widok —
    ├── ICalendarView.h         # interfejs widoku kalendarza
    ├── LessonInfo.h            # struktura danych przekazywana do kafelka
    ├── LessonTile.h / .cpp     # pojedynczy kafelek w siatce kalendarza
    ├── WeeklyCalendarWidget.h / .cpp  # siatka kalendarza tygodniowego
    ├── mainwindow.h / .cpp     # główne okno aplikacji
    └── mainwindow.ui           # layout UI (Qt Designer)
```

---

## Diagram klas (uproszczony)

```
ICalendarEntry  (interfejs)
    └── ILesson  (abstrakcyjna)
            ├── LessonStudent
            └── LessonUSOS

ICalendarView  (interfejs)
    └── MainWindow

CalendarEngine
    ├── WeekSchedule → DaySchedule → ICalendarEntry*
    ├── CSVStorage
    └── LessonMerger

MainWindow
    ├── CalendarEngine
    └── WeeklyCalendarWidget → LessonTile[]
```

---

## Paradygmaty OOP

| Paradygmat | Przykład w projekcie |
|---|---|
| **Dziedziczenie** | `LessonStudent`, `LessonUSOS` dziedziczą po `ILesson` |
| **Polimorfizm** | `DaySchedule` przechowuje `ICalendarEntry*` nie wiedząc czy to korepetycja czy USOS |
| **Abstrakcja** | `ICalendarEntry`, `ICalendarView` — czyste interfejsy |
| **Enkapsulacja** | Wszystkie pola klas prywatne, dostęp przez gettery/settery |
