Utwórz program w języku c++ na windows który zarządza kolejką do lekarzy w Polsce. Mamy 16 województw w każdym jest jeden szpital, w każdym szpitalu jest 3 specjalistów i jeden lekarz ogólny.
W plikach tekstowych znajdują się dane do klas.
W pliku doctors.txt mamy dane dla doktorów w postaci:
Województwo - Nazwa szpitala
	Imię Nazwisko - specjalizacja
przykład:
Dolnośląskie - Wojewódzki Szpital Specjalistyczny we Wrocławiu
	Jan Kowalski - Ortopeda
	Anna Nowak - Kardiolog
	Tomasz Wiśniewski - Dermatolog
	Maria Zielińska - Lekarz ogólny

W pliku symptoms.txt mamy dane jakie problemy rozwiązują specjaliści w postaci:
Specjalista: problem pacjenta
przykład:
Ortopeda: ból kolana, bol kolana, kolano boli, kolanko, bol nogi, bóle stawu, bol stawu, ból stawu, uraz kolana, zlamanie, złamanie, skręcenie, skrecenie, obolale kolano

W pliku patients.txt znajdują się dane pacjenta:
Imię Nazwisko Wiek Województwo Problem jaki ma
Przykład:
Jan Kowalski 35 Podkarpacie ból kolana

Do pliku patients.txt możemy dodawać pacjentów poprzez menu programu

Potem przypisujemy pacjentów do lekarzy na podstawie problemy jaki mu doskwiera i województwa w jakim się znajduje.
Jeśli nie ma specjalisty który zajmuje się danym problemem to jest wysyłany do lekarza ogólnego, jeśli dany specjalista jest w innym województwie niż pacjent pytamy czy chce zapisać się u tego specjalisty czy u lekarza ogólnego w swoim województwie.

Następnie ustalamy datę i godzinę wizyty u danego specjalisty/lekarza ogólnego. Specjaliści przyjmują pacjentów od poniedziałku do piątku od godziny 7 do 15, a lekarz ogólny
od 10 do 20, każda wizyta zajmuje 30 minut.
  
Kryteria które musi spełnić.
Implementacja klasy posiadającej konstruktor domyślny, konstruktor kopiujący i destruktor.
Implementacja funkcji umożliwiających dostęp do składowych prywatnych klasy.
Implementacja funkcji umożliwiającej drukowanie danych obiektu na standardowe wyjście.
Implementacja klasy pochodnej z nowymi składowymi, redefinicją funkcji bazowych i dodaniem nowych funkcji dla klasy pochodnej
Implementacja funkcji wirtualnych dla zaimplementowanej hierarchii klas.
Implementacja dynamicznej alokacji pamięci przez konstruktor lub funkcje klasy oraz automatycznego zwalniania pamięci przez destruktor.
Implementacja funkcji realizujących specyficzne operacje na obiektach.
Implementacja klasy abstrakcyjnej.
Implementacja hierarchii klas złożonej z co najmniej dwóch poziomów i przynajmniej 5 klas.
Implementacja interfejsu opartego na plikach testowych, który umożliwia automatyczne tworzenie obiektów na podstawie tekstowych plików konfiguracyjnych
Projekt zawiera implementację biblioteki dynamicznej zaimplementowanej w C/C++.
Projekt implementuje interfejs Python do biblioteki dynamicznej C/C++.
Biblioteka dynamiczna inicjuje losowe dane obliczeniowe na podstawie parametru podanego przez użytkownika.
Biblioteka wykonuje po wywołaniu określonej funkcji obliczenia równoległe z wykorzystaniem OpenMP.
Biblioteka mierzy czas realizacji obliczeń.
Biblioteka wykorzystuje jedną ze struktur danych: vector, list lub map.
Biblioteka umożliwia ustawienie liczby wątków obliczeniowych, na których ma pracować.
Biblioteka umiejętnie dzieli zadania, aby optymalnie wykorzystać przydzielone jej zasoby obliczeniowe.
Biblioteka implementuje zaawansowany, własny (zaimplementowany przez studenta) algorytm obliczeniowy.
Biblioteka pozwala na wskazania jaki procent dostępnych zasobów obliczeniowych ma zostać wykorzystany do realizacji obliczeń (nie przez wskazanie liczby wątków, ale przez określenie, że na przykład biblioteka ma do dyspozycji 30% zasobów obliczeniowych).
